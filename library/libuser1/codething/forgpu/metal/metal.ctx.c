#include "libuser.h"
struct unidata{
	mat4 mat;
	vec4 vec;
};




void mt20data_nocam(_obj* wnd)
{
	void* trick = wnd->mt20list.camera;
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
	m[2][2] = 1.0;
	m[3][3] = 1.0;

	wnd->mt20list.camera[0] = data;
}
void mt20data_01cam(_obj* wnd)
{
	void* trick = wnd->mt20list.camera;
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

	wnd->mt20list.camera[0] = data;
}
void mt20data_whcam(_obj* wnd, struct fstyle* area)
{
	void* trick = wnd->mt20list.camera;
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
	m[0][0] = 2.0 / (area->vq[0] * wnd->whdf.fbwidth);
	m[1][1] = 2.0 / (area->vq[1] * wnd->whdf.fbheight);
	m[2][2] =-1.0;
	m[3][3] = 1.0;
	//say("%f,%f\n", m[0][0], m[1][1]);

	wnd->mt20list.camera[0] = data;
}




void mt20data_nolit(_obj* wnd)
{
}
void mt20data_mylit(_obj* wnd)
{
}




void mt20data_before(_obj* ctx)
{
	int j;
	struct mt20data* p;

	//camera: default
	ctx->mt20list.camera[0] = 0;

	//light: default
	//ctx->mt20list.light[0] = 0;
	mt20data_nolit(ctx);

	//solid: clear myown, forget other
	for(j=0;j<solidaid_max;j++){
		p = ctx->mt20list.solid[j];
		if(0 == p)continue;

		p->src.vtx[0].vbuf_h = 0;
		p->src.vtx[0].ibuf_h = 0;
	}
	for(;j<64;j++){
		ctx->mt20list.solid[j] = 0;
	}

	//opaque: clear myown, forget other
	for(j=0;j<opaqueaid_max;j++){
		p = ctx->mt20list.opaque[j];
		if(0 == p)continue;

		p->src.vtx[0].vbuf_h = 0;
		p->src.vtx[0].ibuf_h = 0;
	}
	for(;j<64;j++){
		ctx->mt20list.opaque[j] = 0;
	}
}
void mt20data_after(_obj* ctx)
{
	int j;
	struct mt20data* p;

	//solid: enqueue
	for(j=0;j<solidaid_max;j++)
	{
		p = ctx->mt20list.solid[j];
		if(0 == p)continue;

		p->src.vbuf_enq += 1;
		p->src.ibuf_enq += 1;
	}

	//opaque: enqueue
	for(j=0;j<opaqueaid_max;j++)
	{
		p = ctx->mt20list.opaque[j];
		if(0 == p)continue;

		p->src.vbuf_enq += 1;
		p->src.ibuf_enq += 1;
	}
}
void mt20data_insert(_obj* ctx, int type, struct mt20data* src, int cnt)
{
	int j;
	//say("@mt20data_insert:%llx,%x,%llx,%x\n", ctx,type,src,cnt);

	if('s' == type){
		for(j=solidaid_max;j<64;j++){
			if(0 == ctx->mt20list.solid[j]){
				ctx->mt20list.solid[j] = src;
				break;
			}
		}
	}
	if('o' == type){
		for(j=opaqueaid_max;j<64;j++){
			if(0 == ctx->mt20list.opaque[j]){
				ctx->mt20list.opaque[j] = src;
				break;
			}
		}
	}
}




//be the world, get all data
//[-4,-3]: ogl,area -> cam,togl
//[-2,-1]: cam,towr -> wor,geom
int mt20data_taking(_obj* world,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
			entity_take(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, arg,key, buf,len);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int mt20data_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int mt20data_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int mt20data_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int mt20data_search(_obj* win)
{
	return 0;
}
int mt20data_modify(_obj* win)
{
	return 0;
}
int mt20data_delete(_obj* win)
{
	return 0;
}
int mt20data_create(_obj* act, void* flag)
{
	act->mt20list.camera = memorycreate(0x10000, 0);
	act->mt20list.light  = memorycreate(0x10000, 0);
	act->mt20list.solid  = memorycreate(0x10000, 0);
	act->mt20list.opaque = memorycreate(0x10000, 0);
	return 0;
}
