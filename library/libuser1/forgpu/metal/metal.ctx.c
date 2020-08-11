#include "libuser.h"
struct unidata{
	mat4 mat;
	vec4 vec;
};




void mt20data_whcam(struct entity* wnd, struct fstyle* area)
{
	void* trick = wnd->mtfull_camera;
	struct mt20data* data = trick + 0x400;
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
	m[0][0] = 2.0 / (area->vq[0] * wnd->fbwidth);
	m[1][1] = 2.0 / (area->vq[1] * wnd->fbheight);
	m[2][2] =-1.0;
	m[3][3] = 1.0;
	//say("%f,%f\n", m[0][0], m[1][1]);

	wnd->mtfull_camera[0] = data;
}
void mt20data_01cam(struct entity* wnd)
{
	void* trick = wnd->mtfull_camera;
	struct mt20data* data = trick + 0x400;
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
	m[0][0] = 1.0;
	m[1][1] = 1.0;
	m[2][2] =-1.0;
	m[3][3] = 1.0;

	wnd->mtfull_camera[0] = data;
}




void mt20data_nolit(struct entity* wnd)
{
}
void mt20data_mylit(struct entity* wnd)
{
}




void mt20data_before(struct entity* ctx)
{
	int j;
	struct mt20data* p;

	//camera: default
	ctx->mtfull_camera[0] = 0;

	//light: default
	//ctx->mtfull_light[0] = 0;
	mt20data_nolit(ctx);

	//solid: clear myown, forget other
	for(j=0;j<solidaid_max;j++){
		p = ctx->mtfull_solid[j];
		if(0 == p)continue;

		p->src.vtx[0].vbuf_h = 0;
		p->src.vtx[0].ibuf_h = 0;
	}
	for(;j<64;j++){
		ctx->mtfull_solid[j] = 0;
	}

	//opaque: clear myown, forget other
	for(j=0;j<opaqueaid_max;j++){
		p = ctx->mtfull_opaque[j];
		if(0 == p)continue;

		p->src.vtx[0].vbuf_h = 0;
		p->src.vtx[0].ibuf_h = 0;
	}
	for(;j<64;j++){
		ctx->mtfull_opaque[j] = 0;
	}
}
void mt20data_after(struct entity* ctx)
{
	int j;
	struct mt20data* p;

	//solid: enqueue
	for(j=0;j<solidaid_max;j++)
	{
		p = ctx->mtfull_solid[j];
		if(0 == p)continue;

		p->src.vbuf_enq += 1;
		p->src.ibuf_enq += 1;
	}

	//opaque: enqueue
	for(j=0;j<opaqueaid_max;j++)
	{
		p = ctx->mtfull_opaque[j];
		if(0 == p)continue;

		p->src.vbuf_enq += 1;
		p->src.ibuf_enq += 1;
	}
}
void mt20data_insert(struct entity* ctx, int type, struct mt20data* src, int cnt)
{
	int j;
	//say("@mt20data_insert:%llx,%x,%llx,%x\n", ctx,type,src,cnt);

	if('s' == type){
		for(j=solidaid_max;j<64;j++){
			if(0 == ctx->mtfull_solid[j]){
				ctx->mtfull_solid[j] = src;
				break;
			}
		}
	}
	if('o' == type){
		for(j=opaqueaid_max;j<64;j++){
			if(0 == ctx->mtfull_opaque[j]){
				ctx->mtfull_opaque[j] = src;
				break;
			}
		}
	}
}




//[-4,-3]: ogl,area -> cam,togl
//[-2,-1]: cam,towr -> wor,geom
int mt20data_taking(_ent* world,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
int mt20data_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int mt20data_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int mt20data_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int mt20data_search(struct entity* win)
{
	return 0;
}
int mt20data_modify(struct entity* win)
{
	return 0;
}
int mt20data_delete(struct entity* win)
{
	return 0;
}
int mt20data_create(struct entity* act, void* flag)
{
	act->mtfull_camera = memorycreate(0x10000, 0);
	act->mtfull_light  = memorycreate(0x10000, 0);
	act->mtfull_solid  = memorycreate(0x10000, 0);
	act->mtfull_opaque = memorycreate(0x10000, 0);
	return 0;
}
