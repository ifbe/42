#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
u64* eventread();
void eventwrite(u64,u64,u64,u64);
//
u64 startthread(void*, void*);
void stopthread();
//
void printmemory(void*, int);
void say(void*, ...);




struct waveinfo
{
	void* buf;
	int len;
	int enq;
	int deq;
};
static struct waveinfo info;
//
static int alive = 1;
static u64 thread;
//
static int freq;
static int channel;
//
static pa_simple* s_in;
static pa_simple* s_out;
static const pa_sample_spec ss =
{
	.format = PA_SAMPLE_S16LE,
	.rate = 44100,
	.channels = 1
};




void* soundlistener(void* p)
{
	int ret,err;
	while(alive)
	{
		pa_usec_t latency;

		latency = pa_simple_get_latency(s_in, &err);
		if(latency == (pa_usec_t)-1)
		{
			printf("error@pa_simple_get_latency:%s\n", pa_strerror(err));
			goto finish;
		}
		//fprintf(stderr, "In:  %0.0f usec	\r\n", (float)latency);

		pa_simple_read(s_in, (info.buf)+(info.enq), 2048, &err);
		if(err < 0)
		{
			printf("fail@pa_simple_read:%s\n", pa_strerror(err));
			goto finish;
		}

		info.enq = (info.enq + 4096) % 0x100000;
		eventwrite((u64)&info, 's', 0, 0);
		usleep(10000*1024/441);
	}

finish:
	return 0;
}




void listsound()
{
}
void choosesound()
{
}
int readsound(char* buf, int frame)
{
	return 0;
}
int writesound(char* buf, int frame)
{
	int err;
	pa_usec_t latency;

	if ((latency = pa_simple_get_latency(s_out, &err)) == (pa_usec_t) -1)
	{
		printf("error@pa_simple_get_latency:%s\n", pa_strerror(err));
		goto finish;
	}
	//fprintf(stderr, "Out: %0.0f usec	\r\n", (float)latency);


	if (pa_simple_write(s_out, buf, 44100, &err) < 0)
	{
		printf("error@pa_simple_write:%s\n", pa_strerror(err));
		goto finish;
	}

finish:
	return 0;
}
int startsound(unsigned int rate, int ch)
{
	int error;

	//out
	s_out = pa_simple_new(NULL, "42", PA_STREAM_PLAYBACK, NULL,
		"playback", &ss, NULL, NULL, &error);
	if(!s_out)
	{
		printf("error@pa_simple_new:%s\n", pa_strerror(error));
		return 0;
	}

	//in
	s_in = pa_simple_new(NULL, "42", PA_STREAM_RECORD, NULL,
		"record", &ss, NULL, NULL, &error);
	if(!s_in)
	{
		printf("error@pa_simple_new() failed: %s\n", pa_strerror(error));
		return 0;
	}

	//
	freq = rate;
	channel = ch;

	//
	if(info.buf == 0)
	{
		info.enq = 0;
		info.deq = 0;
		info.len = 0x100000;
		info.buf = malloc(info.len);
	}

	//thread
	alive = 1;
	thread = startthread(soundlistener, 0);
	return 1;
}
void stopsound()
{
	int err;

	alive = 0;
	if(pa_simple_drain(s_out, &err) < 0)
	{
		printf("error@pa_simple_drain:%s\n", pa_strerror(err));
	}

	if (s_in)pa_simple_free(s_in);
	if (s_out)pa_simple_free(s_out);
}
void createsound()
{
}
void deletesound()
{
}
