#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include "libuser.h"




//
struct privdata{
	u64 thread;
	int alive;

	u8* ibuf;
	int icur;

	//want
	char path[32];
	int bits;
	int freq;
	int chan;

	//actual
	snd_pcm_t* capture_handle;
	snd_pcm_hw_params_t* capture_params;
	int bytes;	//if bits==24, bytes=4
};
static void copyname(u8* dst, u8* src)
{
	int j;
	for(j=0;j<31;j++){
		if(src[j]<=0x20){break;}
		dst[j] = src[j];
	}
	dst[j]=0;
}




void* micphone_thread(_obj* mic)
{
	int j;
	u64 t0,t1,dt;
	static snd_pcm_uframes_t frames;
	struct privdata* priv = (void*)mic->priv_256b;

	while(priv->alive)
	{
		frames = 1024;
		j = snd_pcm_readi(priv->capture_handle, priv->ibuf+priv->icur, frames);
		if(j != frames)
		{
			printf("snd_pcm_readi:%s\n", snd_strerror(j));
			sleep_us(1000*1000*1024/44100);
			continue;
		}

		t0 = timeread_us();
		say("pcm[%x,%x]\n",priv->icur,priv->icur+1024*priv->bytes);
		printmemory(priv->ibuf+priv->icur, 16);
		give_data_into_peer_temp_stack(mic,_dst_, 0,0, priv->ibuf+priv->icur,1024*priv->bytes);
		priv->icur += 1024*priv->bytes;
		if(priv->icur > 0xc0000)priv->icur = 0;
		t1 = timeread_us();

		dt = 1000*1000*1024/44100 - (t1-t0);
		sleep_us(dt/2);
	}
	return 0;
}
static int getformatfromdepth(int depth)
{
	switch(depth/8){
	case 4:return SND_PCM_FORMAT_S32_LE;
	case 3:return SND_PCM_FORMAT_S24_LE;
	case 1:return SND_PCM_FORMAT_S8;
	}
	return SND_PCM_FORMAT_S16_LE;
}
static int getbytesfrombits(int bits)
{
	switch(bits){
	case 32:
	case 24:return 4;
	case 8:return 1;
	}
	return 2;
}
int micphone_startcapture(_obj* mic)
{
	struct privdata* priv = (void*)mic->priv_256b;
	char* path = priv->path;
	int depth = getformatfromdepth(priv->bits);
	unsigned int rate = priv->freq;
	int chan = priv->chan;

	printf("@snd_pcm_open\n");
	int err = snd_pcm_open(
		&priv->capture_handle,
		path,
		SND_PCM_STREAM_CAPTURE,
		0	//SND_PCM_NONBLOCK
	);
	if (err < 0) {
		printf("snd_pcm_open: %s\n", snd_strerror (err));
		return -1;
	}

	printf("@snd_pcm_hw_params_malloc\n");
	err = snd_pcm_hw_params_malloc (&priv->capture_params);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -2;
	}

	printf("@snd_pcm_hw_params_any\n");
	err = snd_pcm_hw_params_any (priv->capture_handle, priv->capture_params);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -3;
	}

	printf("@snd_pcm_hw_params_set_access\n");
	err = snd_pcm_hw_params_set_access(
		priv->capture_handle, priv->capture_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -4;
	}

	printf("@snd_pcm_hw_params_set_format:%x\n",depth);
	err = snd_pcm_hw_params_set_format(
		priv->capture_handle, priv->capture_params, depth);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -5;
	}
	priv->bytes = getbytesfrombits(priv->bits);

	printf("@snd_pcm_hw_params_set_rate_near:%x\n",rate);
	err = snd_pcm_hw_params_set_rate_near(priv->capture_handle, priv->capture_params, &rate, 0);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -6;
	}

	printf("@snd_pcm_hw_params_set_channels:%x\n",chan);
	err = snd_pcm_hw_params_set_channels(priv->capture_handle, priv->capture_params, chan);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -7;
	}

	printf("@snd_pcm_hw_params\n");
	err = snd_pcm_hw_params (priv->capture_handle, priv->capture_params);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -8;
	}
	snd_pcm_hw_params_free (priv->capture_params);

	printf("@snd_pcm_hw_params\n");
	err = snd_pcm_prepare (priv->capture_handle);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -9;
	}

	//
	printf("sleep 1s\n");
	sleep_us(1000*1000);
	return 1;
}



int micphone_take(_obj* mic,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
	return 0;
}
int micphone_give(_obj* mic,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
	return 0;
}
int micphone_detach()
{
	return 0;
}
int micphone_attach()
{
	return 0;
}




int micphone_reader(_obj* mic,void* foot, void* arg,int idx, void* buf, int len)
{
	return 0;
}
int micphone_writer(_obj* mic,void* foot, void* arg,int idx, void* buf, int len)
{
	return 0;
}
int micphonedelete(_obj* mic)
{
	//printf("@snd_pcm_close\n");
	struct privdata* priv = (void*)mic->priv_256b;
	snd_pcm_close(priv->capture_handle);
	priv->alive = 0;
	return 0;
}
int micphonecreate(_obj* mic, u8* arg, int argc, char** argv)
{
	struct privdata* priv = (void*)mic->priv_256b;

	copyname(priv->path, "plughw:0,0");
	priv->bits = 32;
	priv->freq = 44100;
	priv->chan = 1;
	priv->ibuf = malloc(0x100000);
	priv->icur = 0;

	int j;
	for(j=0;j<argc;j++){
		if(0 == ncmp(argv[j], "path:", 5)){
			copyname(priv->path, argv[j]+5);
		}
	}
	say("path=%s\n",priv->path);

	int ret = micphone_startcapture(mic);
	if(ret < 0)return -1;

	//thread
	priv->alive = 1;
	priv->thread = threadcreate(micphone_thread, mic);
	return 0;
}




void initmic()
{
}
void freemic()
{
}
