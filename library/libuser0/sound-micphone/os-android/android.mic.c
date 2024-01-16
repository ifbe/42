#include "libuser.h"
#include <aaudio/AAudio.h>


#ifdef __cplusplus
extern "C" {
#endif


struct privdata{
	short* pcm;
	int now;
	int todo;
};


static aaudio_data_callback_result_t dataCallback(AAudioStream *stream, void *userData, void *audioData, int32_t numFrames)
{
	short* in = audioData;
	//logtoall((void*)"recordDataCallback: stream=%p,userData=%p,audiodata=%p,numFrames=%x: %d,%d,%d,%d\n", stream, userData, audioData, numFrames, in[0], in[1], in[2], in[3]);

	_obj* mic = userData;
	struct privdata* priv = (void*)mic->priv_256b;
	short* pcm = priv->pcm;

	int j;
	int k = priv->now;
	for(j=0;j<numFrames;j++){
		pcm[k] = in[j];
		k = (k+1)%48000;
	}
	priv->now = k;

	if(priv->now < 1024){
		for(j=0;j<1024;j++)pcm[48000+j] = pcm[j];
	}

	int todo_done= (priv->todo+48000-1024)%48000;
	//int todo = priv->todo;
	int todo_end = (priv->todo+1024) % 48000;
	if(priv->todo < todo_end){		//normal case
		if( (priv->now >= priv->todo) && (priv->now <= todo_end) ){
			goto found;
		}
	}
	else{		//wrap around
		if( (priv->now >= priv->todo) || (priv->now <= todo_end) ){
			goto found;
		}
	}
	goto byebye;

found:
	logtoall((void*)"now=%d,done=[%d,%d)\n", priv->now, todo_done, priv->todo);
	give_data_into_peer_temp_stack(mic,_dst_, 0,0, &pcm[todo_done],1024*2);
	priv->todo = todo_end;

byebye:
	return AAUDIO_CALLBACK_RESULT_CONTINUE;
}
static void errorCallback(AAudioStream *stream, void *userData, aaudio_result_t error)
{
	logtoall((void*)"recordErrorCallback: stream=%p,userData=%p,error=%s\n", stream, userData, AAudio_convertResultToText(error));
}



void micphone_take(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf, int len)
{
}
void micphone_give(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf, int len)
{
}
void micphone_detach()
{
}
void micphone_attach()
{
}


void micphone_reader(_obj* sup,void* foot, p64 arg,int idx, void* buf, int len)
{
}
void micphone_writer(_obj* sup,void* foot, p64 arg,int idx, void* buf, int len)
{
}
void micphone_delete(_obj* mic, void* arg, int argc, u8** argv)
{
	//res = AAudioStream_requestStop(stream);
	//res = AAudioStream_requestPause(stream);
	//res = AAudioStream_requestFlush(stream);

	//AAudioStream_close(stream);
}
void micphone_create(_obj* mic, void* arg, int argc, u8** argv)
{
	logtoall((void*)"micphonecreate\n");

	AAudioStreamBuilder *builder;
	aaudio_result_t res = AAudio_createStreamBuilder(&builder);
	if(AAUDIO_OK != res){
		logtoall((void*)"AAudio_createStreamBuilder: error=%s\n", AAudio_convertResultToText(res));
		return;
	}

	//AAudioStreamBuilder_setDeviceId(builder, AAUDIO_UNSPECIFIED);
	AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_INPUT);  // 录音，AAUDIO_DIRECTION_OUTPUT表示放音
	AAudioStreamBuilder_setSampleRate(builder, 48000);
	AAudioStreamBuilder_setChannelCount(builder, 1);  // 可设置单声道(Mono)或立体声(Stereo)，参数分别为1和2
	AAudioStreamBuilder_setDataCallback(builder, dataCallback, mic);
	AAudioStreamBuilder_setErrorCallback(builder, errorCallback, mic);
	AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_I16);
	AAudioStreamBuilder_setSharingMode(builder, AAUDIO_SHARING_MODE_SHARED);	//AAUDIO_SHARING_MODE_EXCLUSIVE
	AAudioStreamBuilder_setPerformanceMode(builder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);

	AAudioStream* stream;
	res = AAudioStreamBuilder_openStream(builder, &stream);
	if(AAUDIO_OK != res){
		logtoall((void*)"AAudioStreamBuilder_openStream: error=%s, internal error usually mean permission denied\n", AAudio_convertResultToText(res));
		return;
	}

	AAudioStreamBuilder_delete(builder);

	short* pcm = memoryalloc(0x100000, 0);

	struct privdata* priv = (void*)mic->priv_256b;
	priv->pcm = pcm;
	priv->now = 0;
	priv->todo = 1024;

	res = AAudioStream_requestStart(stream);
}


#ifdef __cplusplus
}
#endif