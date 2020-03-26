#include "libuser.h"
#define _ram_ hex32('r','a','m', 0)




int mmiospace_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* ent = self->pchip;
	return relationread(ent,_ram_, stack,rsp, buf,len);
}
int mmiospace_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	return 0;
}
int mmiospace_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int mmiospace_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@mmiospace_linkup\n");
	return 0;
}




int mmiospace_search(struct entity* world)
{
	return 0;
}
int mmiospace_modify(struct entity* world)
{
	return 0;
}
int mmiospace_delete(struct entity* world)
{
	return 0;
}
int mmiospace_create(struct entity* world, void* str)
{
	return 0;
}
