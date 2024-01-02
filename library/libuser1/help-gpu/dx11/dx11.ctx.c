#include "libuser.h"
struct unidata{
	mat4 mat;
	vec4 vec;
};




void dx11data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v)
{
	float w = wnd->whdf.width;
	float h = wnd->whdf.height;
	float x0 = w * area->fs.vc[0];
	float y0 = h * area->fs.vc[1];
	float dx = w * area->fs.vq[0];
	float dy = h * area->fs.vq[1];
	short* t = (void*)ev;
	v[0] = (t[0]     - x0) / dx;
	v[1] = (h-1-t[1] - y0) / dy;
}




void dx11data_nocam(_obj* wnd)
{
	void* trick = wnd->dx11list.world[0].camera;
	struct dx11data* data = trick + 0x400;
	struct unidata* uni = trick + 0x800;
	data->src.uni[0].buf = uni;
	data->src.uni[0].len = sizeof(struct unidata);

	int x,y;
	float (*m)[4] = uni->mat;
	float* v = uni->vec;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)m[y][x] = 0.0;
		v[y] = 0.0;
	}

	//x@[-1,1], y@[-1,1], z@[1.0,-1.0]
	v[0] = 0.0;
	v[1] = 0.0;
	v[2] =-2.0;
	m[0][0] = 1.0;
	m[1][1] = 1.0;
	m[2][2] = 1.0;
	m[3][3] = 1.0;

	wnd->dx11list.world[0].camera[0] = data;
}
void dx11data_01cam(_obj* wnd)
{
	void* trick = wnd->dx11list.world[0].camera;
	struct dx11data* data = trick + 0x400;
	struct unidata* uni = trick + 0x800;
	data->src.uni[0].buf = uni;
	data->src.uni[0].len = sizeof(struct unidata);

	int x,y;
	float (*m)[4] = uni->mat;
	float* v = uni->vec;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)m[y][x] = 0.0;
		v[y] = 0.0;
	}

	//x@[0,1), y@[0,1), z@[0,1) -> x@[-1,1], y@[-1,1], z@[1.0,0.0]
	v[0] = 0.0;
	v[1] = 0.0;
	v[2] = 2.0;
	m[0][0] = 2.0;
	m[0][3] =-1.0;
	m[1][1] = 2.0;
	m[1][3] =-1.0;
	m[2][2] = 1.0;
	m[3][3] = 1.0;

	wnd->dx11list.world[0].camera[0] = data;
}
void dx11data_whcam(_obj* wnd, struct fstyle* area)
{
	void* trick = wnd->dx11list.world[0].camera;
	struct dx11data* data = trick + 0x400;
	struct unidata* uni = trick + 0x800;
	data->src.uni[0].buf = uni;
	data->src.uni[0].len = sizeof(struct unidata);

	int x,y;
	float (*m)[4] = uni->mat;
	float* v = uni->vec;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)m[y][x] = 0.0;
		v[y] = 0.0;
	}

	//x@[0,w), y@[0,h), z@[0,1024) -> x@[-1,1], y@[-1,1], z@[0.9,0.1]
	v[0] = 0.0;
	v[1] = 0.0;
	v[2] = 2.0;
	m[0][0] = 2.0 / ((area->vq[0]-area->vc[0]) * wnd->whdf.fbwidth);
	m[0][3] =-1.0;
	m[1][1] = 2.0 / ((area->vq[1]-area->vc[1]) * wnd->whdf.fbheight);
	m[1][3] =-1.0;
	m[2][2] =-0.8 / 2;
	m[2][3] = 0.9;
	m[3][3] = 1.0;
	//no transpose

	wnd->dx11list.world[0].camera[0] = data;
}




void dx11data_nolit(_obj* wnd)
{
}
void dx11data_mylit(_obj* wnd)
{
}




void dx11data_before(_obj* ctx)
{
	int j;
	struct dx11data* p;

	//camera: default
	ctx->dx11list.world[0].camera[0] = 0;

	//light: default
	//ctx->dx11list.world[0].light[0] = 0;
	dx11data_nolit(ctx);

	//solid: clear myown, forget other
	for(j=0;j<solidaid_max;j++){
		p = ctx->dx11list.world[0].solid[j];
		if(0 == p)continue;

		p->src.vtx[0].vbuf_h = 0;
		p->src.vtx[0].ibuf_h = 0;
	}
	for(;j<64;j++){
		ctx->dx11list.world[0].solid[j] = 0;
	}

	//opaque: clear myown, forget other
	for(j=0;j<opaqueaid_max;j++){
		p = ctx->dx11list.world[0].opaque[j];
		if(0 == p)continue;

		p->src.vtx[0].vbuf_h = 0;
		p->src.vtx[0].ibuf_h = 0;
	}
	for(;j<64;j++){
		ctx->dx11list.world[0].opaque[j] = 0;
	}
}
void dx11data_after(_obj* ctx)
{
	int j;
	struct dx11data* p;

	//solid: enqueue
	for(j=0;j<solidaid_max;j++)
	{
		p = ctx->dx11list.world[0].solid[j];
		if(0 == p)continue;

		p->src.vbuf_enq += 1;
		p->src.ibuf_enq += 1;
	}

	//opaque: enqueue
	for(j=0;j<opaqueaid_max;j++)
	{
		p = ctx->dx11list.world[0].opaque[j];
		if(0 == p)continue;

		p->src.vbuf_enq += 1;
		p->src.ibuf_enq += 1;
	}
}
void dx11data_insert(_obj* ctx, int type, struct dx11data* src, int cnt)
{
	int j;
	//say("@dx11data_insert:%llx,%x,%llx,%x\n", ctx,type,src,cnt);

	if('s' == type){
		for(j=solidaid_max;j<64;j++){
			if(0 == ctx->dx11list.world[0].solid[j]){
				ctx->dx11list.world[0].solid[j] = src;
				break;
			}
		}
	}
	if('o' == type){
		for(j=opaqueaid_max;j<64;j++){
			if(0 == ctx->dx11list.world[0].opaque[j]){
				ctx->dx11list.world[0].opaque[j] = src;
				break;
			}
		}
	}
}




//[-4,-3]: ogl,area -> cam,togl
//[-2,-1]: cam,towr -> wor,geom
int dx11data_taking(_obj* world,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	struct relation* rel = world->orel0;
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dstnodetype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].foottype = rel->srcfoottype;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].foottype = rel->dstfoottype;
			entity_takeby(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, arg,key, buf,len);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int dx11data_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int dx11data_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int dx11data_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int dx11data_search(_obj* win)
{
	return 0;
}
int dx11data_modify(_obj* win)
{
	return 0;
}
int dx11data_delete(_obj* win)
{
	return 0;
}
int dx11data_create(_obj* act, void* flag)
{
	act->dx11list.world[0].camera = memoryalloc(0x10000, 0);
	act->dx11list.world[0].light  = memoryalloc(0x10000, 0);
	act->dx11list.world[0].solid  = memoryalloc(0x10000, 0);
	act->dx11list.world[0].opaque = memoryalloc(0x10000, 0);
	return 0;
}
