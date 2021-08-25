#include "libuser.h"




int reality_taking(_obj* ent,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@reality_read\n");
	return 0;
}
int reality_giving(_obj* ent,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
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




int reality_search(_obj* world)
{
	return 0;
}
int reality_modify(_obj* world)
{
	return 0;
}
int reality_delete(_obj* world)
{
	return 0;
}
int reality_create(_obj* world, void* str)
{
	return 0;
}
