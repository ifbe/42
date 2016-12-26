#include <stdio.h>
#include <windows.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
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
	headin.dwBufferLength = len;
	headin.dwBytesRecorded = 0;
	headin.dwUser = 0;
	headin.dwFlags = 0;
	headin.dwLoops = 1;

	waveInPrepareHeader(wavein, &headin, sizeof(WAVEHDR));
	waveInAddBuffer(wavein, &headin, sizeof (WAVEHDR));
	waveInStart(wavein);
	Sleep(1000);
	waveInReset(wavein);
}
void writesound(u8* buf, int len)
{
	headout.dwLoops = 0L;
	headout.lpData = buf;
	headout.dwBufferLength = len;
	headout.dwFlags = 0L;
	waveOutPrepareHeader(waveout, &headout, sizeof(WAVEHDR));
	waveOutWrite(waveout, &headout, sizeof(WAVEHDR));
}
void startsound()
{
	//in
	HANDLE wait;
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
	fmt.wFormatTag = WAVE_FORMAT_PCM;	//声音格式为PCM
	fmt.nSamplesPerSec = 44100;		//采样率，16000次/秒
	fmt.wBitsPerSample = 16;		//采样比特，16bits/次
	fmt.nChannels = 2;			//采样声道数，2声道
	fmt.nAvgBytesPerSec = 44100*4;		//每秒多少字节的数据
	fmt.nBlockAlign = 4;			//一个块的大小
	fmt.cbSize = 0;
}
void deletesound()
{
}
