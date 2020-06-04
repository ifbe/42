#include "libuser.h"




int carcon_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@carcon_read:%.4s\n",&foot);
	return 0;
}
int carcon_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	say("@carcon_write:%.4s\n",&foot);
	return 0;
}
int carcon_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int carcon_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int carcon_search()
{
	return 0;
}
int carcon_modify()
{
	return 0;
}
int carcon_delete(struct entity* ent)
{
	return 0;
}
int carcon_create(struct entity* ent, void* str)
{
	say("@carcon_create\n");
	return 0;
}
