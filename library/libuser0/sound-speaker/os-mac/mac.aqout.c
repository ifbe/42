#include "libuser.h"
#include "AudioToolbox/AudioToolbox.h"
#define BUFCNT 4
#define SAMPLE_1s 44100
#define SAMPLE_10ms 441




static void speakercallback(void* ptr, AudioQueueRef aqref, AudioQueueBufferRef aqbufref)
{
/*
	OSStatus status;
	PhaseBlah *p = ptr;
	AudioQueueBuffer *buf = buf_ref;
	int nsamp = buf->mAudioDataByteSize / 2;
	short *samp = buf->mAudioData;
	int ii;
	printf ("Callback! nsamp: %d\n", nsamp);
	for (ii = 0; ii < nsamp; ii++) {
		samp[ii] = (int) (30000.0 * sin(ii*freq[p->count]*tau/44100.0));
		//printf("phase: %.3f\n", p->phase);
	}
	p->count = (p->count+1)%12;
	status = AudioQueueEnqueueBuffer(aq, buf_ref, 0, NULL);
	printf ("Enqueue status: %d\n", status);
*/
	_obj* spk = ptr;
	spk->appleaq.aqdeq = (spk->appleaq.aqdeq+1)%BUFCNT;

	say("@aqout.callback: spk=%p,enq=%d,deq=%d, aqref=%p,aqbufref=%p\n",
		spk, spk->appleaq.aqenq, spk->appleaq.aqdeq, aqref, aqbufref);

	if(spk->appleaq.aqenq == spk->appleaq.aqdeq){
		say("aqout hungry\n");
		AudioQueueEnqueueBuffer(aqref, (void*)spk->appleaq.zero10ms, 0, NULL);
		spk->appleaq.aqenq = (spk->appleaq.aqenq+1)%BUFCNT;
	}
}




int speaker_take(_obj* spk,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{/*
	if(spk->orel0)take_data_from_peer(spk,_ctx_, stack,sp, 0,0, 0,0);

	struct pcmdata* pcm = spk->appleaq.data;
	if(0 == pcm)return 0;
	say("fmt=%x,chan=%d,rate=%d,count=%d,buf=%llx\n", pcm->fmt, pcm->chan, pcm->rate, pcm->count, pcm->buf);

	AudioQueueRef aqref = spk->appleaq.aqref;
	AudioQueueBufferRef* aqbufref = (void*)spk->appleaq.aqbufref;

	int j,cnt;
	short* src = pcm->buf;
	short* dst = aqbufref[0]->mAudioData;
	cnt = pcm->count;
	if(cnt > 44100)cnt = 44100;
	for(j=0;j<cnt;j++)dst[j] = src[j];
	aqbufref[0]->mAudioDataByteSize = cnt*2;

	printf("buf: %p, data: %p, len: %d\n", aqbufref[0], aqbufref[0]->mAudioData, aqbufref[0]->mAudioDataByteSize);
	AudioQueueEnqueueBuffer(aqref, aqbufref[0], 0, NULL);

	usleep(1000*1000);*/
	return 0;
}
int speaker_give(_obj* spk,void* foot, _syn* stack,int sp, void* arg, int idx, short* buf, int len)
{
	int j;
	//say("@speakerwrite: len=%x\n", len);

	int enq = spk->appleaq.aqenq;
	int deq = spk->appleaq.aqdeq;
	AudioQueueRef aqref = spk->appleaq.aqref;
	AudioQueueBufferRef aqbufref = spk->appleaq.aqbufref[enq];

	short* dst = aqbufref->mAudioData;
	for(j=0;j<len/2;j++)dst[j] = buf[j];
	aqbufref->mAudioDataByteSize = len;

	j = AudioQueueEnqueueBuffer(aqref, aqbufref, 0, NULL);
	say("@AudioQueueEnqueueBuffer: spk=%p,enq=%d,deq=%d, aqref=%p,aqbufref=%p,ret=%d\n",
		spk,enq,deq, aqref,aqbufref, j);

	spk->appleaq.aqenq = (enq+1)%BUFCNT;
	return 0;
}




int spaker_makerecvpool(_obj* spk, AudioQueueRef aqref)
{
	int j;
	OSStatus status;
	AudioQueueBufferRef* aqbufref = (void*)spk->appleaq.aqbufref;
	for(j=0;j<BUFCNT;j++){
		status = AudioQueueAllocateBuffer(aqref, 2*44100, &aqbufref[j]);
		printf ("[%d]status=%d,buffer=%p\n", j, status, aqbufref[j]);
	}
	return 0;
}
int spaker_makezero10ms(_obj* spk, AudioQueueRef aqref)
{
	OSStatus status = AudioQueueAllocateBuffer(aqref, SAMPLE_10ms*2, (void*)&spk->appleaq.zero10ms);

	AudioQueueBufferRef zero10ms = spk->appleaq.zero10ms;
	say("[x]status=%d,buffer=%p\n", status, zero10ms);

	int j;
	short* dst = zero10ms->mAudioData;
	for(j=0;j<SAMPLE_10ms;j++)dst[j] = 0;
	zero10ms->mAudioDataByteSize = SAMPLE_10ms*2;

	return 0;
}
int speakerdelete(_obj* spk)
{
	return 0;
}
int speakercreate(_obj* spk)
{
	//0.aqref
	AudioStreamBasicDescription fmt = {0};
	fmt.mSampleRate = 44100;
	fmt.mFormatID = kAudioFormatLinearPCM;
	fmt.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
	fmt.mFramesPerPacket = 1;
	fmt.mChannelsPerFrame = 1; // 2 for stereo
	fmt.mBytesPerPacket = fmt.mBytesPerFrame = 2; // x2 for stereo
	fmt.mBitsPerChannel = 16;

	AudioQueueRef aqref;
	OSStatus status = AudioQueueNewOutput(&fmt, speakercallback, spk, 0, 0, 0, &aqref);
	if(status == kAudioFormatUnsupportedDataFormatError){
		puts("oops!");
		return 0;
	}
	printf("NewOutput status: %d\n", status);
	spk->appleaq.aqref = aqref;


	//1.aqbufref
	spaker_makerecvpool(spk, aqref);
	spaker_makezero10ms(spk, aqref);


	//2.start
	status = AudioQueueSetParameter(aqref, kAudioQueueParam_Volume, 1.0);
	printf ("Volume status: %d\n", status);

	status = AudioQueueStart(aqref, NULL);
	printf ("Start status: %d\n", status);


	//3.enq,deq
	spk->appleaq.aqenq = 0;
	spk->appleaq.aqdeq = 0;
	return 0;
}




void initspeaker()
{
}
void freespeaker()
{
}
