#include <stdio.h>
#include <windows.h>
#include "arena.h"
#define _mic_ hex32('m','i','c',0)




struct waveinfo
{
	void* buf;
	int len;
	int enq;
	int deq;
};
static struct waveinfo info;
//
static int alive=0;
static u64 thread=0;
//
static int freq;
static int chan;
static WAVEFORMATEX fmt;
//
static HWAVEIN wavein;
static WAVEHDR headin;
//
static HWAVEOUT waveout;
static WAVEHDR headout;




DWORD WINAPI soundlistener(LPVOID pM)
{
	int ret;
	u32 count=0;

	while(alive == 1)
	{
		headin.lpData = (info.buf)+(info.enq);
		headin.dwBufferLength = 4096;
		headin.dwBytesRecorded = 0;
		headin.dwUser = 0;
		headin.dwFlags = 0;
		headin.dwLoops = 1;

		waveInPrepareHeader(wavein, &headin, sizeof(WAVEHDR));
		waveInAddBuffer(wavein, &headin, sizeof (WAVEHDR));
		waveInStart(wavein);
		Sleep(23);
		waveInReset(wavein);

		info.enq = (info.enq + 4096) % 0x100000;
		eventwrite((u64)&info, _mic_, 0, 0);
	}
	return 0;
}
static void CALLBACK CB(HWAVEOUT hWave, UINT uMsg, DWORD dwInstance, DWORD dw1, DWORD dw2)
{
	switch (uMsg)
	{
		case WOM_DONE://上次缓存播放完成,触发该事件
		{
			headout.dwBufferLength = 0;
			//printf("done\n");
		}
	}
}




void soundlist()
{
}
void soundchoose()
{
}
void soundread(
	struct window* win, struct style* sty,
	void* act, void* pin)
{
	printf("%llx,%llx,%llx,%llx\n", win, sty, act, pin);
	if(0 == act)return;
}
void soundwrite(u8* buf, int len)
{
	headout.dwLoops = 0L;
	headout.lpData = buf;
	headout.dwBufferLength = len;
	headout.dwFlags = 0L;
	waveOutPrepareHeader(waveout, &headout, sizeof(WAVEHDR));
	waveOutWrite(waveout, &headout, sizeof(WAVEHDR));
}
void soundstop()
{
	waveInClose(wavein);
	waveOutUnprepareHeader(waveout, &headout, sizeof(WAVEHDR));
}
void soundstart(unsigned int ra, int ch)
{
	freq = ra;
	chan = 1;

	//both
	fmt.wFormatTag = WAVE_FORMAT_PCM;	//声音格式为PCM
	fmt.nChannels = 1;			//采样声道数，2声道
	fmt.nSamplesPerSec = freq;		//采样率，16000次/秒
	fmt.wBitsPerSample = 16;		//采样比特，16bits/次
	fmt.nAvgBytesPerSec = freq*chan;	//每秒多少字节的数据
	fmt.nBlockAlign = 2;			//一个块的大小
	fmt.cbSize = 0;

	//in
	HANDLE wait;
	wait = CreateEvent(NULL, 0, 0, NULL);
	waveInOpen(&wavein, WAVE_MAPPER, &fmt, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);

	//out
	waveOutOpen(&waveout, WAVE_MAPPER, &fmt, (u64)CB, 0L, CALLBACK_FUNCTION);
	//
	if(info.buf == 0)
	{
		info.enq = 0;
		info.deq = 0;
		info.len = 0x100000;
		info.buf = malloc(info.len);
	}

	//
	alive = 1;
	thread = startthread(soundlistener, 0);
}
void sounddelete()
{
}
void soundcreate()
{
}




void initmic()
{
}
void freemic()
{
}
