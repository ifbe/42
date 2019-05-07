#include "libuser.h"




char* ground_glsl2d_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec2 t;\n"
"out mediump vec2 texuvw;\n"
"void main(){\n"
	"texuvw = t;\n"
	"gl_Position = vec4(v, 1.0);\n"
"}\n";

char* ground_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec2 t;\n"
"out mediump vec3 vertex;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"vertex = v;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char* ground_glsl_f =
GLSL_VERSION
"in mediump vec3 vertex;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"void main(){\n"
	"mediump float x = mod(abs(vertex.x), 1000.0) - 500.0;\n"
	"mediump float y = mod(abs(vertex.y), 1000.0) - 500.0;\n"
	"if(x>-490.0 && x<490.0 && y>-490.0 && y<490.0)discard;\n"
	"FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
	//"mediump vec3 c;\n"
	//"if(x * y > 0.0)c = vec3(0.5, 0.5, 0.5);\n"
	//"else c = vec3(0.75, 0.75, 0.75);\n"
	//"FragColor = vec4(c, 1.0);\n"
"}\n";




static void ground_draw_pixel(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void ground_draw_vbo(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

	struct glsrc* src = (void*)(pin->foot[0]);
	float (*vbuf)[6] = (void*)(src->vbuf);
	//carvesolid_rect(win, 0xffffff, vc, vr, vf);

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2] - 1.0;
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2] - 1.0;
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2] - 1.0;
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2] - 1.0;
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2] - 1.0;
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2] - 1.0;
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
}
static void ground_draw_json(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void ground_draw_html(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void ground_draw_tui(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void ground_draw_cli(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void ground_draw(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)ground_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)ground_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)ground_draw_html(act, pin, win, sty);
	else if(fmt == _json_)ground_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)ground_draw_vbo(act, pin, win, sty);
	else ground_draw_pixel(act, pin, win, sty);
}




static void ground_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct pinid* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	ground_draw(act, pin, win, sty);
}
static void ground_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void ground_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void ground_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void ground_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void ground_start(struct halfrel* self, struct halfrel* peer)
{
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	struct actor* act = (void*)(self->chip);
	struct pinid* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);

	//
	pair = alloc_winobj(win, 's');
	src = &pair->src;
	dst = &pair->dst;
	pin->foot[0] = (u64)src;
	sty->foot[0] = (u64)dst;

	//
	src->geometry = 3;
	src->method = 'v';

	//
	src->vs = ground_glsl_v;
	src->fs = ground_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len);
}
static void ground_delete(struct actor* act)
{
}
static void ground_create(struct actor* act, void* str)
{
}




void ground_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('g', 'r', 'o', 'u', 'n', 'd', 0, 0);

	p->oncreate = (void*)ground_create;
	p->ondelete = (void*)ground_delete;
	p->onstart  = (void*)ground_start;
	p->onstop   = (void*)ground_stop;
	p->oncread  = (void*)ground_cread;
	p->oncwrite = (void*)ground_cwrite;
	p->onsread  = (void*)ground_sread;
	p->onswrite = (void*)ground_swrite;
}
