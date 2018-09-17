#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void* allocofoot();
void actorcreatefromfile(struct actor* act, char* name);




char* ocean_glsl_v =
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
char* ocean_glsl_f =
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
	"}\n";




static void ocean_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
static void ocean_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float (*vbuf)[6];
	struct glsrc* src;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	if(0 == act->buf)return;

	src = (void*)(pin->foot[0]);
	vbuf = (void*)(src->vbuf);

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 1.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 0.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 0.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 0.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 1.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 1.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
}
static void ocean_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void ocean_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void ocean_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void ocean_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void ocean_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)ocean_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)ocean_read_tui(win, sty, act, pin);
	else if(fmt == _html_)ocean_read_html(win, sty, act, pin);
	else if(fmt == _json_)ocean_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)ocean_read_vbo(win, sty, act, pin);
	else ocean_read_pixel(win, sty, act, pin);
}
static void ocean_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void ocean_list()
{
}
static void ocean_change()
{
}
static void ocean_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void ocean_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	struct glsrc* src;
	if(0 == pin)return;

	//
	src = allocofoot();

	//shader
	src->vs = ocean_glsl_v;
	src->fs = ocean_glsl_f;

	//texture
	src->tex[0] = act->buf;
	src->tex_fmt[0] = hex32('r','g','b','a');
	src->tex_w[0] = act->width;
	src->tex_h[0] = act->height;

	//vertex
	src->vbuf = memorycreate(4*6*6);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->method = 'v';

	//send!
	src->shader_enq[0] = 42;
	src->arg_enq[0] = 0;
	src->tex_enq[0] = 42;
	src->vbuf_enq = 0;
	src->ibuf_enq = 0;

	pin->foot[0] = (u64)src;
}
static void ocean_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void ocean_create(struct actor* act)
{
	if(0 == act)return;
	actorcreatefromfile(act, "jpg/ocean.jpg");
}




void ocean_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('o', 'c', 'e', 'a', 'n', 0, 0, 0);

	p->oncreate = (void*)ocean_create;
	p->ondelete = (void*)ocean_delete;
	p->onstart  = (void*)ocean_start;
	p->onstop   = (void*)ocean_stop;
	p->onlist   = (void*)ocean_list;
	p->onchoose = (void*)ocean_change;
	p->onread   = (void*)ocean_read;
	p->onwrite  = (void*)ocean_write;
}
