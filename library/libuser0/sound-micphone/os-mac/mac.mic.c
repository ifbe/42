#include "libuser.h"




//
static u64 thread = 0;
static int alive = 0;
//
static short* ibuf = 0;
static int icur = 0;
void* micphonelistener(_obj* win)
{
	int j;
	struct halfrel stack[0x80];
	while(1){
		for(j=0;j<1024;j++)ibuf[j] = (random_read()&0xfff) - 0x800;
		give_data_into_peer(win,_dst_, stack,0, 0,0, ibuf,1024*2);
		sleep_us(1000*1000);
	}
	return 0;
}




int micphone_take(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf, int len)
{
	return 0;
}
int micphone_give(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf, int len)
{
	return 0;
}
int micphone_attach()
{
	return 0;
}
int micphone_detach()
{
	return 0;
}




int micphone_reader(_obj* sup,void* foot, p64 arg,int idx, void* buf, int len)
{
	return 0;
}
int micphone_writer(_obj* sup,void* foot, p64 arg,int idx, void* buf, int len)
{
	return 0;
}
int micphone_delete(_obj* win)
{
	return 0;
}
int micphone_create(_obj* win)
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
