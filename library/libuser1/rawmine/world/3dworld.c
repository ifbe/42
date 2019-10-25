#include "libuser.h"
int preprocess(void*);
int postprocess(void*);
int actorinput_touch(struct arena* win, struct event* ev);




void gl41data_before(struct actor* ctx)
{
	int j;
	struct datapair* mod;

	mod = ctx->gl_solid;
	for(j=0;j<solidaid_max;j++)
	{
		mod[j].src.vbuf_h = 0;
		mod[j].src.ibuf_h = 0;
	}

	mod = ctx->gl_opaque;
	for(j=0;j<opaqueaid_max;j++)
	{
		mod[j].src.vbuf_h = 0;
		mod[j].src.ibuf_h = 0;
	}
}
void gl41data_after(struct actor* ctx)
{
	int j;
	struct datapair* mod;

	mod = ctx->gl_solid;
	for(j=0;j<solidaid_max;j++)
	{
		mod[j].src.vbuf_enq += 1;
		mod[j].src.ibuf_enq += 1;
	}

	mod = ctx->gl_opaque;
	for(j=0;j<opaqueaid_max;j++)
	{
		mod[j].src.vbuf_enq += 1;
		mod[j].src.ibuf_enq += 1;
	}
}
void* ctx_alloc(struct actor* ctx)
{
	int j;
	struct datapair* pair = ctx->gl_solid;
	if(0 == pair)return 0;

	for(j=16;j<64;j++){
		if(0 == pair[j].src.vbuf)return &pair[j];
	}
	return 0;
}
void ctx_copy(struct actor* ctx, struct style* sty, struct style* pin)
{
	int j,k;
	u8* src;
	u8* dst;
	if(0 == pin)return;
	if(0 == sty)return;

	for(j=0;j<4;j++){
		src = (void*)(pin->data[j]);
		if(0 == src)return;

		//data
		dst = (void*)(sty->data[j]);
		if(0 == dst){
			dst = ctx_alloc(ctx);
			if(0 == dst)return;

			sty->data[j] = (u64)dst;
			say("new: %llx\n", src, dst);
		}

		//copy
		for(k=0;k<sizeof(struct glsrc);k++)dst[k] = src[k];
		//printmemory(dst, 0x200);
	}
//struct datapair* srcpair = (void*)src;
//say("6666@method=%x, geom=%x, ibuf_h=%x\n", srcpair->src.method, srcpair->src.geometry, srcpair->src.ibuf_h);
}




//stack:
//-6: ogl
//-5: glwnd
//-4: glwnd, area
//-3: glctx, frus
//-2: glctx
//-1: world
int world3d_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct actor* glctx;
	struct actor* world;
	struct relation* tocam;
	struct relation* toobj;
	struct style* sty;
	struct style* pin;
	//say("%d,%llx@world3d_read: %.4s, %.4s\n", rsp, stack, &peer->flag, &self->flag);

	glctx = peer->pchip;
	if(0 == glctx)return 0;

	world = self->pchip;
	if(0 == world)return 0;

	gl41data_before(glctx);

readcam:
	tocam = world->orel0;
	while(1){
		if(0 == tocam)break;
		if(stack[rsp-3]->flag == tocam->srcflag){
			stack[rsp+0] = (void*)(tocam->src);
			stack[rsp+1] = (void*)(tocam->dst);
			actorread(stack[rsp+1], stack[rsp+0], stack, rsp+2, 0, 0);

			ctx_copy(glctx, sty, pin);
			goto readobj;
		}
		tocam = samesrcnextdst(tocam);
	}
	goto theend;

readobj:
	toobj = world->orel0;
	while(1){
		if(0 == toobj)break;

		if(toobj == tocam)goto next;
		if(_act_ == toobj->dsttype){
			pin = toobj->pdstfoot;
			sty = toobj->psrcfoot;
			if(sty){if('#' == sty->i.uc[3])goto next;}

			self = (void*)(toobj->dst);
			peer = (void*)(toobj->src);
			actorread(self, peer, stack, rsp+2, 0, 0);

			ctx_copy(glctx, sty, pin);
		}
next:
		toobj = samesrcnextdst(toobj);
	}

theend:
	gl41data_after(glctx);
	return 0;
}
int world3d_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct actor* world;
	struct relation* rel;
	say("@world3d_write\n");

	world = self->pchip;
	if(0 == world)return 0;

	rel = world->orel0;
	while(1){
		if(0 == rel)break;
		if(hex32('c','a','m','0') == rel->srcflag){
			goto found;
		}
		rel = samesrcnextdst(rel);
	}
	return 0;

found:
	self = (void*)(rel->dst);
	peer = (void*)(rel->src);
	actorwrite(self, peer, 0, 0, buf, len);
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
