#include "libuser.h"




//
static u64 thread = 0;
static int alive = 0;
//
static u8* ibuf = 0;
static int icur = 0;
void* visionlistener(struct arena* win)
{
	while(1){
		relationwrite(win, _dst_, 0, 0, ibuf, 640*480*2);
		sleep_us(1000*1000);
	}
	return 0;
}




int videolist()
{
	return 0;
}
int videochoose()
{
	return 0;
}
void* videoread(char* buf, int frame)
{
	return 0;
}
int videowrite(char* buf, int frame)
{
	return 0;
}
int videostop()
{
	return 0;
}
int videostart()
{
	return 0;
}
int videodelete(struct arena* win)
{
	return 0;
}
int videocreate(struct arena* win)
{
	ibuf = memorycreate(0x100000, 0);
	for(icur=0;icur<640*48082;icur++)ibuf[icur] = 0x66;
	icur = 0;

	//thread
	alive = 1;
	thread = threadcreate(visionlistener, win);
	return 0;
}




void initcam()
{
}
void freecam()
{
}
