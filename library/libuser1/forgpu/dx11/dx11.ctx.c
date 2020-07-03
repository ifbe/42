#include "libuser.h"




void dx11data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v)
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




void dx11data_whcam(struct entity* wnd, struct fstyle* area)
{
	int x,y;
	void* trick = wnd->dxfull_camera;
	struct dx11data* data = trick + 0x400;
	float (*m)[4] = data->src.arg.mat;
	float* v = data->src.arg.vec;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)m[y][x] = 0.0;
		v[y] = 0.0;
	}
	m[0][0] = 2.0 / (area->vq[0] * wnd->fbwidth);
	m[1][1] = 2.0 / (area->vq[1] * wnd->fbheight);
	m[2][2] =-1.0;
	m[3][3] = 1.0;
	//say("%f,%f\n", m[0][0], m[1][1]);

	wnd->dxfull_camera[0] = data;
}
void dx11data_01cam(struct entity* wnd)
{
	int x,y;
	void* trick = wnd->dxfull_camera;
	struct dx11data* data = trick + 0x400;
	float (*m)[4] = data->src.arg.mat;
	float* v = data->src.arg.vec;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)m[y][x] = 0.0;
		v[y] = 0.0;
	}
	m[0][0] = 1.0;
	m[1][1] = 1.0;
	m[2][2] =-1.0;
	m[3][3] = 1.0;

	wnd->dxfull_camera[0] = data;
}




void dx11data_nolit(struct entity* wnd)
{
}
void dx11data_mylit(struct entity* wnd)
{
}




void dx11data_before(struct entity* ctx)
{
	int j;
	struct dx11data* p;

	//camera: default
	ctx->dxfull_camera[0] = 0;

	//light: default
	//ctx->dxfull_light[0] = 0;
	dx11data_nolit(ctx);

	//solid: clear myown, forget other
	for(j=0;j<solidaid_max;j++){
		p = ctx->dxfull_solid[j];
		if(0 == p)continue;

		p->src.vtx[0].vbuf_h = 0;
		p->src.vtx[0].ibuf_h = 0;
	}
	for(;j<64;j++){
		ctx->dxfull_solid[j] = 0;
	}

	//opaque: clear myown, forget other
	for(j=0;j<opaqueaid_max;j++){
		p = ctx->dxfull_opaque[j];
		if(0 == p)continue;

		p->src.vtx[0].vbuf_h = 0;
		p->src.vtx[0].ibuf_h = 0;
	}
	for(;j<64;j++){
		ctx->dxfull_opaque[j] = 0;
	}
}
void dx11data_after(struct entity* ctx)
{
	int j;
	struct dx11data* p;

	//solid: enqueue
	for(j=0;j<solidaid_max;j++)
	{
		p = ctx->dxfull_solid[j];
		if(0 == p)continue;

		p->src.vbuf_enq += 1;
		p->src.ibuf_enq += 1;
	}

	//opaque: enqueue
	for(j=0;j<opaqueaid_max;j++)
	{
		p = ctx->dxfull_opaque[j];
		if(0 == p)continue;

		p->src.vbuf_enq += 1;
		p->src.ibuf_enq += 1;
	}
}
void dx11data_insert(struct entity* ctx, int type, struct dx11data* src, int cnt)
{
	int j;
	//say("@dx11data_insert:%llx,%x,%llx,%x\n", ctx,type,src,cnt);

	if('s' == type){
		for(j=solidaid_max;j<64;j++){
			if(0 == ctx->dxfull_solid[j]){
				ctx->dxfull_solid[j] = src;
				break;
			}
		}
	}
	if('o' == type){
		for(j=opaqueaid_max;j<64;j++){
			if(0 == ctx->dxfull_opaque[j]){
				ctx->dxfull_opaque[j] = src;
				break;
			}
		}
	}
}




//[-4,-3]: ogl,area -> cam,togl
//[-2,-1]: cam,towr -> wor,geom
int dx11data_taking(_ent* world,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
int dx11data_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int dx11data_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int dx11data_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int dx11data_search(struct entity* win)
{
	return 0;
}
int dx11data_modify(struct entity* win)
{
	return 0;
}
int dx11data_delete(struct entity* win)
{
	return 0;
}
int dx11data_create(struct entity* act, void* flag)
{
	act->dxfull_camera = memorycreate(0x10000, 0);
	act->dxfull_light  = memorycreate(0x10000, 0);
	act->dxfull_solid  = memorycreate(0x10000, 0);
	act->dxfull_opaque = memorycreate(0x10000, 0);
	return 0;
}
