#include "libuser.h"




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
void* gl41data_alloc(struct actor* ctx)
{
	int j;
	struct datapair* pair = ctx->gl_solid;
	if(0 == pair)return 0;

	for(j=16;j<64;j++){
		if(0 == pair[j].src.vbuf)return &pair[j];
	}
	return 0;
}
void gl41data_copy(struct actor* ctx, struct style* sty, struct style* pin)
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
			dst = gl41data_alloc(ctx);
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
int gl41data_read_matrix(struct actor* glctx, struct actor* world, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct relation* rel;
	//say("%d,%llx@world3d_read: %.4s, %.4s\n", rsp, stack, &peer->flag, &self->flag);

	rel = world->orel0;
	while(1){
		if(0 == rel)break;
		if(stack[rsp-1]->flag == rel->srcflag){
			stack[rsp+0] = (void*)(rel->src);
			stack[rsp+1] = (void*)(rel->dst);
			actorread(stack[rsp+1], stack[rsp+0], stack, rsp+2, 0, 'm');
			break;
		}
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int gl41data_read_vertex(struct actor* glctx, struct actor* world, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct relation* rel;
	struct style* sty;
	struct style* pin;
	gl41data_before(glctx);

	rel = world->orel0;
	while(1){
		if(0 == rel)break;

		if(_act_ == rel->dsttype){
			pin = rel->pdstfoot;
			sty = rel->psrcfoot;
			if(sty){if('#' == sty->i.uc[3])goto next;}

			stack[rsp+0] = (void*)(rel->src);
			stack[rsp+1] = (void*)(rel->dst);
			actorread(stack[rsp+1], stack[rsp+0], stack, rsp+2, 0, 'v');

			gl41data_copy(glctx, sty, pin);
		}
next:
		rel = samesrcnextdst(rel);
	}

	gl41data_after(glctx);
	return 0;
}




//stack:
//-2: glwnd, area
//-1: glctx, frus
int gl41data_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct actor* glctx;
	struct actor* world;
	struct relation* rel;
	//say("%d,%llx@gl41data_read: %.4s, %.4s\n", rsp, stack, &peer->flag, &self->flag);

	glctx = self->pchip;
	if(0 == glctx)return 0;

	rel = glctx->orel0;
	while(1){
		if(0 == rel)break;

		if(_act_ == rel->dsttype){
			world = rel->pdstchip;
			gl41data_read_matrix(glctx, world, stack, rsp, 0, 0);
			gl41data_read_vertex(glctx, world, stack, rsp, 0, 0);
		}

		rel = samesrcnextdst(rel);
	}

	return 0;
}
int gl41data_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
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
	actorwrite(self, peer, stack, rsp, buf, len);
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
