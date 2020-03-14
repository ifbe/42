#include "libuser.h"




//stack:
//-2: glwnd, area
//-1: glctx, frus
int virtual_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	say("@virtual_read\n");
	return 0;
}
int virtual_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	return 0;
/*	struct entity* world;
	struct relation* rel;
	say("@virtual_write\n");

	world = self->pchip;
	if(0 == world)return 0;

	rel = world->orel0;
	while(1){
		if(0 == rel)break;
		if(stack[rsp-1]->flag == rel->srcflag){
			stack[rsp+0] = (void*)(rel->src);
			stack[rsp+1] = (void*)(rel->dst);
			entitywrite(stack[rsp+1], stack[rsp+0], stack, rsp+2, buf, len);
			break;
		}
		rel = samesrcnextdst(rel);
	}
	return 0;

	int ret;
	struct style* sty;
	struct relation* rel;
	struct halfrel* self;
	struct halfrel* peer;

	ret = 0;
	rel = win->oreln;
	while(1)
	{
		if(0 == rel)break;

		if(_ent_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			if(sty){if('#' == sty->i.uc[3])goto next;}

			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			ret = entitywrite(self, peer, ev, 0);
			if(ret)break;
		}
next:
		rel = samesrcprevdst(rel);
	}

	if('p' == (ev->what&0xff))entityinput_touch(win, ev);
	return ret;
*/
}
int virtual_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int virtual_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@virtual_linkup\n");
	return 0;
}




int virtual_search(struct entity* world)
{
	return 0;
}
int virtual_modify(struct entity* world)
{
	return 0;
}
int virtual_delete(struct entity* world)
{
	return 0;
}
int virtual_create(struct entity* world, void* str)
{
	int j;
	u8* buf;
	say("@virtual_create\n");
/*
	buf = world->buf = memorycreate(0x10000, 0);
	for(j=0;j<0x10000;j++)buf[j] = 0;*/
	return 0;
/*
	int j;
	u8* buf;
	struct supply* win;
	struct entity* act;
	if(_vbo_ != type)return 0;

	win = allocsupply();
	if(0 == win)return 0;

	win->type = _ctx_;
	win->fmt = _vbo_;

	win->width = win->fbwidth = 1024;
	win->height = win->fbheight = 768;
	win->depth = win->fbdepth = 1024;

	//defaultvertex(win);

	win->gl_camera = buf = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	win->gl_light = buf = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	win->gl_solid = buf = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	win->gl_opaque = buf = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;
*/
}
