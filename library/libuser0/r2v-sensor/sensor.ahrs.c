#include "libuser.h"
#define _src_ hex32('s','r','c',0)




void ahrs_read(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	//writeback: east north sky?
	say("@ahrs_read\n");
}
void ahrs_write(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
/*
	receive gpsdata(already filtered):
	receive mpudata(already filtered):
*/
	float* q = buf;
	say("@ahrs_write:%f,%f,%f,%f\n", q[0], q[1], q[2], q[3]);
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
		relationread((void*)win, _src_, tmp, 4);
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
