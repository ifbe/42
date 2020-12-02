#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include "libuser.h"




//
static u8* obuf = 0;
static int ocur = 0;
//
static pa_simple* s_out;
static const pa_sample_spec ss =
{
	.format = PA_SAMPLE_S16LE,
	.rate = 44100,
	.channels = 1
};




int speakerlist()
{
	return 0;
}
int speakerchoose()
{
	return 0;
}
int speaker_take(_sup* spk,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	struct pcmdata* pcm;
	if(spk->orel0)take_data_from_peer(spk,_ctx_, stack,sp, 0,0, 0,0);

	pcm = spk->pcmeasy_data;
	if(0 == pcm)return 0;

	int ret,err;
	pa_usec_t latency;

	latency = pa_simple_get_latency(s_out, &err);
	if((pa_usec_t)-1 == latency){
		printf("error@pa_simple_get_latency:%s\n", pa_strerror(err));
		return 0;
	}
	fprintf(stderr, "Out: %0.0f usec\n", (float)latency);

	ret = pa_simple_write(s_out, pcm->buf, pcm->count*2, &err);
	if(ret < 0){
		printf("error@pa_simple_write:%s\n", pa_strerror(err));
		return 0;
	}

	ret = pa_simple_drain(s_out, &err);
	if(ret < 0){
		printf("error@pa_simple_drain:%s\n", pa_strerror(err));
	}

	return 0;
}
int speaker_give(_sup* spk,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	int err;
	pa_usec_t latency;

	if ((latency = pa_simple_get_latency(s_out, &err)) == (pa_usec_t) -1)
	{
		printf("error@pa_simple_get_latency:%s\n", pa_strerror(err));
		goto finish;
	}
	fprintf(stderr, "Out: %0.0f usec\n", (float)latency);

	if (pa_simple_write(s_out, buf, len/2, &err) < 0)
	{
		printf("error@pa_simple_write:%s\n", pa_strerror(err));
		goto finish;
	}

	if(pa_simple_drain(s_out, &err) < 0)
	{
		printf("error@pa_simple_drain:%s\n", pa_strerror(err));
	}
finish:
	return 0;
}
void speakerstop()
{
}
void speakerstart()
{
}
void speakerdelete(struct supply* win)
{
	if(s_out)pa_simple_free(s_out);
}
void speakercreate(struct supply* win)
{
	int error;

	//out
	s_out = pa_simple_new(NULL, "42", PA_STREAM_PLAYBACK, NULL,
		"playback", &ss, NULL, NULL, &error);
	if(!s_out)
	{
		printf("error@pa_simple_new:%s\n", pa_strerror(error));
		return;
	}

	obuf = malloc(0x100000);
	ocur = 0;
}




void initspeaker()
{
}
void freespeaker()
{
}
