#include "libuser.h"
#define _ram_ hex32('r','a','m', 0)




int mmiospace_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void** buf,int len)
{
	return take_data_from_peer(ent,_ram_, stack,sp, arg,key, buf,len);
}
int mmiospace_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void** buf,int len)
{
	return 0;
}
int mmiospace_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int mmiospace_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@mmiospace_linkup\n");
	return 0;
}




int mmiospace_search(struct entity* world)
{
	return 0;
}
int mmiospace_modify(struct entity* world)
{
	return 0;
}
int mmiospace_delete(struct entity* world)
{
	return 0;
}
int mmiospace_create(struct entity* world, void* str)
{
	return 0;
}
