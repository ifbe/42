#include "libuser.h"




void gl41cmdq_clear(_obj* ent)
{
	u64* code = ent->gl41cmdq.code;
	code[0] = hex32('c','l','r',0);
}




int gl41cmdq_search(_obj* act)
{
	return 0;
}
int gl41cmdq_modify(_obj* act)
{
	return 0;
}
int gl41cmdq_delete(_obj* act)
{
	return 0;
}
int gl41cmdq_create(_obj* act, void* addr)
{
	return 0;
}




int gl41cmdq_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int gl41cmdq_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int gl41cmdq_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41cmdq_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}