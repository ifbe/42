#include <stdio.h>
#include <windows.h>
#include "libuser.h"




static struct arena* working;
static WAVEFORMATEX fmt;
static int alive = 0;
//
static HWAVEIN wavein;
static WAVEHDR headin[16];
static u8* ibuf = 0;
static int icur = 0;
//
static HWAVEOUT waveout;
static WAVEHDR headout[16];
static u8* obuf = 0;
static int ocur = 0;
static int olen = 0;



void soundwrite(int dev, int time, u8* buf, int len);
static void CALLBACK icb(HWAVEOUT hWave, UINT uMsg, DWORD dwInstance, DWORD dw1, DWORD dw2)
{
	//printf("@icb\n");
	if(WIM_DATA == uMsg)
	{
		//soundwrite(0, 0, ibuf + (1024*2*icur), 2048);
		//printf("WIM_DATA:%d\n", icur);

		struct relation* orel = (struct relation*)(working->orel0);
		while(1)
		{
			if(0 == orel)break;
			if(_act_ == orel->dsttype)
			{
				struct halfrel* self = (struct halfrel*)&orel->dstchip;
				struct halfrel* peer = (struct halfrel*)&orel->srcchip;
				actor_rootwrite(self, peer, ibuf + (1024*2*icur), 1024*2);
			}
			orel = (struct relation*)samesrcnextdst(orel);
		}

		waveInAddBuffer(wavein, &headin[icur], sizeof (WAVEHDR));
		icur = (icur+1)%16;
	}
	else if(WIM_OPEN == uMsg)
	{
		printf("WIM_OPEN\n");
	}
	else if(WIM_CLOSE == uMsg)
	{
		printf("WIM_CLOSE\n");
	}
}
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




void soundlist()
{
}
void soundchoose()
{
}
void soundread(
	struct arena* win, struct style* sty,
	struct actor* act, struct style* pin)
{
	if(0 == act)return;
}
void soundwrite(int dev, int time, u8* buf, int len)
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
void soundstop()
{
	waveInClose(wavein);
	waveOutClose(waveout);
	//waveOutUnprepareHeader(waveout, &headout, sizeof(WAVEHDR));
}
void soundstart()
{
}
void sounddelete(struct arena* win)
{
}
void soundcreate(struct arena* win)
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

	fmt.wFormatTag = WAVE_FORMAT_PCM;
	fmt.nAvgBytesPerSec = 8000*2;
	fmt.nSamplesPerSec = 8000;
	fmt.wBitsPerSample = 16;
	fmt.nChannels = 1;
	fmt.nBlockAlign = 2;
	fmt.cbSize = 0;
	ret = waveInOpen(
		&wavein, WAVE_MAPPER,
		&fmt, (DWORD_PTR)icb,
		0L, CALLBACK_FUNCTION
	);
	say("@waveInOpen:%d\n", ret);

	icur = 0;
	ibuf = malloc(1024*2*16);
	for(j=0;j<16;j++)
	{
		headin[j].lpData = ibuf + (1024*2*j);
		headin[j].dwBufferLength = 1024*2;
		headin[j].dwBytesRecorded = 0;
		headin[j].dwUser = 0;
		headin[j].dwFlags = 0;
		headin[j].dwLoops = 1;
		waveInPrepareHeader(wavein, &headin[j], sizeof(WAVEHDR));
		waveInAddBuffer(wavein, &headin[j], sizeof (WAVEHDR));
	}

	waveInStart(wavein);
}




void initmic()
{
}
void freemic()
{
}