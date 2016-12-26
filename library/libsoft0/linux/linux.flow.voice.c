#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(void*, int);
void say(void*, ...);




//
static int freq;
static int channel;
//
static snd_pcm_t* capture_handle;
static snd_pcm_hw_params_t* capture_params;
//
static snd_pcm_t* playback_handle;
static snd_pcm_hw_params_t* playback_params;
static snd_pcm_uframes_t frames;
static int dir=0;




int readsound(char* buf, int frame)
{
	int j;

	j = snd_pcm_readi(capture_handle, buf, frame);
	if (j != frame)
	{
		printf ("%s\n", snd_strerror(j));
		return j;
	}

	//single channel -> dual channel
	if(channel == 1)
	{
		for(j=frame-1;j>0;j--)
		{
			buf[4*j + 3] = buf[2*j+1];
			buf[4*j + 2] = buf[2*j+0];
		}
	}

	printmemory(buf, 0x100);
	return frame;
}
int writesound(char* buf, int frame)
{
	return snd_pcm_writei(playback_handle, buf, frames);
}




int startsound_capture(unsigned int rate, int ch)
{
	int err;

	//printf("@snd_pcm_open\n");
	err = snd_pcm_open (&capture_handle, "hw:1,0", SND_PCM_STREAM_CAPTURE, 0);
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

        rc=snd_pcm_open(&playback_handle, "hw:1,0", SND_PCM_STREAM_PLAYBACK, 0);
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
int startsound(unsigned int rate, int ch)
{
	if(startsound_capture(rate, ch) < 0)return -1;
	if(startsound_playback(rate, ch) < 0)return -2;

	freq = rate;
	channel = ch;
	return 1;
}
void stopsound()
{
	//printf("@snd_pcm_close\n");
	snd_pcm_close(capture_handle);
	snd_pcm_close(playback_handle);
}
void createsound()
{
}
void deletesound()
{
}
