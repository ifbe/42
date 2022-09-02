#include "libuser.h"




int baby_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@baby_read: crying\n");
	return 0;
}
int baby_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@baby_write: crying\n");
	return 0;
}
int baby_detach(struct halfrel* self, struct halfrel* peer)
{
	say("@baby_detach: crying\n");
	return 0;
}
int baby_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@baby_attach: crying\n");
	return 0;
}




int baby_search(_obj* act)
{
	say("@baby_search: crying\n");
	return 0;
}
int baby_modify(_obj* act)
{
	say("@baby_modity: crying\n");
	return 0;
}
int baby_delete(_obj* act)
{
	say("@baby_delete: crying\n");
	return 0;
}
int baby_create(_obj* act)
{
	say("@baby_create: crying\n");
	return 0;
}




void baby_exit()
{
	//say("@baby_exit: crying\n");
}
void baby_init(void* addr)
{
	//say("@baby_init: crying\n");
}
