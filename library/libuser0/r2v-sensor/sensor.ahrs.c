#include "libuser.h"
#define _src_ hex32('s','r','c',0)
#define qx q[0]
#define qy q[1]
#define qz q[2]
#define qw q[3]




void ahrs_read(_sup* sup,int foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
	//writeback: east north sky?
	say("@ahrs_read\n");
}
void ahrs_write(_sup* sup,int foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
/*
	receive gpsdata(already filtered):
	receive mpudata(already filtered):
*/
	vec4 e;
	float* q = buf;

	e[0] = arctan2(2*(qw*qx+qy*qz),1-2*(qx*qx+qy*qy))*180/3.141592653;
	e[1] = arcsin(2*qw*qy - 2*qx*qz)*180/3.141592653;
	e[2] = arctan2(2*(qw*qz+qx*qy),1-2*(qy*qy+qz*qz))*180/3.141592653;
	say("@ahrs_write:%f,%f,%f,%f -> %f,%f,%f\n", qx,qy,qz,qw, e[0],e[1],e[2]);
}
int ahrs_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int ahrs_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@ahrs_linkup\n");
	return 0;
}




static void* ahrs_thread(struct supply* win)
{
	vec4 tmp;
	struct halfrel stack[0x80];
	while(1){
		say("@ahrs_thread:%llx\n", win);
		take_data_from_peer(win,_src_, stack,0, 0,0, tmp,4);
		sleep_us(1000000);
	}
}




int ahrs_delete(struct supply* win)
{
	return 0;
}
int ahrs_create(struct supply* win, void* str)
{
	say("@ahrs_create\n");
	threadcreate(ahrs_thread, win);
	return 0;
}
