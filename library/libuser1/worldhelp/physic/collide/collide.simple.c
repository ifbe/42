#include "libuser.h"




int collide_search()
{
	return 0;
}
int collide_modify()
{
	return 0;
}
int collide_delete(_obj* win)
{
	return 0;
}
int collide_create(_obj* win, void* str)
{
	say("@collide_create\n");
	return 0;
}




int collide_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@collide_read\n");
	return 0;
}
int collide_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@collide_write\n");
	return 0;
}
int collide_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int collide_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
