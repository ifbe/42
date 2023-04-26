#include "libuser.h"




void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v)
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




void gl41data_nocam(_obj* wnd)
{//left,bot: (-1,-1), right,top: (+1,+1)
	int x,y;
	void* trick = wnd->gl41list.world[0].camera;
	struct gl41data* data = trick + 0x400;
	float (*m)[4] = trick + 0x800;
	float* v = trick + 0xc00;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)m[y][x] = 0.0;
	}
	m[0][0] = 1.0;
	m[1][1] = 1.0;
	m[2][2] = 1.0;
	m[3][3] = 1.0;
	v[0] = 0.0;
	v[1] = 0.0;
	v[2] =-1.0;

	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = m;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = v;
	wnd->gl41list.world[0].camera[0] = data;
}
void gl41data_01cam(_obj* wnd)
{//left,bot: (0,0), right,top: (1,1)
	int x,y;
	void* trick = wnd->gl41list.world[0].camera;
	struct gl41data* data = trick + 0x400;
	float (*m)[4] = trick + 0x800;
	float* v = trick + 0xc00;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)m[y][x] = 0.0;
		v[y] = 0.0;
	}
	m[0][0] = 2.0;
	m[0][3] =-1.0;
	m[1][1] = 2.0;
	m[1][3] =-1.0;
	m[2][2] = 1.0;
	m[3][3] = 1.0;
	mat4_transpose(m);

	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = m;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = v;
	wnd->gl41list.world[0].camera[0] = data;
}
void gl41data_whcam(_obj* wnd, struct fstyle* area)
{//left,bot: (-w,-h), right,top: (w,h)
	int x,y;
	void* trick = wnd->gl41list.world[0].camera;
	struct gl41data* data = trick + 0x400;
	float (*m)[4] = trick + 0x800;
	float* v = trick + 0xc00;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++)m[y][x] = 0.0;
		v[y] = 0.0;
	}
	m[0][0] = 2.0 / (area->vq[0] * wnd->whdf.fbwidth);
	m[1][1] = 2.0 / (area->vq[1] * wnd->whdf.fbheight);
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
	wnd->gl41list.world[0].camera[0] = data;
}
void gl41data_addcam(_obj* wnd, struct gl41data* data)
{
	int j;
	for(j=1;j<8;j++){
		if(0 == wnd->gl41list.world[0].camera[j]){
			wnd->gl41list.world[0].camera[j] = data;
			break;
		}
	}
}




void gl41data_nolit(_obj* wnd)
{
	void* trick = wnd->gl41list.world[0].light;
	struct gl41data* data = trick + 0x400;

	data->dst.lighttype = _pbrcolor_;

	wnd->gl41list.world[0].light[0] = (void*)data;
}
void gl41data_mylit(_obj* wnd)
{
}
void gl41data_addlit(_obj* wnd, struct gl41data* data)
{
	wnd->gl41list.world[0].light[0] = data;
}




void gl41data_insert(_obj* ctx, int type, struct gl41data* src, int cnt)
{
	int j;
	//say("@gl41data_insert:%llx,%x,%llx,%x\n", ctx,type,src,cnt);

	if('s' == type){
		for(j=solidaid_max;j<64;j++){
			if(0 == ctx->gl41list.world[0].solid[j]){
				ctx->gl41list.world[0].solid[j] = src;
				break;
			}
		}
	}
	if('o' == type){
		for(j=opaqueaid_max;j<64;j++){
			if(0 == ctx->gl41list.world[0].opaque[j]){
				ctx->gl41list.world[0].opaque[j] = src;
				break;
			}
		}
	}
}




void gl41data_before(_obj* ctx)
{
	int j;
	struct gl41data* p;

	//camera: default
	for(j=0;j<16;j++){
		ctx->gl41list.world[0].camera[j] = 0;
	}

	//light: default
	for(j=0;j<16;j++){
		ctx->gl41list.world[0].light[j] = 0;
	}
	gl41data_nolit(ctx);

	//solid: clear myown, forget other
	for(j=0;j<solidaid_max;j++){
		p = ctx->gl41list.world[0].solid[j];
		if(0 == p)continue;

		p->src.vtx[0].vbuf_h = 0;
		p->src.vtx[0].ibuf_h = 0;
	}
	for(;j<64;j++){
		ctx->gl41list.world[0].solid[j] = 0;
	}

	//opaque: clear myown, forget other
	for(j=0;j<opaqueaid_max;j++){
		p = ctx->gl41list.world[0].opaque[j];
		if(0 == p)continue;

		p->src.vtx[0].vbuf_h = 0;
		p->src.vtx[0].ibuf_h = 0;
	}
	for(;j<64;j++){
		ctx->gl41list.world[0].opaque[j] = 0;
	}
}
void gl41data_after(_obj* ctx)
{
	int j;
	struct gl41data* p;

	//solid: enqueue
	for(j=0;j<solidaid_max;j++)
	{
		p = ctx->gl41list.world[0].solid[j];
		if(0 == p)continue;

		p->src.vbuf_enq += 1;
		p->src.ibuf_enq += 1;
	}

	//opaque: enqueue
	for(j=0;j<opaqueaid_max;j++)
	{
		p = ctx->gl41list.world[0].opaque[j];
		if(0 == p)continue;

		p->src.vbuf_enq += 1;
		p->src.ibuf_enq += 1;
	}
}




//[-4,-3]: ogl,area -> cam,togl
//[-2,-1]: cam,towr -> wor,geom
int gl41data_taking(_obj* world,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
int gl41data_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int gl41data_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41data_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int gl41data_search(_obj* win)
{
	return 0;
}
int gl41data_modify(_obj* win)
{
	return 0;
}
int gl41data_delete(_obj* win)
{
	return 0;
}
int gl41data_create(_obj* act, void* flag)
{
	act->gl41list.world[0].camera = memorycreate(0x10000, 0);
	act->gl41list.world[0].light  = memorycreate(0x10000, 0);
	act->gl41list.world[0].solid  = memorycreate(0x10000, 0);
	act->gl41list.world[0].opaque = memorycreate(0x10000, 0);
	return 0;
}
