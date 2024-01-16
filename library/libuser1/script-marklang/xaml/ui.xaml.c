#include "libuser.h"




int xamlroot_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void** buf,int len)
{
	return 0;
}
int xamlroot_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void** buf,int len)
{
	return 0;
}
int xamlroot_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int xamlroot_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@xamlroot_attach\n");
	return 0;
}




int xamlroot_search(_obj* world)
{
	return 0;
}
int xamlroot_modify(_obj* world)
{
	return 0;
}
int xamlroot_delete(_obj* world)
{
	return 0;
}
int xamlroot_create(_obj* world, void* str)
{
	return 0;
}
