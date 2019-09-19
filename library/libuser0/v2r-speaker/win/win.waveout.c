#include <stdio.h>
#include <windows.h>
#include "libuser.h"




static struct arena* working;
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




void speakerlist()
{
}
void speakerchoose()
{
}
void speakerread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct arena* spk = (void*)(self->chip);
	if(0 == spk)return;
	//say("spk=%llx\n",spk);

	struct relation* rel = spk->orel0;
	if(0 == rel)return;
	//say("rel=%llx\n",rel);

	struct actor* act = (void*)(rel->dstchip);
	if(0 == act)return;
	//say("act=%llx\n",act);

	ZeroMemory(&headout[ocur], sizeof(WAVEHDR));
	headout[ocur].lpData = act->ctx;
	headout[ocur].dwBufferLength = 40000*2;
	headout[ocur].dwFlags = 0L;
	headout[ocur].dwLoops = 0L;
	waveOutPrepareHeader(waveout, &headout[ocur], sizeof(WAVEHDR));
	waveOutWrite(waveout, &headout[ocur], sizeof(WAVEHDR));

	ocur = (ocur+1) % 16;
	olen = olen+len;

	sleep_us(1000*1000);
}
void speakerwrite(int dev, int time, u8* buf, int len)
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
void speakerstop()
{
	waveOutClose(waveout);
	//waveOutUnprepareHeader(waveout, &headout, sizeof(WAVEHDR));
}
void speakerstart()
{
}
void speakerdelete(struct arena* win)
{
}
void speakercreate(struct arena* win)
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
	say("@waveOutOpen:%d\n", ret);
}




void initspeaker()
{
}
void freespeaker()
{
}