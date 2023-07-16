#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <libuser.h>




struct privdata{
	u64 thread;
	int alive;

	char* hwname;
	int freq;
	int chan;

	snd_pcm_t* playback_handle;
	snd_pcm_hw_params_t* playback_params;
	snd_pcm_uframes_t period;
	int dir;

	//u8* bufout = 0;
};




int speaker_startplayback(_obj* spk)
{
	struct privdata* priv = (void*)spk->priv_256b;
	char* hwname = priv->hwname;
	unsigned frequency = priv->freq;
	int channels = priv->chan;

	int rc = snd_pcm_open(&priv->playback_handle, hwname, SND_PCM_STREAM_PLAYBACK, 0);
	if(rc<0)
	{
		perror("\nopen PCM device failed:");
		return -1;
	}

	snd_pcm_hw_params_alloca(&priv->playback_params); //分配params结构体
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_alloca:");
		return -2;
	}

	rc=snd_pcm_hw_params_any(
	priv->playback_handle, priv->playback_params);//初始化params
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_any:");
		return -3;
	}

	rc=snd_pcm_hw_params_set_access(
	priv->playback_handle, priv->playback_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	if(rc<0)
	{
		perror("\nsed_pcm_hw_set_access:");
		return -4;
	}

	//采样位数
	snd_pcm_hw_params_set_format(
	priv->playback_handle, priv->playback_params, SND_PCM_FORMAT_S16_LE);

	//
	rc=snd_pcm_hw_params_set_channels(
	priv->playback_handle, priv->playback_params, channels);
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_set_channels:");
		return -5;
	}

	rc=snd_pcm_hw_params_set_rate_near(
	priv->playback_handle, priv->playback_params, &frequency, &priv->dir);
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_set_rate_near:");
		return -6;
	}

	rc = snd_pcm_hw_params(priv->playback_handle, priv->playback_params);
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params: ");
		return -7;
	}

	rc=snd_pcm_hw_params_get_period_size(priv->playback_params, &priv->period, &priv->dir);
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_get_period_size:");
		return -8;
	}
	printf("period = %x, dir = %x\n", (u32)priv->period, priv->dir);

	return 1;
}




int speaker_take(_obj* spk,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf, int len)
{
	return 0;
}
int speaker_give(_obj* spk,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf, int len)
{
	struct privdata* priv = (void*)spk->priv_256b;

	int frames = len/2;
	int j = snd_pcm_writei(priv->playback_handle, buf, frames);
	//printf("snd_pcm_writei:%s\n", snd_strerror(j));
	if(j == frames)return 0;

	if(j == -EPIPE)snd_pcm_prepare(priv->playback_handle);

	return 0;
}
int speaker_detach()
{
	return 0;
}
int speaker_attach()
{
	return 0;
}




int speaker_read()
{
	return 0;
}
int speaker_write()
{
	return 0;
}
int speaker_delete(_obj* spk)
{
	//printf("@snd_pcm_close\n");
	struct privdata* priv = (void*)spk->priv_256b;
	snd_pcm_close(priv->playback_handle);

	priv->alive = 0;
	return 0;
}
int speaker_create(_obj* spk)
{
	struct privdata* priv = (void*)spk->priv_256b;
	priv->hwname = "default";
	priv->freq = 44100;
	priv->chan = 1;

	int ret = speaker_startplayback(spk);
	if(ret < 0)return -2;

	return 0;
}
