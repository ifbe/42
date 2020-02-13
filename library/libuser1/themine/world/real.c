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
int reality_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int reality_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@reality_linkup\n");
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
