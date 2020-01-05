#include "libuser.h"




int baby_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	say("@baby_read: crying\n");
	return 0;
}
int baby_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	say("@baby_write: crying\n");
	return 0;
}
int baby_stop(struct halfrel* self, struct halfrel* peer)
{
	say("@baby_stop: crying\n");
	return 0;
}
int baby_start(struct halfrel* self, struct halfrel* peer)
{
	say("@baby_start: crying\n");
	return 0;
}




int baby_search(struct entity* act)
{
	say("@baby_search: crying\n");
	return 0;
}
int baby_modify(struct entity* act)
{
	say("@baby_modity: crying\n");
	return 0;
}
int baby_delete(struct entity* act)
{
	say("@baby_delete: crying\n");
	return 0;
}
int baby_create(struct entity* act)
{
	say("@baby_create: crying\n");
	return 0;
}




void baby_free()
{
	say("@baby_free: crying\n");
}
void baby_init(void* addr)
{
	say("@baby_init: crying\n");
}
