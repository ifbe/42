#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
u64* eventread();
void eventwrite(u64,u64,u64,u64);
//
u64 startthread(void*, void*);
void stopthread();
//
void printmemory(void*, int);
void say(void*, ...);




//
static u64 thread;
static int alive = 1;
static int freq;
static int channel;
//
static snd_pcm_t* capture_handle;
static snd_pcm_hw_params_t* capture_params;
static u8* bufin = 0;
//
static snd_pcm_t* playback_handle;
static snd_pcm_hw_params_t* playback_params;
static snd_pcm_uframes_t frames;
static int dir=0;
static int count=0;
static u8* bufout = 0;




void* soundlistener(void* p)
{
	int j,k;
	int count;

	k=0;
	while(alive)
	{
		j = snd_pcm_readi(capture_handle, bufin, frames);
		if(j != frames)
		{
			printf("snd_pcm_readi:%s\n", snd_strerror(j));
		}

		if(bufout == 0)j = snd_pcm_writei(playback_handle, bufin, frames);
		else j = snd_pcm_writei(playback_handle, bufout, frames);
		if(j != frames)
		{
			printf("snd_pcm_writei:%s\n", snd_strerror(j));
			if(j == -EPIPE)
			{
				snd_pcm_prepare(playback_handle);
			}
		}

		usleep(frames*10000/441/2);	//dont know need /2
		k += frames;
		count++;
		if(k >= 1024)
		{
			eventwrite(0, 's', 0, 0);
			k = 0;
		}
		if(count > 0x8000/frames)
		{
			bufout = 0;
			count = 0;
		}
	}
}
int startsound_capture(unsigned int rate, int ch)
{
	int err;

	//printf("@snd_pcm_open\n");
	err = snd_pcm_open(
		&capture_handle, "hw:1,0",
		SND_PCM_STREAM_CAPTURE, SND_PCM_NONBLOCK
	);
	if (err < 0) {
		printf("%s\n", snd_strerror (err));
		return -1;
	}

	//printf("@snd_pcm_hw_params_malloc\n");
	err = snd_pcm_hw_params_malloc (&capture_params);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -2;
	}

	//printf("@snd_pcm_hw_params_any\n");
	err = snd_pcm_hw_params_any (capture_handle, capture_params);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -3;
	}

	//printf("@snd_pcm_hw_params_set_access\n");
	err = snd_pcm_hw_params_set_access(
		capture_handle, capture_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -4;
	}

	//printf("@snd_pcm_hw_params_set_format\n");
	err = snd_pcm_hw_params_set_format(
		capture_handle, capture_params, SND_PCM_FORMAT_S16_LE);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -5;
	}

	//printf("@snd_pcm_hw_params_set_rate_near\n");
	err = snd_pcm_hw_params_set_rate_near (capture_handle, capture_params, &rate, 0);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -6;
	}

	//printf("@snd_pcm_hw_params_set_channels\n");
	err = snd_pcm_hw_params_set_channels (capture_handle, capture_params, ch);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -7;
	}

	//printf("@snd_pcm_hw_params\n");
	err = snd_pcm_hw_params (capture_handle, capture_params);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -8;
	}
	snd_pcm_hw_params_free (capture_params);

	//printf("@snd_pcm_hw_params\n");
	err = snd_pcm_prepare (capture_handle);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -9;
	}

	//
	return 1;
}
int startsound_playback(unsigned int frequency, int channels)
{
	int rc;

	rc = snd_pcm_open(&playback_handle, "hw:1,0", SND_PCM_STREAM_PLAYBACK, 0);
	if(rc<0)
	{
		perror("\nopen PCM device failed:");
		return -1;
	}

	snd_pcm_hw_params_alloca(&playback_params); //分配params结构体
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_alloca:");
		return -2;
	}

	rc=snd_pcm_hw_params_any(
	playback_handle, playback_params);//初始化params
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_any:");
		return -3;
	}

	rc=snd_pcm_hw_params_set_access(
	playback_handle, playback_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	if(rc<0)
	{
		perror("\nsed_pcm_hw_set_access:");
		return -4;
	}

	//采样位数
	snd_pcm_hw_params_set_format(
	playback_handle, playback_params, SND_PCM_FORMAT_S16_LE);

	//
	rc=snd_pcm_hw_params_set_channels(
	playback_handle, playback_params, channels);
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_set_channels:");
		return -5;
	}

	rc=snd_pcm_hw_params_set_rate_near(
	playback_handle, playback_params, &frequency, &dir);
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_set_rate_near:");
		return -6;
	}

	rc = snd_pcm_hw_params(playback_handle, playback_params);
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params: ");
		return -7;
	}

	rc=snd_pcm_hw_params_get_period_size(playback_params, &frames, &dir);
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_get_period_size:");
		return -8;
	}
	printf("frames = %x, dir = %x\n", (u32)frames, dir);

	return 1;
}




int sound_take(_sup* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
	return 0;
}
int sound_give(_sup* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
	bufout = buf;
	count = 0;
	return 0;
}
int sound_gone()
{
	return 0;
}
int sound_link()
{
	return 0;
}




int soundlist()
{
	return 0;
}
int soundchoose()
{
	return 0;
}
int sounddelete(struct supply* w)
{
	//printf("@snd_pcm_close\n");
	snd_pcm_close(capture_handle);
	snd_pcm_close(playback_handle);

	alive = 0;
	return 0;
}
int soundcreate(struct supply* w)
{
	unsigned int rate = w->width;
	int ch = w->height;
	u8* buf;

	if(startsound_capture(rate, ch) < 0)return -1;
	if(startsound_playback(rate, ch) < 0)return -2;

	freq = rate;
	channel = ch;
	bufin = buf;

	//thread
	alive = 1;
	thread = startthread(soundlistener, 0);
	return 0;
}




void initmic()
{
}
void freemic()
{
}
