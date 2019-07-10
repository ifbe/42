#include "libuser.h"




void ahrs_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//writeback: east north sky?
	say("@ahrs_read\n");
}
void ahrs_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
/*
	receive gpsdata(already filtered):
	receive mpudata(already filtered):
*/
	say("@ahrs_write\n");
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
	struct relation* rel;
	struct halfrel* self;
	struct halfrel* peer;
	while(1){
		//say("@ahrs_thread:%llx,%llx\n", win, win->irel0, win->orel0);

		rel = win->irel0;
		while(1){
			if(0 == rel)break;
			if(_art_ == rel->srctype){
				self = (void*)&rel->srcchip;
				peer = (void*)&rel->dstchip;
				arteryread(self, peer, tmp, 16);
			}
			rel = samedstnextsrc(rel);
		}
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
