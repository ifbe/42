#include "libuser.h"
void carveskybox(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41data_insert(struct entity* ctx, int type, struct mysrc* src, int cnt);




char* texbox_glsl_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec2 texuvw;\n"
	"uniform mat4 cammvp;\n"
	"out mediump vec2 uvw;\n"
	"void main()\n"
	"{\n"
		"uvw = texuvw;\n"
		"gl_Position = cammvp * vec4(vertex, 1.0);\n"
	"}\n";
char* texbox_glsl_t = 0;
char* texbox_glsl_g = 0;
char* texbox_glsl_f = 
	GLSL_VERSION
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"uniform sampler2D tex0;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
		//"FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n";



static void texbox_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texbox_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* none)
{
	struct mysrc* src = act->buf0;
	if(0 == src)return;

	void* vbuf = src->vtx[0].vbuf;
	void* ibuf = src->vtx[0].ibuf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	carveskybox(vbuf, ibuf, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;

	gl41data_insert(ctx, 's', src, 1);
}
static void texbox_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texbox_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texbox_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texbox_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("texbox(%x,%x,%x)\n",win,act,sty);
}
static void texbox_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
}




static void texbox_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	texbox_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void texbox_wrl_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
}
static void texbox_wnd(_ent* ent,void* slot, _ent* wnd,void* area)
{
}




static void texbox_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41list_:
		texbox_wnd(ent,slot, caller,area);
		break;
	default:
		texbox_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void texbox_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void texbox_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void texbox_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void texbox_search(struct entity* act)
{
}
static void texbox_modify(struct entity* act)
{
}
static void texbox_delete(struct entity* act)
{
	if(0 == act)return;
	if(0 == act->buf0){
		memorydelete(act->buf0);
		act->buf0 = 0;
	}
}
static void texbox_create(struct entity* act, void* str)
{
	if(0 == act)return;

	struct gl41data* data = act->buf0 = memorycreate(0x1000, 0);
	if(0 == data)return;

	//shader
	data->src.vs = texbox_glsl_v;
	data->src.fs = texbox_glsl_f;
	data->src.shader_enq = 42;

	//texture
	data->src.tex[0].fmt = hex32('r','g','b','a');
	data->src.tex[0].data = memorycreate(2048*2048*4, 0);
	if(0 == str)str = "datafile/jpg/earth.jpg";
	loadtexfromfile(&data->src.tex[0], str);

	data->dst.texname[0] = "tex0";
	data->src.tex_enq[0] = 42;
	//say("w=%d,h=%d\n",data->src.tex[0].w, data->src.tex[0].h);

	struct vertex* vtx = data->src.vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 4*6;
	vtx->vbuf_h = 24;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);
	data->src.vbuf_enq = 0;

	vtx->ibuf_fmt = 0x222;
	vtx->ibuf_w = 2*3;
	vtx->ibuf_h = 36;
	vtx->ibuf_len = (vtx->ibuf_w) * (vtx->ibuf_h);
	vtx->ibuf = memorycreate(vtx->ibuf_len, 0);
	data->src.ibuf_enq = 0;
}




void texbox_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'e', 'x', 'b', 'o', 'x', 0, 0);

	p->oncreate = (void*)texbox_create;
	p->ondelete = (void*)texbox_delete;
	p->onsearch = (void*)texbox_search;
	p->onmodify = (void*)texbox_modify;

	p->onlinkup = (void*)texbox_linkup;
	p->ondiscon = (void*)texbox_discon;
	p->ontaking = (void*)texbox_taking;
	p->ongiving = (void*)texbox_giving;
}
