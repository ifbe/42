#include "libuser.h"




int reality_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	say("@reality_read\n");
	return 0;
}
int reality_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	return 0;
}
int reality_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int reality_start(struct halfrel* self, struct halfrel* peer)
{
	say("@reality_start\n");
	return 0;
}




int reality_search(struct entity* world)
{
	return 0;
}
int reality_modify(struct entity* world)
{
	return 0;
}
int reality_delete(struct entity* world)
{
	return 0;
}
int reality_create(struct entity* world, void* str)
{
	return 0;
}
