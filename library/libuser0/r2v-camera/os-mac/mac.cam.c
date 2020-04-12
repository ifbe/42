#include "libuser.h"




//
static u64 thread = 0;
static int alive = 0;
//
static u8* ibuf = 0;
static int icur = 0;
void* visionlistener(struct supply* win)
{
	int x,y;
	struct halfrel stack[0x80];

	while(1){
		for(y=0;y<480;y++){
			for(x=0;x<640;x++){
				if(x >= 480){
					ibuf[2*(640*y+x)+0] = getrandom()&0xff;
					ibuf[2*(640*y+x)+1] = getrandom()&0xff;
				}
				else{
					if(y>x){
						ibuf[2*(640*y+x)+0] = 0;
						ibuf[2*(640*y+x)+1] = getrandom()&0xff;
					}
					else{
						ibuf[2*(640*y+x)+0] = getrandom()&0xff;
						ibuf[2*(640*y+x)+1] = 0;
					}
				}
			}
		}
		relationwrite(win,_dst_, stack,0, 0,0, ibuf,640*480*2);
		sleep_us(16*1000);
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
int videoread(_sup* sup,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int videowrite(_sup* sup,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
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
int videodelete(struct supply* win)
{
	return 0;
}
int videocreate(struct supply* win)
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
