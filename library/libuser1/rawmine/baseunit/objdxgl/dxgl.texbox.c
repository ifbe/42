#include "libuser.h"
void loadtexfromfile(struct glsrc* src, int idx, char* name);
void carveskybox(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);




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
static void texbox_draw_vbo3d(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* ctx, struct style* none)
{
	void* vbuf;
	void* ibuf;
	struct glsrc* src;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;

	src = act->buf;
	if(0 == src)return;

	vbuf = (void*)(src->vbuf);
	ibuf = (void*)(src->ibuf);
	carveskybox(vbuf, ibuf, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;
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
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"texbox\" style=\"width:50%%;height:100px;float:left;background-color:#3368a9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
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
static void texbox_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)texbox_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)texbox_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)texbox_draw_html(act, pin, win, sty);
	else if(fmt == _json_)texbox_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)texbox_draw_vbo2d(act, pin, win, sty);
		//else texbox_draw_vbo3d(act, pin, win, sty);
	}
	else texbox_draw_pixel(act, pin, win, sty);
}
static void texbox_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
}




//stack:
//-4: wnd, area
//-3: ctx
//-2: cam, part of cam
//-1: world, geom of cam
static void texbox_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> ctx
	struct entity* ctx;

	//cam -> world
	struct entity* cam;
	struct entity* wrd;struct style* camg;

	//world -> texbox
	struct entity* win;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		ctx = stack[rsp-3]->pchip;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		win = peer->pchip;geom = peer->pfoot;
		act = self->pchip;part = self->pfoot;
		texbox_draw_vbo3d(act,part, win,geom, wrd,camg, ctx,0);
	}
}
static void texbox_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//texbox_event(act, pin, win, sty, ev, 0);
}
static void texbox_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void texbox_start(struct halfrel* self, struct halfrel* peer)
{
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	if(0 == act)return;
	if(0 == pin)return;

	pin->data[0] = (u64)(act->buf);
	say("@texbox_start:%llx, %llx\n", pin->data[0], pin->data[1]);
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
	if(0 == act->buf){
		memorydelete(act->buf);
		act->buf = 0;
	}
}
static void texbox_create(struct entity* act, void* str)
{
	int j;
	struct glsrc* src;
	if(0 == act)return;

	src = act->buf = memorycreate(0x200, 0);
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
	loadtexfromfile(src, 0, str);
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

	p->onstart = (void*)texbox_start;
	p->onstop  = (void*)texbox_stop;
	p->onread  = (void*)texbox_read;
	p->onwrite = (void*)texbox_write;
}
