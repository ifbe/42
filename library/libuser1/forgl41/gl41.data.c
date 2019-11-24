#include "libuser.h"




void gl41data_before(struct entity* ctx)
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
void gl41data_after(struct entity* ctx)
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
void* gl41data_alloc(struct entity* ctx)
{
	int j;
	struct datapair* pair = ctx->gl_solid;
	if(0 == pair)return 0;

	for(j=16;j<64;j++){
		if(0 == pair[j].src.vbuf)return &pair[j];
	}
	return 0;
}
void gl41data_copy(struct entity* ctx, struct style* geom, struct style* part)
{
	int j,k;
	u8* src;
	u8* dst;
	if(0 == part)return;
	if(0 == geom)return;

	for(j=0;j<4;j++){
		src = (void*)(part->data[j]);
		if(0 == src)return;

		//data
		dst = (void*)(geom->data[j]);
		if(0 == dst){
			dst = gl41data_alloc(ctx);
			if(0 == dst)return;

			geom->data[j] = (u64)dst;
			say("new: %llx\n", src, dst);
		}

		//copy
		for(k=0;k<sizeof(struct glsrc);k++)dst[k] = src[k];
		//printmemory(dst, 0x200);
	}
//struct datapair* srcpair = (void*)src;
//say("6666@method=%x, geom=%x, ibuf_h=%x\n", srcpair->src.method, srcpair->src.geometry, srcpair->src.ibuf_h);
}




int gl41data_write_event(struct entity* ctx, struct entity* cam, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct relation* rel;
	struct entity* world;
	//say("@gldata_write_event\n");

	rel = cam->irel0;
	while(1){
		if(0 == rel)break;
		if(_act_ == rel->srctype){
			//search for camera's world
			world = rel->psrcchip;
			if(_world3d_ == world->type){
				stack[rsp+0] = (void*)(rel->dst);	//camera
				stack[rsp+1] = (void*)(rel->src);	//world
				entitywrite(stack[rsp+0], stack[rsp+1], stack, rsp+2, buf, len);
			}
		}
		rel = samedstnextsrc(rel);
	}
	return 0;
}
int gl41data_read_matrix(struct entity* ctx, struct entity* cam, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct relation* rel;
	struct entity* world;
	//say("@gldata_read_matrix\n");

	rel = cam->irel0;
	while(1){
		if(0 == rel)break;
		if(_act_ == rel->srctype){
			//search for camera's world
			world = rel->psrcchip;
			if(_world3d_ == world->type){
				stack[rsp+0] = (void*)(rel->dst);	//camera
				stack[rsp+1] = (void*)(rel->src);	//world
				entityread(stack[rsp+0], stack[rsp+1], stack, rsp+2, 0, 'm');
				return 1;
			}
		}
		rel = samedstnextsrc(rel);
	}
	return 0;
}
int gl41data_read_vertex(struct entity* glctx, struct entity* world, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct relation* rel;
	struct style* geom;
	struct style* part;
	//say("gldata_read_vertex\n");

	gl41data_before(glctx);

	rel = world->orel0;
	while(1){
		if(0 == rel)break;

		if(_act_ == rel->dsttype){
			part = rel->pdstfoot;
			geom = rel->psrcfoot;
			if(geom){if('#' == geom->i.uc[3])goto next;}

			entityread((void*)(rel->dst), (void*)(rel->src), stack, rsp, 0, 'v');
			gl41data_copy(glctx, geom, part);
		}
next:
		rel = samesrcnextdst(rel);
	}

	gl41data_after(glctx);
	return 0;
}




//stack:
//-2: wnd, area
//-1: ctx, 0
int gl41data_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	int ret;
	struct supply* wnd;
	struct entity* ctx;
	struct entity* cam;
	struct relation* rel;
	//say("%d,%llx@gl41data_read: %.4s, %.4s\n", rsp, stack, &peer->flag, &self->flag);

	wnd = peer->pchip;
	if(_gl41wnd0_ != wnd->type)return 0;

	ctx = self->pchip;
	if(0 == ctx)return 0;

	rel = ctx->orel0;
	while(1){
		if(0 == rel)break;

		if(_act_ == rel->dsttype){
		if(stack[rsp-1]->flag == rel->srcflag){
			//this is cam
			cam = rel->pdstchip;

			//get matrix
			ret = gl41data_read_matrix(ctx, cam, stack, rsp, 0, 0);
			if(ret <= 0)break;

			//get vertex
			gl41data_read_vertex(ctx, stack[rsp+1]->pchip, stack, rsp+2, 0, 0);

			//get framebuffer
		}//found camera
		}//correct type

		rel = samesrcnextdst(rel);
	}

	return 0;
}
int gl41data_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* ctx;
	struct entity* cam;
	struct relation* rel;
	//say("@gl41data_write\n");

	ctx = self->pchip;
	if(0 == ctx)return 0;

	rel = ctx->orel0;
	while(1){
		if(0 == rel)break;

		if(_act_ == rel->dsttype){
		if(stack[rsp-1]->flag == rel->srcflag){
			cam = rel->pdstchip;
			//get matrix
			stack[rsp+0] = (void*)(rel->src);
			stack[rsp+1] = (void*)(rel->dst);
			gl41data_write_event(ctx, cam, stack, rsp+2, buf, len);
		}//found camera
		}//correct type

		rel = samesrcnextdst(rel);
	}
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




int gl41data_search(struct supply* win)
{
	return 0;
}
int gl41data_modify(struct supply* win)
{
	return 0;
}
int gl41data_delete(struct supply* win)
{
	return 0;
}
int gl41data_create(struct entity* act, void* flag)
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
