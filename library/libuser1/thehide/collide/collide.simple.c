#include "libuser.h"




int collide_search()
{
	return 0;
}
int collide_modify()
{
	return 0;
}
int collide_delete(struct supply* win)
{
	return 0;
}
int collide_create(struct supply* win, void* str)
{
	say("@collide_create\n");
	return 0;
}




int collide_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@collide_read\n");
	return 0;
}
int collide_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@collide_write\n");
	return 0;
}
int collide_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int collide_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
