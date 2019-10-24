#include "libuser.h"
/*
void* allocarena();
int preprocess(void*);
int postprocess(void*);
int actorinput_touch(struct arena* win, struct event* ev);



int gl41data_read(struct arena* win, struct style* stack)
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
			if(sty){if('#' == sty->i.uc[3])goto next;}

			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			actorread(self, peer, 0, 0);
		}
next:
		rel = samesrcnextdst(rel);
	}

	postprocess(win);
	return 0;
}
int gl41data_write(struct arena* win, struct style* stack, struct event* ev)
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
}
int gl41data_stop(struct arena* win, struct style* sty)
{
	return 0;
}
int gl41data_start(struct arena* twig, void* tf, struct arena* root, void* rf)
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
}*/
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
void gl41data_get(struct relation* rel)
{
	struct halfrel* self;
	struct halfrel* peer;
	if(0 == rel)return;

	self = (void*)(&rel->dstchip);
	peer = (void*)(&rel->srcchip);
	actorread(self, peer, 0, 0, 0, 0);
}
void gl41data_copy(struct relation* rel)
{
	int j,k;
	struct actor* world;
	struct actor* data;
	struct datapair* srcpair;
	struct datapair* dstpair;
	u8* src;
	u8* dst;

	world = (void*)(rel->dstchip);
	if(0 == world)return;

	data = (void*)(rel->srcchip);
	if(0 == data)return;

	srcpair = world->buf;
	if(0 == srcpair)return;

	dstpair = data->gl_solid;
	if(0 == dstpair)return;

	for(j=0;j<16;j++){
		if(0 == srcpair[j].src.vbuf)continue;

		src = (void*)&srcpair[j];
		dst = (void*)&dstpair[j + solidaid_max];
		for(k=0;k<sizeof(struct glsrc);k++)dst[k] = src[k];
	}

//say("7777@method=%x, geom=%x, ibuf_h=%x\n", srcpair->src.method, srcpair->src.geometry, srcpair->src.ibuf_h);
}




int gl41data_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	void* stack[8];
	struct actor* data;
	struct actor* mwnd;
	struct relation* rel;
	//say("@gl41data_read: %.4s, %.4s\n", &self->flag, &peer->flag);

	mwnd = peer->pchip;
	if(0 == mwnd)return 0;

	data = self->pchip;
	if(0 == data)return 0;

	gl41data_before(data);

	stack[0] = peer;
	stack[1] = self;
	idx = 2;

	rel = data->orel0;
	while(1){
		if(0 == rel)break;

		actorread((void*)(rel->dst), (void*)(rel->src), stack, idx, 0, 0);

		rel = samesrcnextdst(rel);
	}

	gl41data_after(data);
/*
	//say("mwnd = %llx, data = %llx\n", mwnd, data);

	rel = data->orel0;
	if(0 == rel)return 0;

	gl41data_before(data);

	gl41data_get(rel);

	gl41data_after(data);

	gl41data_copy(rel);
*/
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
