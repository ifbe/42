#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void actorcreatefromfile(struct actor* act, char* name);




char* light_glsl2d_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec2 texuvw;\n"
	"out mediump vec2 uvw;\n"
	"void main()\n"
	"{\n"
		"uvw = texuvw;\n"
		"gl_Position = vec4(vertex, 1.0);\n"
	"}\n";
char* light_glsl_v =
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
char* light_glsl_f =
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"mediump float n = 1.0;"
		"mediump float f = 1000.0;"
		"mediump float d = texture(tex0, uvw).r;"
		"mediump float c = (2.0 * n) / (f + n - d * (f - n));"
		"FragColor = vec4(c, c, c, 1.0);\n"
	"}\n";




static void light_read_pixel(
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
static void light_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid_cone(win, 0xffff00, vc, vr, vu);

	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	tu[0] = vu[0]/2;
	tu[1] = vu[1]/2;
	tu[2] = vu[2]/2;
	carvesolid_sphere(win, 0xffff00, vc, tr, tf, tu);

	struct glsrc* src = (void*)(pin->foot[0]);
	float (*vbuf)[6] = (void*)(src->vbuf);
	//carvesolid_rect(win, 0xffffff, vc, vr, vf);

	vbuf[0][0] = vc[0] - vr[0] - vf[0] + vu[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1] + vu[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2] + vu[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0] + vu[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1] + vu[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2] + vu[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0] + vu[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1] + vu[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2] + vu[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0] + vu[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1] + vu[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2] + vu[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0] + vu[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1] + vu[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2] + vu[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0] + vu[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1] + vu[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2] + vu[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
}
static void light_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void light_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void light_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void light_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void light_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)light_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)light_read_tui(win, sty, act, pin);
	else if(fmt == _html_)light_read_html(win, sty, act, pin);
	else if(fmt == _json_)light_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)light_read_vbo(win, sty, act, pin);
	else light_read_pixel(win, sty, act, pin);
}
static void light_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void light_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void light_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void light_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void light_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
	struct relation* rel;
	struct arena* tmp;

	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	if(0 == lf)return;

	//
	pair = alloc_winobj(root);
	src = &pair->src;
	dst = &pair->dst;
	lf->foot[0] = (u64)src;
	tf->foot[0] = (u64)dst;

	//
	src->vs = light_glsl_v;
	src->fs = light_glsl_f;
	if(twig){if(_fg2d_ == twig->fmt)src->vs = light_glsl2d_v;}

	//vertex
	src->vbuf = memorycreate(4*6*6);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->method = 'v';

	//send!
	src->shader_enq[0] = 42;
	src->arg_enq[0] = 0;
	src->tex_enq[0] = 0;
	src->vbuf_enq = 0;
	src->ibuf_enq = 0;

	//special
	rel = leaf->orel0;
	if(0 == rel)return;

	tmp = (void*)(rel->dstchip);
	if(0 == tmp)return;
	if(_fbo_ != tmp->fmt)return;

	say("tex_rgb=%x\n", tmp->tex_depth);
	dst->tex[0] = tmp->tex_depth;

	tmp->target.vc[0] = 0.0;
	tmp->target.vc[1] = 0.0;
	tmp->target.vc[2] = 0.0;

	tmp->camera.vc[0] = 0.0;
	tmp->camera.vc[1] = -512.0;
	tmp->camera.vc[2] = 512.0;

	tmp->camera.vf[0] = (tmp->target.vc[0])-(tmp->camera.vc[0]);
	tmp->camera.vf[1] = (tmp->target.vc[1])-(tmp->camera.vc[1]);
	tmp->camera.vf[2] = (tmp->target.vc[2])-(tmp->camera.vc[2]);

	tmp->camera.vu[0] = 0.0;
	tmp->camera.vu[1] = 0.0;
	tmp->camera.vu[2] = 1.0;
}
static void light_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void light_create(struct actor* act, void* str)
{
	void* win;
	if(0 == act)return;

	win = arenacreate(_fbo_, 0);
	if(0 == win)return;

	relationcreate(win, 0, _win_, act, 0, _act_);
}




void light_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('l', 'i', 'g', 'h', 't', 0, 0, 0);

	p->oncreate = (void*)light_create;
	p->ondelete = (void*)light_delete;
	p->onstart  = (void*)light_start;
	p->onstop   = (void*)light_stop;
	p->onget    = (void*)light_cread;
	p->onpost   = (void*)light_cwrite;
	p->onread   = (void*)light_sread;
	p->onwrite  = (void*)light_swrite;
}
