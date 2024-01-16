#include "libuser.h"




int baby_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	logtoall("@baby_read: crying\n");
	return 0;
}
int baby_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	logtoall("@baby_write: crying\n");
	return 0;
}
int baby_detach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@baby_detach: crying\n");
	return 0;
}
int baby_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@baby_attach: crying\n");
	return 0;
}




int baby_search(_obj* act)
{
	logtoall("@baby_search: crying\n");
	return 0;
}
int baby_modify(_obj* act)
{
	logtoall("@baby_modity: crying\n");
	return 0;
}
int baby_delete(_obj* act)
{
	logtoall("@baby_delete: crying\n");
	return 0;
}
int baby_create(_obj* act)
{
	logtoall("@baby_create: crying\n");
	return 0;
}




void baby_exit()
{
	//logtoall("@baby_exit: crying\n");
}
void baby_init(void* addr)
{
	//logtoall("@baby_init: crying\n");
}
