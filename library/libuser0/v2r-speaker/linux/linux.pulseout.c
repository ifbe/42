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
int speakerread(int dev, int time, u8* buf, int len)
{
	return 0;
}
int speakerwrite(int dev, int time, u8* buf, int len)
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
void speakerdelete(struct arena* win)
{
	if(s_out)pa_simple_free(s_out);
}
void speakercreate(struct arena* win)
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
