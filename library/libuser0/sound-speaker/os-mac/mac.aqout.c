#include "libuser.h"
#include "AudioToolbox/AudioToolbox.h"

/*
typedef struct {
	int count;
}PhaseBlah;
static PhaseBlah phase = {0};
static float freq[12] = {440, 466, 494, 523, 554, 587, 622, 659, 698.5, 740, 784, 830.6};
*/

static void speakercallback(void* ptr, AudioQueueRef aq, AudioQueueBufferRef buf_ref)
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
	_obj* sup = ptr;
	say("@audioqueue.callback: %llx, %d, %d\n", sup, sup->pcmeasy.aqenq, sup->pcmeasy.aqdeq);
}




int speaker_take(_obj* spk,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	if(spk->orel0)take_data_from_peer(spk,_ctx_, stack,sp, 0,0, 0,0);

	struct pcmdata* pcm = spk->pcmeasy.data;
	if(0 == pcm)return 0;
	say("fmt=%x,chan=%d,rate=%d,count=%d,buf=%llx\n", pcm->fmt, pcm->chan, pcm->rate, pcm->count, pcm->buf);

	AudioQueueRef aqref = spk->pcmeasy.aqref;
	AudioQueueBuffer** aqctx = spk->pcmeasy.aqctx;

	int j,cnt;
	short* src = pcm->buf;
	short* dst = aqctx[0]->mAudioData;
	cnt = pcm->count;
	if(cnt > 44100)cnt = 44100;
	for(j=0;j<cnt;j++)dst[j] = src[j];
	aqctx[0]->mAudioDataByteSize = cnt*2;

	printf("buf: %p, data: %p, len: %d\n", aqctx[0], aqctx[0]->mAudioData, aqctx[0]->mAudioDataByteSize);
	AudioQueueEnqueueBuffer(aqref, aqctx[0], 0, NULL);

	usleep(1000*1000);
	return 0;
}
int speaker_give(_obj* spk,void* foot, _syn* stack,int sp, void* arg, int idx, short* buf, int len)
{
	int j;
	say("@speakerwrite: len=%x\n", len);

	AudioQueueRef aqref = spk->pcmeasy.aqref;
	AudioQueueBuffer** aqctx = spk->pcmeasy.aqctx;
	int enq = spk->pcmeasy.aqenq;

	short* dst = aqctx[enq]->mAudioData;
	for(j=0;j<len/2;j++)dst[j] = buf[j];
	aqctx[enq]->mAudioDataByteSize = len;

	j = AudioQueueEnqueueBuffer(aqref, aqctx[enq], 0, NULL);
	//say("j=%d\n", j);

	spk->pcmeasy.aqenq = (enq+1)%2;
	return 0;
}
int speakerstop()
{
	return 0;
}
int speakerstart()
{
	return 0;
}




int speakerlist()
{
	return 0;
}
int speakerchoose()
{
	return 0;
}
int speakerdelete(_obj* win)
{
	return 0;
}
int speakercreate(_obj* win)
{
	AudioStreamBasicDescription fmt = {0};
	fmt.mSampleRate = 44100;
	fmt.mFormatID = kAudioFormatLinearPCM;
	fmt.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
	fmt.mFramesPerPacket = 1;
	fmt.mChannelsPerFrame = 1; // 2 for stereo
	fmt.mBytesPerPacket = fmt.mBytesPerFrame = 2; // x2 for stereo
	fmt.mBitsPerChannel = 16;

	//0.aqref
	AudioQueueRef aq;
	OSStatus status = AudioQueueNewOutput(&fmt, speakercallback, win, 0, 0, 0, &aq);
	if(status == kAudioFormatUnsupportedDataFormatError){
		puts("oops!");
		return 0;
	}
	else{
		printf("NewOutput status: %d\n", status);
		win->pcmeasy.aqref = aq;
	}

	//1.aqctx
	int j;
	AudioQueueBufferRef* buf_ref = win->pcmeasy.aqctx = malloc(0x1000);
	for(j=0;j<2;j++){
		status = AudioQueueAllocateBuffer(win->pcmeasy.aqref, 2*44100, &buf_ref[j]);
		printf ("[%d]Allocate status: %d\n", j, status);
	}

	status = AudioQueueSetParameter(win->pcmeasy.aqref, kAudioQueueParam_Volume, 0.5);
	printf ("Volume status: %d\n", status);

	status = AudioQueueStart(win->pcmeasy.aqref, NULL);
	printf ("Start status: %d\n", status);

	//2.aqidx
	win->pcmeasy.aqenq = 0;
	win->pcmeasy.aqdeq = 0;
	return 0;
}




void initspeaker()
{
}
void freespeaker()
{
}
