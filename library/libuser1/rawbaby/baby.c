#include "libuser.h"




int baby_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	say("@baby_read: crying\n");
	return 0;
}
int baby_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	return 0;
}
int baby_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int baby_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int baby_search(struct actor* act)
{
	return 0;
}
int baby_modify(struct actor* act)
{
	return 0;
}
int baby_delete(struct actor* act)
{
	return 0;
}
int baby_create(struct actor* act)
{
	return 0;
}
