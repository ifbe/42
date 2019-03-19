#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void actorcreatefromfile(struct actor* act, char* name);




char* mirror_glsl2d_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec2 texuvw;\n"
	"out mediump vec2 uvw;\n"
	"void main()\n"
	"{\n"
		"uvw = texuvw;\n"
		"gl_Position = vec4(vertex, 1.0);\n"
	"}\n";
char* mirror_glsl_v =
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
char* mirror_glsl_f =
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"vec3 c = 0.8*texture(tex0, uvw).rgb + vec3(0.2, 0.2, 0.2);\n"
		"FragColor = vec4(c, 1.0);\n"
	"}\n";




void mat4_vector(mat4 m, float* v);
void fixview(mat4 viewmatrix, struct arena* win);
void mirrorfrustum(struct arena* win, struct style* mir, struct style* cam)
{
	mat4 view;
	vec3 v0,v1,v2,v3;
	float* vc = mir->vc;
	float* vr = mir->vr;
	float* vf = mir->vf;

	win->nearn = cam->vc[2] + 1.0;
	win->nearl = - (vc[0] - vr[0] - win->camera.vc[0]);
	win->nearr = - (vc[0] + vr[0] - win->camera.vc[0]);
	win->nearb = vc[1] + vf[1] - win->camera.vc[1];
	win->neart = vc[1] - vf[1] - win->camera.vc[1];

	fixview(view, win);
	v0[0] = vc[0] - vr[0] - vf[0];
	v0[1] = vc[1] - vr[1] - vf[1];
	v0[2] = vc[2] - vr[2] - vf[2];
	mat4_vector(view, v0);
	v1[0] = vc[0] + vr[0] - vf[0];
	v1[1] = vc[1] + vr[1] - vf[1];
	v1[2] = vc[2] + vr[2] - vf[2];
	mat4_vector(view, v1);
	v2[0] = vc[0] - vr[0] + vf[0];
	v2[1] = vc[1] - vr[1] + vf[1];
	v2[2] = vc[2] - vr[2] + vf[2];
	mat4_vector(view, v2);
	v3[0] = vc[0] + vr[0] + vf[0];
	v3[1] = vc[1] + vr[1] + vf[1];
	v3[2] = vc[2] + vr[2] + vf[2];
	mat4_vector(view, v3);
	say("%f,%f,%f\n", v0[0], v0[1], v0[2]);
	say("%f,%f,%f\n", v1[0], v1[1], v1[2]);
	say("%f,%f,%f\n", v2[0], v2[1], v2[2]);
	say("%f,%f,%f\n", v3[0], v3[1], v3[2]);
	say("\n");
}
void mirrorcamera(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	struct relation* rel;
	struct arena* tmp;
	struct glsrc* src = (void*)(lf->foot[0]);
	struct gldst* dst = (void*)(tf->foot[0]);

	rel = leaf->orel0;
	if(0 == rel)return;

	tmp = (void*)(rel->dstchip);
	if(0 == tmp)return;
	if(_fbo_ != tmp->fmt)return;

	//say("tex_rgb=%x\n", tmp->tex_color);
	dst->tex[0] = tmp->tex_color;


	tmp->target.vc[0] = root->camera.vc[0];
	tmp->target.vc[1] = root->camera.vc[1];
	tmp->target.vc[2] = root->camera.vc[2];

	tmp->camera.vc[0] = root->camera.vc[0];
	tmp->camera.vc[1] = root->camera.vc[1];
	tmp->camera.vc[2] =-root->camera.vc[2];

	tmp->camera.vf[0] = (tmp->target.vc[0])-(tmp->camera.vc[0]);
	tmp->camera.vf[1] = (tmp->target.vc[1])-(tmp->camera.vc[1]);
	tmp->camera.vf[2] = (tmp->target.vc[2])-(tmp->camera.vc[2]);

	tmp->camera.vu[0] = 0.0;
	tmp->camera.vu[1] = 1.0;
	tmp->camera.vu[2] = 0.0;

	mirrorfrustum(tmp, tf, &root->camera);
}




static void mirror_read_pixel(
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
static void mirror_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
	mirrorcamera(act, pin, 0, sty, win, 0);
}
static void mirror_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mirror_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mirror_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mirror_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mirror_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)mirror_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)mirror_read_tui(win, sty, act, pin);
	else if(fmt == _html_)mirror_read_html(win, sty, act, pin);
	else if(fmt == _json_)mirror_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)mirror_read_vbo(win, sty, act, pin);
	else mirror_read_pixel(win, sty, act, pin);
}
static void mirror_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void mirror_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mirror_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void mirror_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void mirror_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
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
	src->vs = mirror_glsl_v;
	src->fs = mirror_glsl_f;
	if(twig){if(_fg2d_ == twig->fmt)src->vs = mirror_glsl2d_v;}

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
}
static void mirror_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void mirror_create(struct actor* act, void* str)
{
	void* win;
	if(0 == act)return;

	win = arenacreate(_fbo_, 0);
	if(0 == win)return;

	relationcreate(win, 0, _win_, act, 0, _act_);
}




void mirror_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('m', 'i', 'r', 'r', 'o', 'r', 0, 0);

	p->oncreate = (void*)mirror_create;
	p->ondelete = (void*)mirror_delete;
	p->onstart  = (void*)mirror_start;
	p->onstop   = (void*)mirror_stop;
	p->onget    = (void*)mirror_cread;
	p->onpost   = (void*)mirror_cwrite;
	p->onread   = (void*)mirror_sread;
	p->onwrite  = (void*)mirror_swrite;
}
