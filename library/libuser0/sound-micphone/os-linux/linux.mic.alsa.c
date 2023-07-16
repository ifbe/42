#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include "libuser.h"
u64 timeread_ns();




//
struct privdata{
	u64 thread;
	int alive;
	int log;

	u8* ibuf;
	int icur;

	//want
	char path[32];
	int bits;
	int freq;
	int chan;

	//actual
	snd_pcm_t* capture_handle;
	snd_pcm_hw_params_t* capture_hwparams;
	snd_pcm_sw_params_t* capture_swparams;
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



u64 gst_alsasrc_get_timestamp(_obj* mic, snd_pcm_status_t* status)
{
	struct privdata* priv = (void*)mic->priv_256b;

	int ret = snd_pcm_status (priv->capture_handle, status);
	if(ret != 0) {
		say("snd_pcm_status failed");
		return 0;
	}

	//in case an xrun condition has occured we need to handle this
	ret = snd_pcm_status_get_state (status);
	if (ret != SND_PCM_STATE_RUNNING) {
		say("snd_pcm_status_get_state:ret=%d\n",ret);
/*		int err = -EPIPE;
		ret = xrun_recovery (asrc, priv->capture_handle, err);
		if(ret < 0) {
			GST_WARNING_OBJECT (asrc, "Could not recover from xrun condition !");
		}
		//reload the status alsa status object, since recovery made it invalid
		ret = snd_pcm_status (priv->capture_handle, status);
		if(ret != 0) {
			say("snd_pcm_status failed");
		}*/
	}

	//struct timespec ht;
	//snd_pcm_status_get_tstamp (status, &ht);
	snd_htimestamp_t ht;
	snd_pcm_status_get_htstamp (status, &ht);

	u64 timestamp = ht.tv_sec * 1000*1000*1000 + ht.tv_nsec;

	/* max available frames sets the depth of the buffer */
	snd_pcm_uframes_t avail = snd_pcm_status_get_avail (status);

	/* calculate the timestamp of the next sample to be read */
	u64 delta = (u64)avail * 1000*1000*1000 / priv->freq;

	/* compensate for the fact that we really need the timestamp of the previously read data segment */
	//timestamp -= asrc->period_time * 1000;

	u64 ts = timestamp-delta;
	if(priv->log)say("ts=%lld, htstamp=%lld, now=%lld, avail=%lld\n", ts, timestamp, timeread_ns(), avail);

	return ts;
}
void* micphone_thread(_obj* mic)
{
	struct kv88 kv[2] = {
		{'t', 0},
		{0,0}
	};
	int ret;
	int dt,dt01,dt12;
	u64 ts,t0,t1,t2,tn;
	struct privdata* priv = (void*)mic->priv_256b;

	snd_pcm_status_t* status;
	ret = snd_pcm_status_malloc (&status);
	if(ret != 0) {
		say("snd_pcm_status_malloc failed");
		return 0;
	}

	while(priv->alive)
	{
		t0 = timeread_us();

		ts = gst_alsasrc_get_timestamp(mic, status);	//this value is wrong, dont know why
		ts = gst_alsasrc_get_timestamp(mic, status);

		static snd_pcm_uframes_t frames = 1024;
		ret = snd_pcm_readi(priv->capture_handle, priv->ibuf+priv->icur, frames);
		if(ret != frames)
		{
			printf("snd_pcm_readi:%s\n", snd_strerror(ret));
			sleep_us(1000*1000*1024/44100);
			continue;
		}

		t1 = timeread_us();

		if(priv->log)say("pcm[%x,%x]\n",priv->icur,priv->icur+1024*priv->bytes);
		if(priv->log)printmemory(priv->ibuf+priv->icur, 16);
		kv[0].val = ts;
		give_data_into_peer_temp_stack(mic,_dst_, (p64)kv,_kv88_, priv->ibuf+priv->icur,1024*priv->bytes);
		priv->icur += 1024*priv->bytes;
		if(priv->icur > 0xc0000)priv->icur = 0;

		t2 = timeread_us();

		dt01 = t1-t0;
		dt12 = t2-t1;
		if(priv->log)printf("t0=%lld,t1=%lld,t2=%lld,dt01=%d,dt12=%d\n",t0,t1,t2,dt01,dt12);

		tn = timeread_us();
		dt = 1000*1000*1024/44100 - (tn-t1);
		if(dt>0)sleep_us(dt/4);	//after this: ring buffer have at least 1024/4=250 samples
	}

	snd_pcm_status_free (status);

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
	err = snd_pcm_hw_params_malloc (&priv->capture_hwparams);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -2;
	}

	printf("@snd_pcm_hw_params_any\n");
	err = snd_pcm_hw_params_any (priv->capture_handle, priv->capture_hwparams);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -3;
	}

	printf("@snd_pcm_hw_params_set_access\n");
	err = snd_pcm_hw_params_set_access(
		priv->capture_handle, priv->capture_hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -4;
	}

	printf("@snd_pcm_hw_params_set_format:%x\n",depth);
	err = snd_pcm_hw_params_set_format(
		priv->capture_handle, priv->capture_hwparams, depth);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -5;
	}
	priv->bytes = getbytesfrombits(priv->bits);

	printf("@snd_pcm_hw_params_set_rate_near:%x\n",rate);
	err = snd_pcm_hw_params_set_rate_near(priv->capture_handle, priv->capture_hwparams, &rate, 0);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -6;
	}

	printf("@snd_pcm_hw_params_set_channels:%x\n",chan);
	err = snd_pcm_hw_params_set_channels(priv->capture_handle, priv->capture_hwparams, chan);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -7;
	}

	printf("@snd_pcm_hw_params\n");
	err = snd_pcm_hw_params (priv->capture_handle, priv->capture_hwparams);
	if (err < 0) {
		printf ("%s\n", snd_strerror (err));
		return -8;
	}
	snd_pcm_hw_params_free (priv->capture_hwparams);


	snd_pcm_sw_params_alloca(&priv->capture_swparams);

	/* get the current swparams */
	printf("@snd_pcm_sw_params_current\n");
	err = snd_pcm_sw_params_current(priv->capture_handle, priv->capture_swparams);
	if (err < 0) {
		printf("Unable to determine current swparams : %s\n", snd_strerror(err));
		return err;
	}

	printf("@snd_pcm_sw_params_set_tstamp_mode\n");
	err = snd_pcm_sw_params_set_tstamp_mode(priv->capture_handle, priv->capture_swparams, SND_PCM_TSTAMP_ENABLE);
	if (err < 0) {
		printf("alsa: Unable to enable timestamps: %s\n", snd_strerror(err));
		return err;
	}

	/* write the parameters to the playback device */
	printf("@snd_pcm_sw_params\n");
	err = snd_pcm_sw_params(priv->capture_handle, priv->capture_swparams);
	if (err < 0) {
		printf("Unable to set sw params : %s\n", snd_strerror(err));
		return err;
	}

	//dont free!
	//snd_pcm_sw_params_free(priv->capture_swparams);


	printf("@snd_pcm_prepare\n");
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



int micphone_take(_obj* mic,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf, int len)
{
	return 0;
}
int micphone_give(_obj* mic,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf, int len)
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




int micphone_reader(_obj* mic,void* foot, p64 arg,int idx, void* buf, int len)
{
	return 0;
}
int micphone_writer(_obj* mic,void* foot, p64 arg,int idx, void* buf, int len)
{
	return 0;
}
int micphone_delete(_obj* mic)
{
	printf("@snd_pcm_close\n");
	struct privdata* priv = (void*)mic->priv_256b;
	priv->alive = 0;
	usleep(1000*1000);
	snd_pcm_drain(priv->capture_handle);
	snd_pcm_close(priv->capture_handle);
	return 0;
}
int micphone_create(_obj* mic, u8* arg, int argc, char** argv)
{
	struct privdata* priv = (void*)mic->priv_256b;
	priv->log = 0;

	copyname(priv->path, "default");
	priv->bits = 16;
	priv->freq = 44100;
	priv->chan = 1;
	priv->ibuf = malloc(0x100000);
	priv->icur = 0;

	int j;
	for(j=0;j<argc;j++){
		if(0 == ncmp(argv[j], "log:", 4)){
			priv->log = 1;
		}
		if(0 == ncmp(argv[j], "bits:", 5)){
			decstr2u32(argv[j]+5, &priv->bits);
		}
		else if(0 == ncmp(argv[j], "path:", 5)){
			copyname(priv->path, argv[j]+5);
		}
	}
	say("path=%s\n",priv->path);
	say("bits=%d\n",priv->bits);

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
