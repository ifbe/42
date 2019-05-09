#include "libuser.h"
void* allocarena();
int actorstart(void*, void*);
//
int preprocess(void*);
int postprocess(void*);
int actorinput_touch(struct arena* win, struct event* ev);




void defaultvertex(struct arena* win)
{
	//target
	win->target.vc[0] = 0.0;
	win->target.vc[1] = 0.0;
	win->target.vc[2] = 0.0;

	win->target.vr[0] = 500.0;
	win->target.vr[1] = 0.0;
	win->target.vr[2] = 0.0;

	win->target.vf[0] = 0.0;
	win->target.vf[1] = 500.0;
	win->target.vf[2] = 0.0;

	win->target.vu[0] = 0.0;
	win->target.vu[1] = 0.0;
	win->target.vu[2] = 500.0;

	//camera
	win->camera.vl[0] = -1.0;
	win->camera.vl[1] = 0.0;
	win->camera.vl[2] = 0.0;

	win->camera.vr[0] = 1.0;
	win->camera.vr[1] = 0.0;
	win->camera.vr[2] = 0.0;

	win->camera.vb[0] = 0.0;
	win->camera.vb[1] =-0.70710678118655;
	win->camera.vb[2] =-0.70710678118655;

	win->camera.vu[0] = 0.0;
	win->camera.vu[1] = 0.70710678118655;
	win->camera.vu[2] = 0.70710678118655;

	win->camera.vn[0] = 0.0;
	win->camera.vn[1] = 0.70710678118655;
	win->camera.vn[2] =-0.70710678118655;
/*
	win->camera.vf[0] = 0.0;
	win->camera.vf[1] = 0.0;
	win->camera.vf[2] = 0.0;

	win->camera.vq[0] = 0.0;
	win->camera.vq[1] = 0.0;
	win->camera.vq[2] = 0.0;
*/
	win->camera.vc[0] = 0.0;
	win->camera.vc[1] = -2000.0;
	win->camera.vc[2] = 2000.0;
}




int vbonode_sread(struct arena* win, struct style* stack)
{
	struct style* sty;
	struct relation* rel;
	struct halfrel* self;
	struct halfrel* peer;

	win->vfmt = _3d_;
	preprocess(win);

	rel = win->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			if(sty){if('#' == sty->uc[3])goto next;}

			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			actor_rootread(self, peer, 0, 0);
		}
next:
		rel = samesrcnextdst(rel);
	}

	postprocess(win);
	return 0;
}
int vbonode_swrite(struct arena* win, struct style* stack, struct event* ev)
{
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
			if(sty){if('#' == sty->uc[3])goto next;}

			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			ret = actor_rootwrite(self, peer, ev, 0);
			if(ret)break;
		}
next:
		rel = samesrcprevdst(rel);
	}

	if('p' == (ev->what&0xff))actorinput_touch(win, ev);
	return ret;
}
int vbonode_stop(struct arena* win, struct style* sty)
{
	return 0;
}
int vbonode_start(struct arena* twig, void* tf, struct arena* root, void* rf)
{
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
}
int vbonode_delete(struct arena* win)
{
	return 0;
}
void* vbonode_create(u64 type, u64 flag)
{
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

	defaultvertex(win);

	win->gl_camera = buf = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	win->gl_light = buf = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	win->gl_solid = buf = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	win->gl_opaque = buf = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	return win;
}
