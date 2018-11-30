#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include "libuser.h"




static int alive = 1;
static u64 thread;
//
static u8* ibuf = 0;
static u8* obuf = 0;
static int icur = 0;
static int ocur = 0;
//
static pa_simple* s_in;
static pa_simple* s_out;
static const pa_sample_spec ss =
{
	.format = PA_SAMPLE_S16LE,
	.rate = 44100,
	.channels = 1
};




void* soundlistener(struct arena* win)
{
	int ret,err;
	pa_usec_t latency;
	while(alive)
	{
		if ((latency = pa_simple_get_latency(s_in, &err)) == (pa_usec_t) -1)
		{
			printf("error@pa_simple_get_latency:%s\n", pa_strerror(err));
			goto finish;
		}
 		fprintf(stderr, "In: %0.0f usec\n", (float)latency);

		ret = pa_simple_read(s_in, ibuf+(1024*2*icur), 2048, &err);
		if(err < 0)
		{
			printf("fail@pa_simple_read:%s\n", pa_strerror(err));
			goto finish;
		}
		usleep(1000000*10/441);

		struct relation* orel = win->orel0;
		while(1)
		{
			if(0 == orel)break;
			if(_act_ == orel->dsttype)
			{
				actor_rootwrite(
					(void*)(orel->dstchip), (void*)(orel->dstfoot),
					(void*)(orel->srcchip), (void*)(orel->srcfoot),
					ibuf + (1024*2*icur), 1024*2
				);
			}
			orel = (struct relation*)samesrcnextdst(orel);
		}
		icur = (icur+1)%8;

		//info.enq = (info.enq + 4096) % 0x100000;
		//eventwrite((u64)&info, 's', 0, 0);
		//usleep(10000*1024/441);
	}

finish:
	return 0;
}




int soundlist()
{
	return 0;
}
int soundchoose()
{
	return 0;
}
int soundread(int dev, int time, u8* buf, int len)
{
	return 0;
}
int soundwrite(int dev, int time, u8* buf, int len)
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
void soundstop()
{
}
void soundstart()
{
}
void sounddelete(struct arena* win)
{
	alive = 0;
	if(s_in)pa_simple_free(s_in);
	if(s_out)pa_simple_free(s_out);
}
void soundcreate(struct arena* win)
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

	//in
	s_in = pa_simple_new(NULL, "42", PA_STREAM_RECORD, NULL,
		"record", &ss, NULL, NULL, &error);
	if(!s_in)
	{
		printf("error@pa_simple_new() failed: %s\n", pa_strerror(error));
		return;
	}

	ibuf = malloc(0x100000);
	obuf = malloc(0x100000);
	icur = 0;
	ocur = 0;

	//thread
	alive = 1;
	thread = threadcreate(soundlistener, win);
}




void initmic()
{
}
void freemic()
{
}
