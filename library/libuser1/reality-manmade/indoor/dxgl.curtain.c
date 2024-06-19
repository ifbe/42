#include "libuser.h"




char* curtain_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec2 n;\n"
"out mediump vec3 vertex;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"vertex = v;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* curtain_glsl_f =
GLSL_VERSION
"in mediump vec3 vertex;\n"
"layout(location = 0)out mediump vec4 FragColor;\n"
"uniform sampler2D shadowmap;\n"
"uniform sampler2D prjtormap;\n"
"uniform mat4 sunmvp;\n"
"void main(){\n"
	"mediump vec4 tmp = sunmvp * vec4(vertex, 1.0);\n"
	"tmp /= tmp.w;\n"
	"tmp = (tmp+1.0)*0.5;\n"
	//"FragColor = vec4(tmp.x, tmp.y, tmp.z, 1.0);\n"
	"mediump float visiable = 1.0;\n"
	"if(tmp.z - texture(shadowmap, tmp.xy).r > 0.0001)visiable = 0.0;\n"
	"FragColor = vec4(visiable * texture(prjtormap, tmp.xy).bgr, 1.0);\n"
"}\n";




static void curtain_search(_obj* act)
{
}
static void curtain_modify(_obj* act)
{
}
static void curtain_delete(_obj* act)
{
}
static void curtain_create(_obj* act, void* str)
{
	struct mysrc* src = act->listptr.buf0 = memoryalloc(0x1000, 0);
	if(0 == src)return;

	//shader
	src->vs = curtain_glsl_v;
	src->fs = curtain_glsl_f;
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
}




static void curtain_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void curtain_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	struct mysrc* src = act->listptr.buf0;
	if(0 == src)return;
	float (*vbuf)[6] = src->vtx[0].vbuf;
	if(0 == vbuf)return;

	//depth fbo (for debug)
	vbuf[0][0] = vc[0] - vr[0] - vt[0];
	vbuf[0][1] = vc[1] - vr[1] - vt[1];
	vbuf[0][2] = vc[2] - vr[2] - vt[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vt[0];
	vbuf[1][1] = vc[1] + vr[1] + vt[1];
	vbuf[1][2] = vc[2] + vr[2] + vt[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vt[0];
	vbuf[2][1] = vc[1] - vr[1] + vt[1];
	vbuf[2][2] = vc[2] - vr[2] + vt[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vt[0];
	vbuf[3][1] = vc[1] + vr[1] + vt[1];
	vbuf[3][2] = vc[2] + vr[2] + vt[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vt[0];
	vbuf[4][1] = vc[1] - vr[1] - vt[1];
	vbuf[4][2] = vc[2] - vr[2] - vt[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vt[0];
	vbuf[5][1] = vc[1] + vr[1] - vt[1];
	vbuf[5][2] = vc[2] + vr[2] - vt[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
}
static void curtain_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void curtain_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void curtain_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void curtain_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void curtain_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	curtain_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void curtain_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		break;
	default:
		curtain_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void curtain_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void curtain_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void curtain_attach(struct halfrel* self, struct halfrel* peer)
{
}




void curtain_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('c', 'u', 'r', 't', 'a', 'i', 'n', 0);

	p->oncreate = (void*)curtain_create;
	p->ondelete = (void*)curtain_delete;
	p->onreader = (void*)curtain_search;
	p->onwriter = (void*)curtain_modify;

	p->onattach = (void*)curtain_attach;
	p->ondetach = (void*)curtain_detach;
	p->ontaking = (void*)curtain_taking;
	p->ongiving = (void*)curtain_giving;
}
