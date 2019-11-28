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




void* gl41data_findworld(struct entity* cam)
{
	struct relation* rel;
	struct entity* world;
	//say("@gldata_write_event\n");

	rel = cam->irel0;
	while(1){
		if(0 == rel)break;
		if(_ent_ == rel->srctype){
			world = rel->psrcchip;
			if(_world3d_ == world->type)return rel;
		}
		rel = samedstnextsrc(rel);
	}
	return 0;
}
int gl41data_read_vertex(struct entity* glctx, struct relation* wrd2cam, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct relation* rel;
	struct style* geom;
	struct style* part;
	struct entity* camera;
	struct entity* world;
	//say("gldata_read_vertex\n");

	gl41data_before(glctx);

	//first read camera
	camera = stack[rsp-2]->pchip;
	world = stack[rsp-1]->pchip;
	entityread((void*)(wrd2cam->dst), (void*)(wrd2cam->src), stack, rsp, 0, 'v');

	//second read other
	rel = world->orel0;
	while(1){
		if(0 == rel)break;

		if(rel == wrd2cam)goto next;
		if(_ent_ == rel->dsttype){
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
int gl41data_read_matrix(struct entity* ctx, struct relation* wrd2cam, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct relation* rel;
	struct entity* camera;
	struct entity* world;
	//say("@gldata_read_matrix\n");

	//first read mirror
	camera = stack[rsp-2]->pchip;
	world = stack[rsp-1]->pchip;
	rel = world->orel0;
	while(1){
		if(0 == rel)break;

		if(rel == wrd2cam)goto next;
		if(_ent_ == rel->dsttype){
			entityread((void*)(rel->dst), (void*)(rel->src), stack, rsp, 0, '?');
		}
next:
		rel = samesrcnextdst(rel);
	}

	//then read mvp
	entityread((void*)(wrd2cam->dst), (void*)(wrd2cam->src), stack, rsp, 0, '?');

	return 0;
}




//stack[-4] = ogl, 0
//stack[-3] = wnd, 0
//stack[-2] = wnd, area
//stack[-1] = cam, 0
int gl41data_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	int ret;
	struct entity* wnd;
	struct entity* cam;
	struct relation* rel;
	//say("%d,%llx@gl41data_read: %.4s, %.4s\n", rsp, stack, &peer->flag, &self->flag);

	wnd = self->pchip;
	if(0 == wnd)return 0;

	//cam given by stack
	cam = stack[rsp-1]->pchip;
	if(0 == cam)return 0;

	//from cam find world
	rel = gl41data_findworld(cam);
	if(0 == rel)return 0;

found:
	//stack: (ogl,0, wnd,0), (wnd,area, cam,0), (cam,0, world,geom)
	stack[rsp+0] = (void*)(rel->dst);	//camera
	stack[rsp+1] = (void*)(rel->src);	//world
	if('v' == len)gl41data_read_vertex(wnd, rel, stack, rsp+2, 0, 0);
	if('?' == len)gl41data_read_matrix(wnd, rel, stack, rsp+2, 0, 0);
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
	stack[rsp+1] = (void*)(rel->dst);	//cam,-

	rel = gl41data_findworld(rel->pdstchip);
	stack[rsp+2] = (void*)(rel->dst);	//cam,-
	stack[rsp+3] = (void*)(rel->src);	//world,geom

	entitywrite(stack[rsp+2], stack[rsp+3], stack, rsp+4, buf, len);

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
