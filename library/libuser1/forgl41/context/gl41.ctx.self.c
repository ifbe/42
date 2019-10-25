#include "libuser.h"




//stack:
//-4: ogl
//-3: glwnd
//-2: glwnd, area
//-1: glctx, frus
int gl41data_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct actor* mwnd;
	struct actor* data;
	struct relation* rel;
	//say("%d,%llx@gl41data_read: %.4s, %.4s\n", rsp, stack, &peer->flag, &self->flag);

	data = self->pchip;
	if(0 == data)return 0;

	rel = data->orel0;
	while(1){
		if(0 == rel)break;

		if(_act_ == rel->dsttype){
			stack[rsp+0] = (void*)(rel->src);
			stack[rsp+1] = (void*)(rel->dst);
			actorread(stack[rsp+1], stack[rsp+0], stack, rsp+2, 0, 0);
		}

		rel = samesrcnextdst(rel);
	}

	return 0;
}
int gl41data_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct actor* act;
	struct relation* rel;
	say("@gl41data_write\n");

	act = self->pchip;
	if(0 == act)return 0;
	rel = act->orel0;
	if(0 == rel)return 0;

	self = (void*)(rel->dst);
	peer = (void*)(rel->src);
	actorwrite(self, peer, 0, 0, buf, len);
	return 0;
}
int gl41data_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41data_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int gl41data_search(struct arena* win)
{
	return 0;
}
int gl41data_modify(struct arena* win)
{
	return 0;
}
int gl41data_delete(struct arena* win)
{
	return 0;
}
int gl41data_create(struct actor* act, void* flag)
{
	int j;
	u8* buf;

	buf = act->gl_camera = memorycreate(0x10000, 0);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	buf = act->gl_light = memorycreate(0x10000, 0);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	buf = act->gl_solid = memorycreate(0x10000, 0);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	buf = act->gl_opaque = memorycreate(0x10000, 0);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	return 0;
}
