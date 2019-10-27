#include "libuser.h"




//stack:
//-2: glwnd, area
//-1: glctx, frus
int world3d_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	say("@world3d_read\n");
	return 0;
}
int world3d_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct actor* world;
	struct relation* rel;
	say("@world3d_write\n");

	world = self->pchip;
	if(0 == world)return 0;

	rel = world->orel0;
	while(1){
		if(0 == rel)break;
		if(stack[rsp-1]->flag == rel->srcflag){
			stack[rsp+0] = (void*)(rel->src);
			stack[rsp+1] = (void*)(rel->dst);
			actorwrite(stack[rsp+1], stack[rsp+0], stack, rsp+2, buf, len);
			break;
		}
		rel = samesrcnextdst(rel);
	}
	return 0;
/*
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

		if(_act_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			if(sty){if('#' == sty->i.uc[3])goto next;}

			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			ret = actorwrite(self, peer, ev, 0);
			if(ret)break;
		}
next:
		rel = samesrcprevdst(rel);
	}

	if('p' == (ev->what&0xff))actorinput_touch(win, ev);
	return ret;
*/
}
int world3d_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int world3d_start(struct halfrel* self, struct halfrel* peer)
{
	say("@world3d_start\n");
	return 0;
/*
	struct halfrel* self;
	struct halfrel* peer;
	struct relation* rel;

	rel = twig->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype){
			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			actorstart(self, peer);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
*/
}




int world3d_search(struct actor* world)
{
	return 0;
}
int world3d_modify(struct actor* world)
{
	return 0;
}
int world3d_delete(struct actor* world)
{
	return 0;
}
int world3d_create(struct actor* world, void* str)
{
	int j;
	u8* buf;
	say("@world3d_create\n");

	buf = world->buf = memorycreate(0x10000, 0);
	for(j=0;j<0x10000;j++)buf[j] = 0;
	return 0;
/*
	int j;
	u8* buf;
	struct arena* win;
	struct actor* act;
	if(_vbo_ != type)return 0;

	win = allocarena();
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
