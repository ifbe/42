#include "libuser.h"




void gl41data_before(struct entity* ctx)
{
	int j;
	struct glsrc* src;

	//solid clear myown, forget other
	for(j=0;j<solidaid_max;j++){
		src = ctx->gl_solid[j];
		if(0 == src)continue;

		src->vbuf_h = 0;
		src->ibuf_h = 0;
	}
	for(;j<64;j++){
		ctx->gl_solid[j] = 0;
	}

	//opaque: clear myown, forget other
	for(j=0;j<opaqueaid_max;j++){
		src = ctx->gl_opaque[j];
		if(0 == src)continue;

		src->vbuf_h = 0;
		src->ibuf_h = 0;
	}
	for(;j<64;j++){
		ctx->gl_opaque[j] = 0;
	}
}
void gl41data_after(struct entity* ctx)
{
	int j;
	struct glsrc* src;

	//solid: enqueue
	for(j=0;j<solidaid_max;j++)
	{
		src = ctx->gl_solid[j];
		if(0 == src)continue;

		src->vbuf_enq += 1;
		src->ibuf_enq += 1;
	}

	//opaque: enqueue
	for(j=0;j<opaqueaid_max;j++)
	{
		src = ctx->gl_opaque[j];
		if(0 == src)continue;

		src->vbuf_enq += 1;
		src->ibuf_enq += 1;
	}
}
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt)
{
	int j;
	//say("@gl41data_insert:%llx,%x,%llx,%x\n", ctx,type,src,cnt);

	if('s' == type){
		for(j=solidaid_max;j<64;j++){
			if(0 == ctx->gl_solid[j]){
				ctx->gl_solid[j] = src;
				break;
			}
		}
	}
	if('o' == type){
		for(j=opaqueaid_max;j<64;j++){
			if(0 == ctx->gl_opaque[j]){
				ctx->gl_opaque[j] = src;
				break;
			}
		}
	}
}/*
void* gl41data_alloc(struct entity* ctx, int type)
{
	int j;
	struct datapair* pair;

	if('s' == type){
		pair = ctx->gl_solid;
		if(0 == pair)return 0;

		for(j=solidaid_max;j<64;j++){
			if(0 == pair[j].src.vbuf)return &pair[j];
		}
	}
	if('o' == type){
		pair = ctx->gl_opaque;
		if(0 == pair)return 0;

		for(j=opaqueaid_max;j<64;j++){
			if(0 == pair[j].src.vbuf)return &pair[j];
		}
	}
	return 0;
}
void gl41data_copy(struct entity* ctx, struct style* geom, struct style* part)
{
	int j,k;
	struct glsrc* data;
	u8* src;
	u8* dst;
	if(0 == part)return;
	if(0 == geom)return;

	for(j=0;j<4;j++){
		src = (void*)(part->data[j]);
		if(0 == src)return;

		//
		dst = (void*)(geom->data[j]);
		if(0 == dst){
			data = (void*)src;
			if(data->opaque)dst = gl41data_alloc(ctx, 'o');
			else dst = gl41data_alloc(ctx, 's');
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
}*/




int gl41data_read_vertex(
	struct entity* glctx, struct entity* cam, struct entity* actor, struct entity* world,
	struct halfrel** stack, int rsp, void* buf, int len)
{
	struct relation* rel;
	//say("gldata_read_vertex\n");

	gl41data_before(glctx);

	//second read other
	rel = world->orel0;
	while(1){
		if(0 == rel)break;

		if(cam == rel->pdstchip)goto next;
		if(_ent_ == rel->dsttype){
			stack[rsp+0] = (void*)(rel->src);
			stack[rsp+1] = (void*)(rel->dst);
			entityread(stack[rsp+1], stack[rsp+0], stack, rsp, 0, 'v');
		}
next:
		rel = samesrcnextdst(rel);
	}

	gl41data_after(glctx);
	return 0;
}
int gl41data_read_matrix(
	struct entity* glctx, struct entity* cam, struct entity* actor, struct entity* world,
	struct halfrel** stack, int rsp, void* buf, int len)
{
	struct relation* rel;
	//say("@gldata_read_matrix\n");

	rel = world->orel0;
	while(1){
		if(0 == rel)break;

		if(cam == rel->pdstchip)goto next;
		if(_ent_ == rel->dsttype){
			stack[rsp+0] = (void*)(rel->src);
			stack[rsp+1] = (void*)(rel->dst);
			entityread(stack[rsp+1], stack[rsp+0], stack, rsp, 0, '?');
		}
next:
		rel = samesrcnextdst(rel);
	}
	return 0;
}




//stack[-6] = ogl, 0
//stack[-5] = wnd, 0
//stack[-4] = wnd, area
//stack[-3] = cam, 0
//stack[-2] = act, slot
//stack[-1] = wrd, geom
int gl41data_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	int ret;
	struct entity* wnd;
	struct entity* cam;
	struct entity* actor;
	struct entity* world;
	//say("%d,%llx@gl41data_read: %.4s, %.4s\n", rsp, stack, &peer->flag, &self->flag);

	wnd = stack[rsp-4]->pchip;
	if(0 == wnd)return 0;
	cam = stack[rsp-3]->pchip;
	if(0 == cam)return 0;
	actor = stack[rsp-2]->pchip;
	if(0 == actor)return 0;
	world = stack[rsp-1]->pchip;
	if(0 == world)return 0;

found:
	if('v' == len)gl41data_read_vertex(wnd,cam, actor,world, stack, rsp, 0, 0);
	if('?' == len)gl41data_read_matrix(wnd,cam, actor,world, stack, rsp, 0, 0);
	return 0;
}
int gl41data_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	float x,y,x0,y0,xn,yn;
	short* v;
	struct relation* rel;
	struct event* ev;

	struct supply* wnd;
	struct fstyle* sty;
	struct entity* cam;

	ev = buf;
	//say("@gl41data_write:%llx,%llx,%llx,%llx\n", ev->why, ev->what, ev->where, ev->when);

	wnd = self->pchip;
	if(0 == wnd)return 0;

	if(0x4070 == ev->what){
		rel = wnd->oreln;
		if(0 == rel)return 0;

		while(1){
			if(0 == rel)return 0;
			sty = rel->psrcfoot;
			x0 = sty->vc[0] * wnd->width;
			y0 = sty->vc[1] * wnd->height;
			xn = sty->vq[0] * wnd->width + x0;
			yn = sty->vq[1] * wnd->height + y0;

			v = (short*)ev;
			x = v[0];
			y = wnd->height-v[1];
			if( (x>x0) && (x<xn) && (y>y0) && (y<yn) )goto found;
			rel = samesrcprevdst(rel);
		}
		return 0;
	}
	else{
		rel = wnd->buf;
		if(0 == rel)rel = wnd->orel0;
		if(0 == rel)return 0;
	}

found:
	wnd->buf = rel;
	stack[rsp+0] = (void*)(rel->src);	//wnd,area
	stack[rsp+1] = (void*)(rel->dst);	//cam,gl41
	entitywrite(stack[rsp+1], stack[rsp+0], stack, rsp+4, buf, len);

/*	struct entity* ctx;
	struct entity* cam;
	struct relation* rel;
	//say("@gl41data_write\n");

	ctx = self->pchip;
	if(0 == ctx)return 0;

	rel = ctx->orel0;
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dsttype){
		if(stack[rsp-1]->flag == rel->srcflag){
			cam = rel->pdstchip;
			//get matrix
			stack[rsp+0] = (void*)(rel->src);
			stack[rsp+1] = (void*)(rel->dst);
			gl41data_write_event(ctx, cam, stack, rsp+2, buf, len);
		}//found camera
		}//correct type

		rel = samesrcnextdst(rel);
	}*/
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




int gl41data_search(struct entity* win)
{
	return 0;
}
int gl41data_modify(struct entity* win)
{
	return 0;
}
int gl41data_delete(struct entity* win)
{
	return 0;
}
int gl41data_create(struct entity* act, void* flag)
{
	act->gl_camera = memorycreate(0x10000, 0);
	act->gl_light  = memorycreate(0x10000, 0);
	act->gl_solid  = memorycreate(0x10000, 0);
	act->gl_opaque = memorycreate(0x10000, 0);
	return 0;
}
