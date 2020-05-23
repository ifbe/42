#include "libuser.h"




int xamlroot_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void** buf,int len)
{
	return 0;
}
int xamlroot_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void** buf,int len)
{
	return 0;
}
int xamlroot_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int xamlroot_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@xamlroot_linkup\n");
	return 0;
}




int xamlroot_search(struct entity* world)
{
	return 0;
}
int xamlroot_modify(struct entity* world)
{
	return 0;
}
int xamlroot_delete(struct entity* world)
{
	return 0;
}
int xamlroot_create(struct entity* world, void* str)
{
	return 0;
}
