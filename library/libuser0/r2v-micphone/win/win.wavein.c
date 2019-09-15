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




static void CALLBACK icb(HWAVEOUT hWave, UINT uMsg, DWORD dwInstance, DWORD dw1, DWORD dw2)
{
	//printf("@icb\n");
	if(WIM_DATA == uMsg)
	{
		//printf("WIM_DATA:%d\n", icur);
		relationwrite(working, _dst_, 0, 0, ibuf + (1024*2*icur), 1024*2);
/*
		struct relation* orel = (struct relation*)(working->orel0);
		while(1)
		{
			if(0 == orel)break;
			if(_act_ == orel->dsttype)
			{
				struct halfrel* self = (struct halfrel*)&orel->dstchip;
				struct halfrel* peer = (struct halfrel*)&orel->srcchip;
				actorwrite(self, peer, ibuf + (1024*2*icur), 1024*2);
			}
			orel = (struct relation*)samesrcnextdst(orel);
		}
*/
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




void micphonelist()
{
}
void micphonechoose()
{
}
void micphoneread(
	struct arena* win, struct style* sty,
	struct actor* act, struct style* pin)
{
}
void micphonewrite(int dev, int time, u8* buf, int len)
{
}
void micphonestop()
{
	waveInClose(wavein);
}
void micphonestart()
{
}
void micphonedelete(struct arena* win)
{
}
void micphonecreate(struct arena* win)
{
	int j,ret;
	working = win;

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
