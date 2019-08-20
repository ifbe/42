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




int gl41data_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//for each object in target world: read ctx
	struct actor* data;
	struct relation* rel;

	data = (void*)(self->chip);
	if(0 == data)return 0;

	rel = data->orel0;
	if(0 == rel)return 0;

	gl41data_before(data);

	self = (void*)(&rel->dstchip);
	peer = (void*)(&rel->srcchip);
	actorread(self, peer, buf, len);

	gl41data_after(data);
	return 0;
}
int gl41data_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
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

	buf = act->gl_camera = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	buf = act->gl_light = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	buf = act->gl_solid = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	buf = act->gl_opaque = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	return 0;
}
