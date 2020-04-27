#include "libuser.h"
void carveskybox(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);




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
	void* vbuf;
	void* ibuf;
	struct glsrc* src;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;

	src = act->buf0;
	if(0 == src)return;

	vbuf = (void*)(src->vbuf);
	ibuf = (void*)(src->ibuf);
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




static void texbox_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		texbox_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void texbox_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct supply* sup = stack[sp-2].pchip;
	switch(sup->fmt){
	default:{
		texbox_read_bycam(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	}
}
static void texbox_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
	int j;
	struct glsrc* src;
	if(0 == act)return;

	src = act->buf0 = memorycreate(0x200, 0);
	if(0 == src)return;

	//
	src->geometry = 3;
	src->method = 'i';

	//shader
	src->vs = texbox_glsl_v;
	src->fs = texbox_glsl_f;
	src->shader_enq = 42;

	//texture
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].name = "tex0";
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	if(0 == str)str = "datafile/jpg/earth.jpg";
	loadtexfromfile(&src->tex[0], str);
	src->tex[0].enq = 42;
	//say("w=%d,h=%d\n",src->tex[0].w, src->tex[0].h);

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*6;
	src->vbuf_h = 24;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
	src->vbuf_enq = 0;

	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 36;
	src->ibuf_len = (src->ibuf_w) * (src->ibuf_h);
	src->ibuf = memorycreate(src->ibuf_len, 0);
	src->ibuf_enq = 0;
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
	p->onread  = (void*)texbox_read;
	p->onwrite = (void*)texbox_write;
}
