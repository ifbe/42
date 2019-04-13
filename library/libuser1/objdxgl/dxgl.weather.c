#include "libuser.h"




char* weather_glsl2d_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec2 texuvw;\n"
	"out mediump vec2 uvw;\n"
	"void main()\n"
	"{\n"
		"uvw = texuvw;\n"
		"gl_Position = vec4(vertex, 1.0);\n"
	"}\n";
char* weather_glsl_v =
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
char* weather_glsl_f =
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"mediump float k = uvw.x*uvw.x+uvw.y*uvw.y;\n"
		"FragColor = vec4(vec3(1.0,1.0,1.0)*k, 1.0);\n"
		//"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
		//"FragColor = vec4(uvw, 1.0, 1.0);\n"
	"}\n";




static void weather_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void weather_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	struct glsrc* src = (void*)(pin->foot[0]);
	float (*vbuf)[6] = src->vbuf;

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = -1.0;
	vbuf[0][4] = -1.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = -1.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = -1.0;
	vbuf[4][4] = -1.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = -1.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
}
static void weather_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,z;
	vec3 tc;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

	for(z=-16;z<=16;z++)
	{
		for(y=-16;y<=16;y++)
		{
			for(x=-16;x<=16;x++)
			{
				tc[0] = vc[0] + vr[0]*x/16 + vf[0]*y/16 + vu[0]*z/16;
				tc[1] = vc[1] + vr[1]*x/16 + vf[1]*y/16 + vu[1]*z/16;
				tc[2] = vc[2] + vr[2]*x/16 + vf[2]*y/16 + vu[2]*z/16;
				carvepoint(win, 0xffffff, tc);
			}
		}
	}
/*
	struct glsrc* src = (void*)(pin->foot[0]);
	float (*vbuf)[6] = src->vbuf;

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = -1.0;
	vbuf[0][4] = -1.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = -1.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = -1.0;
	vbuf[4][4] = -1.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = -1.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
*/
}
static void weather_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void weather_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void weather_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void weather_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("weather(%x,%x,%x)\n",win,act,sty);
}
static void weather_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)weather_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)weather_read_tui(win, sty, act, pin);
	else if(fmt == _html_)weather_read_html(win, sty, act, pin);
	else if(fmt == _json_)weather_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)weather_read_vbo2d(win, sty, act, pin);
		else weather_read_vbo3d(win, sty, act, pin);
	}
	else weather_read_pixel(win, sty, act, pin);
}
static void weather_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void weather_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void weather_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void weather_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void weather_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	if(0 == lf)return;
	if(_fg3d_ == twig->fmt)return;

	//alloc
	pair = alloc_winobj(root);
	src = &pair->src;
	dst = &pair->dst;
	lf->foot[0] = (u64)src;
	tf->foot[0] = (u64)dst;

	//
	src->geometry = 3;
	src->method = 'v';

	//shader
	src->vs = weather_glsl_v;
	src->fs = weather_glsl_f;
	if(twig){if(_fg2d_ == twig->fmt)src->vs = weather_glsl2d_v;}
	src->shader_enq[0] = 42;

	//vertex
	src->vbuf = memorycreate(4*6*6);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_enq = 0;
}
static void weather_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void weather_create(struct actor* act)
{
	if(0 == act)return;
	act->buf = memorycreate(16);

	act->target.vc[0] = 0;
	act->target.vc[1] = 0;
	act->target.vr[0] = 1.0;
	act->target.vf[1] = 1.0;
}




void weather_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('w', 'e', 'a', 't', 'h', 'e', 'r', 0);

	p->oncreate = (void*)weather_create;
	p->ondelete = (void*)weather_delete;
	p->onstart  = (void*)weather_start;
	p->onstop   = (void*)weather_stop;
	p->oncread  = (void*)weather_cread;
	p->oncwrite = (void*)weather_cwrite;
	p->onsread  = (void*)weather_sread;
	p->onswrite = (void*)weather_swrite;
}
