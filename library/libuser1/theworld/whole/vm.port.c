#include "libuser.h"




int portspace_taking(struct entity* ent,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@portspace_read\n");
	return 0;
}
int portspace_giving(struct entity* ent,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int portspace_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int portspace_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@portspace_linkup\n");
	return 0;
}




int portspace_search(struct entity* world)
{
	return 0;
}
int portspace_modify(struct entity* world)
{
	return 0;
}
int portspace_delete(struct entity* world)
{
	return 0;
}
int portspace_create(struct entity* world, void* str)
{
	return 0;
}
