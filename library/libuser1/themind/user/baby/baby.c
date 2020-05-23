#include "libuser.h"




int baby_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@baby_read: crying\n");
	return 0;
}
int baby_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@baby_write: crying\n");
	return 0;
}
int baby_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@baby_discon: crying\n");
	return 0;
}
int baby_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@baby_linkup: crying\n");
	return 0;
}




int baby_search(struct entity* act)
{
	say("@baby_search: crying\n");
	return 0;
}
int baby_modify(struct entity* act)
{
	say("@baby_modity: crying\n");
	return 0;
}
int baby_delete(struct entity* act)
{
	say("@baby_delete: crying\n");
	return 0;
}
int baby_create(struct entity* act)
{
	say("@baby_create: crying\n");
	return 0;
}




void baby_free()
{
	//say("@baby_free: crying\n");
}
void baby_init(void* addr)
{
	//say("@baby_init: crying\n");
}
