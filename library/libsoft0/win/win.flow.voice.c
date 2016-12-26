#include <stdio.h>
#include <windows.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(void*, int);




//
static WAVEFORMATEX fmt;
//
static HWAVEIN wavein;
static WAVEHDR headin;
//
static HWAVEOUT waveout;
static WAVEHDR headout;




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
	headin.lpData = buf;
	headin.dwBufferLength = 0x100000;
	headin.dwBytesRecorded = 0;
	headin.dwUser = 0;
	headin.dwFlags = 0;
	headin.dwLoops = 1;

	waveInPrepareHeader(wavein, &headin, sizeof(WAVEHDR));
	waveInAddBuffer(wavein, &headin, sizeof (WAVEHDR));
	waveInStart(wavein);
	Sleep(1000);
	waveInReset(wavein);

	printf("%d\n", headin.dwBytesRecorded);
	//printmemory(buf+0x3000, 0x1000);
}
void writesound(u8* buf, int len)
{
	headout.dwLoops = 0L;
	headout.lpData = buf;
	headout.dwBufferLength = 44100*2;
	headout.dwFlags = 0L;
	waveOutPrepareHeader(waveout, &headout, sizeof(WAVEHDR));
	waveOutWrite(waveout, &headout, sizeof(WAVEHDR));
}
void startsound()
{
	//both
	fmt.wFormatTag = WAVE_FORMAT_PCM;	//声音格式为PCM
	fmt.nChannels = 1;			//采样声道数，2声道
	fmt.nSamplesPerSec = 44100;		//采样率，16000次/秒
	fmt.wBitsPerSample = 16;		//采样比特，16bits/次
	fmt.nAvgBytesPerSec = 44100*2;		//每秒多少字节的数据
	fmt.nBlockAlign = 2;			//一个块的大小
	fmt.cbSize = 0;

	//in
	HANDLE wait;
	wait = CreateEvent(NULL, 0, 0, NULL);
	waveInOpen(&wavein, WAVE_MAPPER, &fmt, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);

	//out
	waveOutOpen(&waveout, WAVE_MAPPER, &fmt, (u64)CB, 0L, CALLBACK_FUNCTION);
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
