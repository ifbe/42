#include "libuser.h"




void gl41cmdq_clear(struct entity* ent)
{
	u64* code = ent->gl41cmdq_code;
	code[0] = hex32('c','l','r',0);
}




int gl41cmdq_search(struct entity* act)
{
	return 0;
}
int gl41cmdq_modify(struct entity* act)
{
	return 0;
}
int gl41cmdq_delete(struct entity* act)
{
	return 0;
}
int gl41cmdq_create(struct entity* act, void* addr)
{
	return 0;
}




int gl41cmdq_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int gl41cmdq_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int gl41cmdq_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41cmdq_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}