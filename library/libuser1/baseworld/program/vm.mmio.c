#include "libuser.h"
#define _ram_ hex32('r','a','m', 0)




int mmiospace_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void** buf,int len)
{
	return take_data_from_peer(ent,_ram_, stack,sp, arg,key, buf,len);
}
int mmiospace_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void** buf,int len)
{
	return 0;
}
int mmiospace_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int mmiospace_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@mmiospace_attach\n");
	return 0;
}




int mmiospace_search(_obj* world)
{
	return 0;
}
int mmiospace_modify(_obj* world)
{
	return 0;
}
int mmiospace_delete(_obj* world)
{
	return 0;
}
int mmiospace_create(_obj* world, void* str)
{
	return 0;
}
