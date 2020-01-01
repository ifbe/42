#include "libuser.h"




void gl41data_tmpcam(struct entity* wnd)
{
    int x,y;
    void* trick = wnd->gl_camera;
    struct gl41data* data = trick + 0x400;
    float (*m)[4] = trick + 0x800;
    float* v = trick + 0xc00;
    for(y=0;y<4;y++){
        for(x=0;x<4;x++){
            if(x == y)m[y][x] = 1.0;
            else m[y][x] = 0.0;
        }
        v[y] = 0.0;
    }

    data->src.arg[0].fmt = 'm';
    data->src.arg[0].name = "cammvp";
    data->src.arg[0].data = m;
    data->src.arg[1].fmt = 'v';
    data->src.arg[1].name = "camxyz";
    data->src.arg[1].data = v;
    wnd->gl_camera[0] = data;
}
void gl41data_tmplit(struct entity* wnd)
{
    int x,y;
    void* trick = wnd->gl_light;
    struct gl41data* data = trick + 0x400;
    float (*m)[4] = trick + 0x800;
    float* v = trick + 0xc00;
}
void gl41data_before(struct entity* ctx)
{
	int j;
	struct glsrc* src;

	//camera,light: forget all
	ctx->gl_camera[0] = 0;
	ctx->gl_light[0] = 0;

	//solid: clear myown, forget other
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
}




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
		while(1){
			if(0 == rel)return 0;
			sty = rel->psrcfoot;
			x0 = sty->vc[0] * wnd->width;
			y0 = sty->vc[1] * wnd->height;
			xn = sty->vq[0] * wnd->width + x0;
			yn = sty->vq[1] * wnd->height + y0;

			v = (short*)ev;
			x = v[0];
			y = (wnd->height-1) - v[1];
			if( (x>x0) && (x<xn) && (y>y0) && (y<yn) )goto found;
			rel = samesrcprevdst(rel);
		}
		return 0;
	}
	else{
		rel = wnd->buf;
		if(0 == rel)rel = wnd->oreln;
		if(0 == rel)return 0;
	}

found:
	wnd->buf = rel;
	stack[rsp+0] = (void*)(rel->src);	//wnd,area
	stack[rsp+1] = (void*)(rel->dst);	//cam,gl41
	entitywrite(stack[rsp+1], stack[rsp+0], stack, rsp+4, buf, len);
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
