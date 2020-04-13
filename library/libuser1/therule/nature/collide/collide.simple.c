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




int collide_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@collide_read\n");
	return 0;
}
int collide_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
