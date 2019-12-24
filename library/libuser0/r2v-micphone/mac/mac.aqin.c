#include "libuser.h"
#include "AudioToolbox/AudioToolbox.h"




static void micphonecallback(void* ptr, AudioQueueRef inaq, AudioQueueBufferRef inctx, const AudioTimeStamp *inStartTime, UInt32 inNumberPacketDescriptions, const AudioStreamPacketDescription *inPacketDescs)
{
	say("@micphonecallback: ptr=%llx, aqref=%llx, aqctx=%llx, time=%llx, num=%x, desc=%llx\n",
	ptr, inaq, inctx, inStartTime, inNumberPacketDescriptions, inPacketDescs);

	void* buf = inctx->mAudioData;
	int len = inctx->mAudioDataByteSize;
	say("%llx,%x,%x\n", buf, len, inNumberPacketDescriptions);
	relationwrite(ptr, _dst_, 0, 0, buf, len);

	AudioQueueEnqueueBuffer(inaq, inctx, 0, NULL);
}




int micphonelist()
{
	return 0;
}
int micphonechoose()
{
	return 0;
}
void* micphoneread(char* buf, int frame)
{
	return 0;
}
int micphonewrite(char* buf, int frame)
{
	return 0;
}
int micphonestop()
{
	return 0;
}
int micphonestart()
{
	return 0;
}
int micphonedelete(struct supply* win)
{
	return 0;
}
int micphonecreate(struct supply* win)
{
	AudioStreamBasicDescription fmt = {0};
	fmt.mSampleRate = 44100;
	fmt.mFormatID = kAudioFormatLinearPCM;
	fmt.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
	fmt.mFramesPerPacket = 1;
	fmt.mChannelsPerFrame = 1; // 2 for stereo
	fmt.mBytesPerPacket = fmt.mBytesPerFrame = 2; // x2 for stereo
	fmt.mBitsPerChannel = 16;

	AudioQueueRef aq;
	OSStatus status = AudioQueueNewInput(&fmt, micphonecallback, win, 0, 0, 0, &aq);
	if(status == kAudioFormatUnsupportedDataFormatError){
		puts("oops!");
		return 0;
	}
	else{
		printf("NewInput status: %d\n", status);
		win->aqref = aq;
	}

	int j;
	AudioQueueBufferRef* buf_ref = win->aqctx = malloc(0x1000);
	for(j=0;j<16;j++){
		status = AudioQueueAllocateBuffer(aq, 2*1024, &buf_ref[j]);
		printf("[%d]Allocate status: %d\n", j, status);

		status = AudioQueueEnqueueBuffer(aq, buf_ref[j], 0, NULL);
		printf("[%d]Enqueue status: %d\n", j, status);
	}

	win->aqref = aq;
	status = AudioQueueSetParameter(aq, kAudioQueueParam_Volume, 1.0);
	printf ("Volume status: %d\n", status);

	status = AudioQueueStart(aq, NULL);
	printf ("Start status: %d\n", status);

	win->aqctx = buf_ref;
	return 0;
}




void initmicphone()
{
}
void freemicphone()
{
}
