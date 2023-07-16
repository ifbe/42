#include "libuser.h"
#define _fbog_ hex32('f','b','o','g')
#define CTXBUF listptr.buf0
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_01cam(_obj* wnd);
void gl41data_insert(_obj* ctx, int type, void* src, int cnt);
//
int copypath(u8* path, u8* data);




void gbuffer_ctxforwnd(struct mysrc* src, char* vs, char* fs)
{
	//shader
	src->vs = memorycreate(0x1000, 0);
	loadglslfromfile(vs, 0, src->vs, 0x1000);
	src->fs = memorycreate(0x1000, 0);
	loadglslfromfile(fs, 0, src->fs, 0x1000);
	src->shader_enq = 42;

	//vertex
	struct vertex* vtx = src->vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);
	src->vbuf_enq = 42;
}
static void gbuffer_readfrom_gbuffer(_obj* ent, struct gl41data* data)
{
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)return;
		if(_fbog_ == rel->srcfoottype)break;
		rel = samesrcnextdst(rel);
	}

	_obj* fbo = rel->pdstchip;

	data->dst.texname[0] = "tex0";
	data->dst.texname[1] = "tex1";
	data->dst.texname[2] = "tex2";
	data->dst.texname[3] = "tex3";
	data->dst.texname[4] = "tex4";
/*
	data->src.tex[0].glfd = fbo->gl41list.tex[0];
	data->src.tex[0].fmt = '!';
	data->src.tex_enq[0] += 1;

	data->src.tex[1].glfd = fbo->gl41list.tex[1];
	data->src.tex[1].fmt = '!';
	data->src.tex_enq[1] += 1;

	data->src.tex[2].glfd = fbo->gl41list.tex[2];
	data->src.tex[2].fmt = '!';
	data->src.tex_enq[2] += 1;

	data->src.tex[3].glfd = fbo->gl41list.tex[3];
	data->src.tex[3].fmt = '!';
	data->src.tex_enq[3] += 1;

	data->src.tex[4].glfd = fbo->gl41list.dep;
	data->src.tex[4].fmt = '!';
	data->src.tex_enq[4] += 1;
	//say("%d,%d,%d,%d\n", data->src.tex[0].glfd, data->src.tex[1].glfd, data->src.tex[2].glfd, data->src.tex[3].glfd);
*/
}
static void gbuffer_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	if(0 == act->CTXBUF)return;

	struct gl41data* data = act->CTXBUF;
	if(0 == data)return;
	float (*vbuf)[6] = data->src.vtx[0].vbuf;
	if(0 == vbuf)return;
	gbuffer_readfrom_gbuffer(act, data);

	float x,y;
	float x0,y0,xn,yn;
	x = ctx->whdf.width * (area->fs.vq[0] - area->fs.vc[0]);
	y = ctx->whdf.height* (area->fs.vq[1] - area->fs.vc[1]);
	if(x > y){
		x0 = 0.0;
		xn = 1.0;
		y0 = 0.5 - y/x/2;
		yn = 0.5 + y/x/2;
	}
	else{
		x0 = 0.5 - x/y/2;
		xn = 0.5 + x/y/2;
		y0 = 0.0;
		yn = 1.0;
	}

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = x0;
	vbuf[0][4] = y0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = xn;
	vbuf[1][4] = yn;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = x0;
	vbuf[2][4] = yn;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = xn;
	vbuf[3][4] = yn;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = x0;
	vbuf[4][4] = y0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = xn;
	vbuf[5][4] = y0;
	vbuf[5][5] = 0.0;

	data->src.vbuf_enq += 1;
	gl41data_insert(ctx, 's', data, 1);
}




static int gbuffer_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	_obj* world;

	rel = act->irel0;
	while(1){
		if(0 == rel)break;
		world = (void*)(rel->srcchip);
		if(_virtual_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return 1;
		}
		rel = samedstnextsrc(rel);
	}
	return 0;
}
static void gbuffer_modify(_obj* act)
{
}
static void gbuffer_delete(_obj* act)
{
	if(0 == act)return;
}
static void gbuffer_create(_obj* act, void* arg, int argc, u8** argv)
{
	int j;
	u8 vspath[128];
	u8 fspath[128];
	char* vs = 0;
	char* fs = 0;
	if(0 == act)return;

	for(j=0;j<argc;j++){
		//say("%d:%.8s\n", j, argv[j]);
		if(0 == ncmp(argv[j], "vs:", 3)){
			copypath(vspath, argv[j]+3);
			vs = (void*)vspath;
		}
		if(0 == ncmp(argv[j], "fs:", 3)){
			copypath(fspath, argv[j]+3);
			fs = (void*)fspath;
		}
	}
	if(0 == vs)vs = "datafile/shader/deferred/vert.glsl";
	if(0 == fs)fs = "datafile/shader/deferred/frag.glsl";

	act->CTXBUF = memorycreate(0x1000, 0);
	gbuffer_ctxforwnd(act->CTXBUF, vs, fs);
}





static void gbuffer_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void gbuffer_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void gbuffer_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void gbuffer_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void gbuffer_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("gbuffer(%x,%x,%x)\n",win,act,sty);
}




static void gbuffer_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	take_data_from_peer(ent,_fbog_, stack,sp, 0,0, 0,0);
	gbuffer_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void gbuffer_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	gl41data_before(wnd);
	take_data_from_peer(ent,_fbog_, stack,sp, 0,0, 0,0);
	gbuffer_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_01cam(wnd);
	gl41data_after(wnd);
}




static void gbuffer_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		gbuffer_wnd(ent,slot, stack,sp);
		break;
	default:
		gbuffer_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void gbuffer_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//say("@gbuffer_write\n");
	if(_wnd_ == stack[sp-1].foottype){
		give_data_into_peer(ent,_fbog_, stack,sp, arg,key, buf,len);
	}
}
static void gbuffer_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void gbuffer_attach(struct halfrel* self, struct halfrel* peer)
{
}




void gbuffer_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('g','b','u','f','f','e','r', 0);

	p->oncreate = (void*)gbuffer_create;
	p->ondelete = (void*)gbuffer_delete;
	p->onreader = (void*)gbuffer_search;
	p->onwriter = (void*)gbuffer_modify;

	p->onattach = (void*)gbuffer_attach;
	p->ondetach = (void*)gbuffer_detach;
	p->ontaking = (void*)gbuffer_taking;
	p->ongiving = (void*)gbuffer_giving;
}
