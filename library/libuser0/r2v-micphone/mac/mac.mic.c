#include "libuser.h"




//
static u64 thread = 0;
static int alive = 0;
//
static u8* ibuf = 0;
static int icur = 0;
void* micphonelistener(struct arena* win)
{
	while(1){
		relationwrite(win, _dst_, ibuf, 44100*2);
		sleep_us(1000*1000);
	}
	return 0;
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
int micphonedelete(struct arena* win)
{
	return 0;
}
int micphonecreate(struct arena* win)
{
	ibuf = memorycreate(0x100000, 0);
	icur = 0;

	//thread
	alive = 1;
	thread = threadcreate(micphonelistener, win);
	return 0;
}




void initmicphone()
{
}
void freemicphone()
{
}
