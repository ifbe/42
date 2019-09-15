#include "libuser.h"
#define _src_ hex32('s','r','c',0)
#define qx q[0]
#define qy q[1]
#define qz q[2]
#define qw q[3]




void ahrs_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//writeback: east north sky?
	say("@ahrs_read\n");
}
void ahrs_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
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
int ahrs_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int ahrs_start(struct halfrel* self, struct halfrel* peer)
{
	say("@ahrs_start\n");
	return 0;
}




static void* ahrs_thread(struct arena* win)
{
	vec4 tmp;
	while(1){
		say("@ahrs_thread:%llx\n", win);
		relationread((void*)win, _src_, 0, 0, tmp, 4);
		sleep_us(1000000);
	}
}




int ahrs_delete(struct arena* win)
{
	return 0;
}
int ahrs_create(struct arena* win, void* str)
{
	say("@ahrs_create\n");
	threadcreate(ahrs_thread, win);
	return 0;
}
