#include "libuser.h"




void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v)
{
	float w = wnd->width;
	float h = wnd->height;
	float x0 = w * area->fs.vc[0];
	float y0 = h * area->fs.vc[1];
	float dx = w * area->fs.vq[0];
	float dy = h * area->fs.vq[1];
	short* t = (void*)ev;
	v[0] = (t[0]     - x0) / dx;
	v[1] = (h-1-t[1] - y0) / dy;
}




void gl41data_xxxcam(struct entity* wnd, struct fstyle* area)
{
	int x,y;
	void* trick = wnd->gl_camera;
	struct gl41data* data = trick + 0x400;
	float (*m)[4] = trick + 0x800;
	float* v = trick + 0xc00;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)m[y][x] = 0.0;
		v[y] = 0.0;
	}
	m[0][0] = 2.0 / (area->vq[0] * wnd->fbwidth);
	m[1][1] = 2.0 / (area->vq[1] * wnd->fbheight);
	m[2][2] = 1.0;
	m[3][3] = 1.0;
	//say("%f,%f\n", m[0][0], m[1][1]);

	data->src.arg[0].fmt = 'm';
	data->src.arg[0].name = "cammvp";
	data->src.arg[0].data = m;
	data->src.arg[1].fmt = 'v';
	data->src.arg[1].name = "camxyz";
	data->src.arg[1].data = v;
	wnd->gl_camera[0] = data;
}
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




//stack[-4] = ogl, area
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
	return 0;
}
int gl41data_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41data_linkup(struct halfrel* self, struct halfrel* peer)
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
