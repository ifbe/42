#include "libuser.h"
#include <math.h>
#include <aaudio/AAudio.h>


#ifdef __cplusplus
extern "C" {
#endif


struct privdata{
	short* pcm;
	int now;
};


static aaudio_data_callback_result_t dataCallback(AAudioStream *stream, void *userData, void *audioData, int32_t numFrames)
{
	logtoall((void*)"recordDataCallback: stream=%p,userData=%p,audiodata=%p,numFrames=%x\n", stream, userData, audioData, numFrames);

	_obj* spk = userData;
	struct privdata* priv = (void*)spk->priv_256b;

	short* pcm = priv->pcm;
	short* out = audioData;

	int j;
	for(j=0;j<numFrames;j++){
		out[j] = pcm[priv->now+j];
	}
	priv->now = (priv->now+numFrames)%48000;

	return AAUDIO_CALLBACK_RESULT_CONTINUE;
}
static void errorCallback(AAudioStream *stream, void *userData, aaudio_result_t error)
{
	logtoall((void*)"recordErrorCallback: stream=%p,userData=%p,error=%s\n", stream, userData, AAudio_convertResultToText(error));
}



void speaker_take(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf, int len)
{
}
void speaker_give(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf, int len)
{
}
void speaker_detach()
{
}
void speaker_attach()
{
}


void speaker_reader(_obj* sup,void* foot, p64 arg,int idx, void* buf, int len)
{
}
void speaker_writer(_obj* sup,void* foot, p64 arg,int idx, void* buf, int len)
{
}
void speaker_delete(_obj* spk, void* arg, int argc, u8** argv)
{
	//res = AAudioStream_requestStop(stream);
	//res = AAudioStream_requestPause(stream);
	//res = AAudioStream_requestFlush(stream);

	//AAudioStream_close(stream);
}
void speaker_create(_obj* spk, void* arg, int argc, u8** argv)
{
	logtoall((void*)"speakercreate\n");

	AAudioStreamBuilder *builder;
	aaudio_result_t res = AAudio_createStreamBuilder(&builder);
	if(AAUDIO_OK != res){
		logtoall((void*)"AAudio_createStreamBuilder: error=%s\n", AAudio_convertResultToText(res));
		return;
	}

	//AAudioStreamBuilder_setDeviceId(builder, AAUDIO_UNSPECIFIED);
	AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_OUTPUT);  // 录音，AAUDIO_DIRECTION_OUTPUT表示放音
	AAudioStreamBuilder_setSampleRate(builder, 48000);
	AAudioStreamBuilder_setChannelCount(builder, 1);  // 可设置单声道(Mono)或立体声(Stereo)，参数分别为1和2
	AAudioStreamBuilder_setDataCallback(builder, dataCallback, spk);
	AAudioStreamBuilder_setErrorCallback(builder, errorCallback, spk);
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
	int j;
	for(j=0;j<48000*2;j++){
		pcm[j] = 32767*sin(j*tau*440/48000);
	}

	struct privdata* priv = (void*)spk->priv_256b;
	priv->pcm = pcm;
	priv->now = 0;

	res = AAudioStream_requestStart(stream);
}




void initspeaker()
{
}
void freespeaker()
{
}


#ifdef __cplusplus
}
#endif