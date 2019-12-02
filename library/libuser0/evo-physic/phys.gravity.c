#include "libuser.h"




int gravity_search()
{
	return 0;
}
int gravity_modify()
{
	return 0;
}
int gravity_delete(struct supply* win)
{
	return 0;
}
int gravity_create(struct supply* win, void* str)
{
	say("@gravity_create\n");
	return 0;
}




int gravity_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@gravity_read\n");
	return 0;
}
int gravity_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@gravity_write\n");
	return 0;
}
int gravity_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gravity_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}