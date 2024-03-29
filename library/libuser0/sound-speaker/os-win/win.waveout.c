#include <stdio.h>
#include <windows.h>
#include "libuser.h"




static _obj* working;
static WAVEFORMATEX fmt;
static int alive = 0;
//
static HWAVEOUT waveout;
static WAVEHDR headout[16];
static u8* obuf = 0;
static int ocur = 0;
static int olen = 0;




static void CALLBACK ocb(HWAVEOUT hWave, UINT uMsg, DWORD dwInstance, DWORD dw1, DWORD dw2)
{
	//printf("@ocb\n");
	if(WIM_DATA == uMsg)
	{
		printf("WOM_DONE\n");
	}
	else if(WOM_OPEN == uMsg)
	{
		printf("WOM_OPEN\n");
	}
	else if(WOM_CLOSE == uMsg)
	{
		printf("WOM_CLOSE\n");
	}
}




void speaker_take(_obj* spk,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	struct pcmdata* pcm;
	if(spk->orel0)take_data_from_peer(spk,_ctx_, stack,sp, 0,0, 0,0);

	pcm = spk->pcmeasy.data;
	if(0 == pcm)return;

	ZeroMemory(&headout[ocur], sizeof(WAVEHDR));
	headout[ocur].lpData = (void*)(pcm->buf);
	headout[ocur].dwBufferLength = pcm->count*2;
	headout[ocur].dwFlags = 0L;
	headout[ocur].dwLoops = 0L;
	waveOutPrepareHeader(waveout, &headout[ocur], sizeof(WAVEHDR));
	waveOutWrite(waveout, &headout[ocur], sizeof(WAVEHDR));

	ocur = (ocur+1) % 16;
	olen = olen+len;

	sleep_us(1000*1000);
}
void speaker_give(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	int j;
	if(0 == obuf)obuf = malloc(0x100000);
	if(len >= 0x100000)return;

	if(olen+len >= 0x100000)olen = 0;
	for(j=0;j<len;j++)obuf[olen+j] = buf[j];

	ZeroMemory(&headout[ocur], sizeof(WAVEHDR));
	headout[ocur].lpData = obuf+olen;
	headout[ocur].dwBufferLength = len;
	headout[ocur].dwFlags = 0L;
	headout[ocur].dwLoops = 0L;
	waveOutPrepareHeader(waveout, &headout[ocur], sizeof(WAVEHDR));
	waveOutWrite(waveout, &headout[ocur], sizeof(WAVEHDR));

	ocur = (ocur+1) % 16;
	olen = olen+len;
}
int speaker_attach()
{
	return 0;
}
int speaker_detach()
{
	return 0;
}




int speaker_reader(_obj* spk,void* foot, p64 arg,int idx, void* buf, int len)
{
	return 0;
}
int speaker_writer(_obj* spk,void* foot, p64 arg,int idx, void* buf, int len)
{
	return 0;
}
void speaker_delete(_obj* win)
{
	waveOutClose(waveout);
	//waveOutUnprepareHeader(waveout, &headout, sizeof(WAVEHDR));
}
void speaker_create(_obj* win)
{
	int j,ret;
	working = win;

	fmt.wFormatTag = WAVE_FORMAT_PCM;
	fmt.nAvgBytesPerSec = 44100*2;
	fmt.nSamplesPerSec = 44100;
	fmt.wBitsPerSample = 16;
	fmt.nChannels = 1;
	fmt.nBlockAlign = 2;
	fmt.cbSize = 0;
	ret = waveOutOpen(
		&waveout, WAVE_MAPPER,
		&fmt, (DWORD_PTR)ocb,
		0L, CALLBACK_FUNCTION
	);
	logtoall("@waveOutOpen:%d\n", ret);
}




void initspeaker()
{
}
void freespeaker()
{
}
