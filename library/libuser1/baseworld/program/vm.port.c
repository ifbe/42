#include "libuser.h"




int portspace_taking(_obj* ent,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	say("@portspace_read\n");
	return 0;
}
int portspace_giving(_obj* ent,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int portspace_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int portspace_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@portspace_attach\n");
	return 0;
}




int portspace_search(_obj* world)
{
	return 0;
}
int portspace_modify(_obj* world)
{
	return 0;
}
int portspace_delete(_obj* world)
{
	return 0;
}
int portspace_create(_obj* world, void* str)
{
	return 0;
}
