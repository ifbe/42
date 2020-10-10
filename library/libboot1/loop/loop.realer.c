#include "libuser.h"
#define _theear_ hex64('t','h','e','e','a','r',0,0)
#define _theeye_ hex64('t','h','e','e','y','e',0,0)
int input(void*, int);




static int alive = 1;
static struct supply* supply = 0;
static struct entity* entity = 0;




void realer(void* realer)
{
	//before
	u64 t0;
	u64 dt;
	struct event* ev;
	//say("@realer!!!!!!!\n");

	//forever
	while(alive)
	{
		//cur time
		t0 = timeread();

		//max fps
		dt = timeread() - t0;
		//say("dt=%d\n", delta);
		if(dt < 16000)sleep_us(16000-dt);
	}
}




void realer_delete(struct item* wrk, u8* arg)
{
}
void realer_create(struct item* wrk, u8* arg, int argc, u8** argv)
{
}




void freerealer()
{
}
void initrealer(void* addr)
{
	supply = addr + 0xc00000;
	entity = addr + 0xe00000;
}
