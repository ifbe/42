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




void gl41data_whcam(struct entity* wnd, struct fstyle* area)
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
	m[2][2] =-1.0;
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
void gl41data_01cam(struct entity* wnd)
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
	m[0][0] = 1.0;
	m[1][1] = 1.0;
	m[2][2] =-1.0;
	m[3][3] = 1.0;

	data->src.arg[0].fmt = 'm';
	data->src.arg[0].name = "cammvp";
	data->src.arg[0].data = m;
	data->src.arg[1].fmt = 'v';
	data->src.arg[1].name = "camxyz";
	data->src.arg[1].data = v;
	wnd->gl_camera[0] = data;
}




void gl41data_nolit(struct entity* wnd)
{
	int x,y;
	void* trick = wnd->gl_light;
	struct gl41data* data = trick + 0x400;
	float (*m)[4] = trick + 0x800;
	float* v = trick + 0xc00;
}
void gl41data_mylit(struct entity* wnd)
{
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




//[-4,-3]: ogl,area -> cam,togl
//[-2,-1]: cam,towr -> wor,geom
int gl41data_read(_ent* world,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct relation* rel = world->orel0;
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dsttype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].flag = rel->srcflag;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].flag = rel->dstflag;
			entityread(stack[sp+1].pchip, stack[sp+1].flag, stack,sp+2, arg,key, buf,len);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int gl41data_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
