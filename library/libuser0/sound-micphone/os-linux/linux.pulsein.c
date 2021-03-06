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
static int icur = 0;
//
static pa_simple* s_in;
static const pa_sample_spec ss =
{
	.format = PA_SAMPLE_S16LE,
	.rate = 44100,
	.channels = 1
};




void* micphonelistener(struct supply* win)
{
	int ret,err;
	pa_usec_t latency;
	struct halfrel stack[0x80];

	while(alive)
	{
		if ((latency = pa_simple_get_latency(s_in, &err)) == (pa_usec_t) -1)
		{
			printf("error@pa_simple_get_latency:%s\n", pa_strerror(err));
			goto finish;
		}
 		//fprintf(stderr, "In: %0.0f usec\n", (float)latency);

		ret = pa_simple_read(s_in, ibuf+(1024*2*icur), 2048, &err);
		if(err < 0)
		{
			printf("fail@pa_simple_read:%s\n", pa_strerror(err));
			goto finish;
		}
		usleep(1000000*10/441);

		give_data_into_peer(win,_dst_, stack,0, 0,0, ibuf+(1024*2*icur),1024*2);
		icur = (icur+1)%8;
		//info.enq = (info.enq + 4096) % 0x100000;
		//eventwrite((u64)&info, 's', 0, 0);
		//usleep(10000*1024/441);
	}

finish:
	return 0;
}




int micphone_take(_sup* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
	return 0;
}
int micphone_give(_sup* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
	return 0;
}
void micphonestop()
{
}
void micphonestart()
{
}
void micphonedelete(struct supply* win)
{
	alive = 0;
	if(s_in)pa_simple_free(s_in);
}
void micphonecreate(struct supply* win)
{
	int error;

	//in
	s_in = pa_simple_new(NULL, "42", PA_STREAM_RECORD, NULL,
		"record", &ss, NULL, NULL, &error);
	if(!s_in)
	{
		printf("error@pa_simple_new() failed: %s\n", pa_strerror(error));
		return;
	}

	ibuf = malloc(0x100000);
	icur = 0;

	//thread
	alive = 1;
	thread = threadcreate(micphonelistener, win);
}




void initmicphone()
{
}
void freemicphone()
{
}
