#include "libuser.h"
#define _fbo_ hex32('f','b','o',0)
#define CTXBUF listptr.buf0
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_01cam(_obj* wnd);
void gl41data_insert(_obj* ctx, int type, void* data, int cnt);




char* teevee_glsl_v =
GLSL_VERSION
GLSL_PRECISION
"layout(location = 0)in vec3 vertex;\n"
"layout(location = 1)in vec2 texuvw;\n"
"out vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* teevee_glsl_f =
GLSL_VERSION
GLSL_PRECISION
"in vec2 uvw;\n"
"out vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"void main(){\n"
	"float dx = uvw.x-0.5;\n"
	"float dy = uvw.y-0.5;\n"
	"float ff = max(0.0, 1.0-(dx*dx+dy*dy)*4);\n"
	"vec3 rgb = ff*texture(tex0, uvw).rgb;\n"
	"FragColor = vec4(rgb, 1.0);\n"
"}\n";




void cbuffer_ctxforwnd(struct mysrc* src)
{
	//shader
	src->vs = teevee_glsl_v;
	src->fs = teevee_glsl_f;
	src->shader_enq = 42;

	//vertex
	struct vertex* vtx = src->vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memoryalloc(vtx->vbuf_len, 0);
	src->vbuf_enq = 42;
}
static void cbuffer_readfrom_cbuffer(_obj* ent, struct gl41data* data)
{
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)return;
		if(_fbo_ == rel->srcfoottype)break;
		rel = samesrcnextdst(rel);
	}
/*
	_obj* sup = rel->pdstchip;
	data->src.tex[0].glfd = sup->gl41list.tex[0];
	data->src.tex[0].fmt = '!';

	data->dst.texname[0] = "tex0";
	data->src.tex_enq[0] += 1;*/
}
static void cbuffer_draw_gl41(
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
	cbuffer_readfrom_cbuffer(act, data);

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




static void cbuffer_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
}
static void cbuffer_modify(_obj* act)
{
}
static void cbuffer_delete(_obj* act)
{
	if(0 == act)return;
}
static void cbuffer_create(_obj* act, void* arg, int argc, u8** argv)
{
	act->CTXBUF = memoryalloc(0x1000, 0);
	cbuffer_ctxforwnd(act->CTXBUF);
}





static void cbuffer_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void cbuffer_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void cbuffer_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void cbuffer_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void cbuffer_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("cbuffer(%x,%x,%x)\n",win,act,sty);
}




static void cbuffer_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	take_data_from_peer(ent,_fbo_, stack,sp, 0,0, 0,0);
	cbuffer_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void cbuffer_read_bywnd(_obj* ent,void* foot, _obj* wnd,void* area, _syn* stack,int sp)
{
	struct fstyle fs;
	switch(wnd->vfmt){
	case _rgba8888_:
		break;
	case _gl41list_:
		fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
		fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
		fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
		gl41data_before(wnd);
		take_data_from_peer(ent,_fbo_, stack,sp, 0,0, 0,0);
		cbuffer_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
		gl41data_01cam(wnd);
		gl41data_after(wnd);
		break;
	}
}




static void cbuffer_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		cbuffer_read_bywnd(ent,foot, caller,area, stack,sp);
		break;
	default:
		cbuffer_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void cbuffer_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//logtoall("@cbuffer_write\n");
	if(_wnd_ == stack[sp-1].foottype){
		//give_data_into_peer(ent,_fbo_, stack,sp, arg,key, buf,len);
	}
}
static void cbuffer_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void cbuffer_attach(struct halfrel* self, struct halfrel* peer)
{
}




void cbuffer_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('c','b','u','f','f','e','r', 0);

	p->oncreate = (void*)cbuffer_create;
	p->ondelete = (void*)cbuffer_delete;
	p->onreader = (void*)cbuffer_search;
	p->onwriter = (void*)cbuffer_modify;

	p->onattach = (void*)cbuffer_attach;
	p->ondetach = (void*)cbuffer_detach;
	p->ontaking = (void*)cbuffer_taking;
	p->ongiving = (void*)cbuffer_giving;
}
