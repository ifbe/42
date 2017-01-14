#include <stdio.h>
#include <windows.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void eventwrite(u64,u64,u64,u64);
void printmemory(void*, int);




//
static int alive=0;
static HANDLE thread=0;
static char* buffer;
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
		headin.lpData = buffer;
		headin.dwBufferLength = 1024*4;
		headin.dwBytesRecorded = 0;
		headin.dwUser = 0;
		headin.dwFlags = 0;
		headin.dwLoops = 1;

		waveInPrepareHeader(wavein, &headin, sizeof(WAVEHDR));
		waveInAddBuffer(wavein, &headin, sizeof (WAVEHDR));
		waveInStart(wavein);
		Sleep(23);
		waveInReset(wavein);

		eventwrite(0, 's', 0, 0);
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
			printf("done\n");
		}
	}
}




void listsound()
{
}
void choosesound()
{
}
void readsound(u8* buf, int len)
{
}
void writesound(u8* buf, int len)
{
	headout.dwLoops = 0L;
	headout.lpData = buf;
	headout.dwBufferLength = 44100*chan;
	headout.dwFlags = 0L;
	waveOutPrepareHeader(waveout, &headout, sizeof(WAVEHDR));
	waveOutWrite(waveout, &headout, sizeof(WAVEHDR));
}
void startsound(unsigned int ra, int ch, void* buf, int max)
{
	freq = ra;
	chan = 1;
	buffer = buf;

	//both
	fmt.wFormatTag = WAVE_FORMAT_PCM;	//声音格式为PCM
	fmt.nChannels = 1;			//采样声道数，2声道
	fmt.nSamplesPerSec = freq;		//采样率，16000次/秒
	fmt.wBitsPerSample = 16;		//采样比特，16bits/次
	fmt.nAvgBytesPerSec = freq*chan;		//每秒多少字节的数据
	fmt.nBlockAlign = 2;			//一个块的大小
	fmt.cbSize = 0;

	//in
	HANDLE wait;
	wait = CreateEvent(NULL, 0, 0, NULL);
	waveInOpen(&wavein, WAVE_MAPPER, &fmt, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);

	//out
	waveOutOpen(&waveout, WAVE_MAPPER, &fmt, (u64)CB, 0L, CALLBACK_FUNCTION);

	//
	alive = 1;
	thread = CreateThread(NULL, 0, soundlistener, NULL, 0, NULL);
}
void stopsound()
{
	waveInClose(wavein);
	waveOutUnprepareHeader(waveout, &headout, sizeof(WAVEHDR));
}
void createsound()
{
}
void deletesound()
{
}
