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




void gl41data_01cam(struct entity* wnd)
{
	int x,y;
	void* trick = wnd->glfull_camera;
	struct gl41data* data = trick + 0x400;
	float (*m)[4] = trick + 0x800;
	float* v = trick + 0xc00;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)m[y][x] = 0.0;
		v[y] = 0.0;
	}
	m[0][0] = 1.0;
	m[1][1] = 1.0;
	m[2][2] =-0.5;
	m[2][3] = 0.5;
	m[3][3] = 1.0;

	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = m;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = v;
	wnd->glfull_camera[0] = data;
}
void gl41data_whcam(struct entity* wnd, struct fstyle* area)
{
	int x,y;
	void* trick = wnd->glfull_camera;
	struct gl41data* data = trick + 0x400;
	float (*m)[4] = trick + 0x800;
	float* v = trick + 0xc00;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)m[y][x] = 0.0;
		v[y] = 0.0;
	}
	m[0][0] = 2.0 / (area->vq[0] * wnd->fbwidth);
	m[1][1] = 2.0 / (area->vq[1] * wnd->fbheight);
	m[2][2] =-0.5;
	m[2][3] = 0.5;
	m[3][3] = 1.0;
	//say("%f,%f\n", m[0][0], m[1][1]);

	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = m;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = v;
	wnd->glfull_camera[0] = data;
}
void gl41data_addcam(struct entity* wnd, struct gl41data* data)
{
	int j;
	for(j=1;j<8;j++){
		if(0 == wnd->glfull_camera[j]){
			wnd->glfull_camera[j] = data;
			break;
		}
	}
}




void gl41data_nolit(struct entity* wnd)
{
	void* trick = wnd->glfull_light;
	struct gl41data* data = trick + 0x400;

	data->dst.routine_name = "passtype";
	data->dst.routine_detail = "pbrcolor";

	wnd->glfull_light[0] = (void*)data;
}
void gl41data_mylit(struct entity* wnd)
{
}
void gl41data_addlit(struct entity* wnd, struct gl41data* data)
{
	wnd->glfull_light[0] = data;
}




void gl41data_insert(struct entity* ctx, int type, struct gl41data* src, int cnt)
{
	int j;
	//say("@gl41data_insert:%llx,%x,%llx,%x\n", ctx,type,src,cnt);

	if('s' == type){
		for(j=solidaid_max;j<64;j++){
			if(0 == ctx->glfull_solid[j]){
				ctx->glfull_solid[j] = src;
				break;
			}
		}
	}
	if('o' == type){
		for(j=opaqueaid_max;j<64;j++){
			if(0 == ctx->glfull_opaque[j]){
				ctx->glfull_opaque[j] = src;
				break;
			}
		}
	}
}




void gl41data_before(struct entity* ctx)
{
	int j;
	struct gl41data* p;

	//camera: default
	for(j=0;j<16;j++){
		ctx->glfull_camera[j] = 0;
	}

	//light: default
	for(j=0;j<16;j++){
		ctx->glfull_light[j] = 0;
	}
	gl41data_nolit(ctx);

	//solid: clear myown, forget other
	for(j=0;j<solidaid_max;j++){
		p = ctx->glfull_solid[j];
		if(0 == p)continue;

		p->src.vtx[0].vbuf_h = 0;
		p->src.vtx[0].ibuf_h = 0;
	}
	for(;j<64;j++){
		ctx->glfull_solid[j] = 0;
	}

	//opaque: clear myown, forget other
	for(j=0;j<opaqueaid_max;j++){
		p = ctx->glfull_opaque[j];
		if(0 == p)continue;

		p->src.vtx[0].vbuf_h = 0;
		p->src.vtx[0].ibuf_h = 0;
	}
	for(;j<64;j++){
		ctx->glfull_opaque[j] = 0;
	}
}
void gl41data_after(struct entity* ctx)
{
	int j;
	struct gl41data* p;

	//solid: enqueue
	for(j=0;j<solidaid_max;j++)
	{
		p = ctx->glfull_solid[j];
		if(0 == p)continue;

		p->src.vbuf_enq += 1;
		p->src.ibuf_enq += 1;
	}

	//opaque: enqueue
	for(j=0;j<opaqueaid_max;j++)
	{
		p = ctx->glfull_opaque[j];
		if(0 == p)continue;

		p->src.vbuf_enq += 1;
		p->src.ibuf_enq += 1;
	}
}




//[-4,-3]: ogl,area -> cam,togl
//[-2,-1]: cam,towr -> wor,geom
int gl41data_taking(_ent* world,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
			entity_take(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, arg,key, buf,len);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int gl41data_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
	act->glfull_camera = memorycreate(0x10000, 0);
	act->glfull_light  = memorycreate(0x10000, 0);
	act->glfull_solid  = memorycreate(0x10000, 0);
	act->glfull_opaque = memorycreate(0x10000, 0);
	return 0;
}
