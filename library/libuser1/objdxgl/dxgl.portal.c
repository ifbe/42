#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void actorcreatefromfile(struct actor* act, char* name);




char* portal_glsl2d_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = vec4(vertex, 1.0);\n"
"}\n";

char* portal_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out mediump vec2 uvw;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* portal_glsl_f =
GLSL_VERSION
"uniform sampler2D tex0;\n"
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"float t = 4.0*(uvw.x-0.5)*(uvw.x-0.5) + 4.0*(uvw.y-0.5)*(uvw.y-0.5);\n"
	"vec3 c = texture(tex0, uvw).rgb;\n"
	"if(t > 1.0)discard;\n"
	"if(t > 0.8)c += 25.0*(t-0.8)*(t-0.8)*vec3(1.0, 1.0, 1.0);\n"
	"FragColor = vec4(c, 1.0);\n"
"}\n";




/*
void mat4_vector(mat4 m, float* v);
void fixview(mat4 viewmatrix, struct arena* win);
void portalfrustum(struct arena* win, struct style* por)
{
	mat4 view;
	vec3 lb,rt;
	float* vc = por->vc;
	float* vr = por->vr;
	float* vf = por->vf;

	fixview(view, win);
	lb[0] = vc[0] - vr[0] - vf[0];
	lb[1] = vc[1] - vr[1] - vf[1];
	lb[2] = vc[2] - vr[2] - vf[2];
	mat4_vector(view, lb);
	rt[0] = vc[0] + vr[0] + vf[0];
	rt[1] = vc[1] + vr[1] + vf[1];
	rt[2] = vc[2] + vr[2] + vf[2];
	mat4_vector(view, rt);

	say("\n");
	say("%f,%f,%f\n", lb[0], lb[1], lb[2]);
	say("%f,%f,%f\n", rt[0], rt[1], rt[2]);
}*/
void portalcamera(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	float x,y,z,t;
	float r,u,n;
	vec3 p,q;
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




	//q = from center to camera
	q[0] = root->camera.vc[0] - tf->vc[0];
	q[1] = root->camera.vc[1] - tf->vc[1];
	q[2] = root->camera.vc[2] - tf->vc[2];

	//portal.n
	x = -tf->vf[0];
	y = -tf->vf[1];
	z = -tf->vf[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;
	n = x * q[0] + y * q[1] + z * q[1];

	//portal.r
	x = tf->vr[0];
	y = tf->vr[1];
	z = tf->vr[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;
	r = x * q[0] + y * q[1] + z * q[2];

	//portal.t
	x = tf->vu[0];
	y = tf->vu[1];
	z = tf->vu[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;
	u = x * q[0] + y * q[1] + z * q[2];




	//camera position
	tmp->camera.vc[0] = leaf->target.vc[0];
	tmp->camera.vc[1] = leaf->target.vc[1];
	tmp->camera.vc[2] = leaf->target.vc[2];
	//say("1:%f,%f,%f\n",tmp->camera.vc[0], tmp->camera.vc[1], tmp->camera.vc[2]);

	//target.f
	x = -leaf->target.vf[0];
	y = -leaf->target.vf[1];
	z = -leaf->target.vf[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;
	tmp->camera.vc[0] += x * n;
	tmp->camera.vc[1] += y * n;
	tmp->camera.vc[2] += z * n;
	tmp->camera.vn[0] = -x * n;
	tmp->camera.vn[1] = -y * n;
	tmp->camera.vn[2] = -z * n;

	//target.r
	x = leaf->target.vr[0];
	y = leaf->target.vr[1];
	z = leaf->target.vr[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;
	tmp->camera.vc[0] += x * r;
	tmp->camera.vc[1] += y * r;
	tmp->camera.vc[2] += z * r;
	tmp->camera.vl[0] = - leaf->target.vr[0] - r*x;
	tmp->camera.vl[1] = - leaf->target.vr[1] - r*y;
	tmp->camera.vl[2] = - leaf->target.vr[2] - r*z;
	tmp->camera.vr[0] = leaf->target.vr[0] - r*x;
	tmp->camera.vr[1] = leaf->target.vr[1] - r*y;
	tmp->camera.vr[2] = leaf->target.vr[2] - r*z;

	//target.u
	x = leaf->target.vu[0];
	y = leaf->target.vu[1];
	z = leaf->target.vu[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;
	tmp->camera.vc[0] += x * u;
	tmp->camera.vc[1] += y * u;
	tmp->camera.vc[2] += z * u;
	tmp->camera.vb[0] = - leaf->target.vu[0] - u*x;
	tmp->camera.vb[1] = - leaf->target.vu[1] - u*y;
	tmp->camera.vb[2] = - leaf->target.vu[2] - u*z;
	tmp->camera.vu[0] = leaf->target.vu[0] - u*x;
	tmp->camera.vu[1] = leaf->target.vu[1] - u*y;
	tmp->camera.vu[2] = leaf->target.vu[2] - u*z;

	say("%f,%f,%f\n",root->camera.vc[0], root->camera.vc[1], root->camera.vc[2]);
	say("%f,%f,%f\n",tmp->camera.vc[0], tmp->camera.vc[1], tmp->camera.vc[2]);
	say("%f,%f,%f\n",tmp->camera.vn[0], tmp->camera.vn[1], tmp->camera.vn[2]);
	say("%f,%f,%f\n",tmp->camera.vl[0], tmp->camera.vl[1], tmp->camera.vl[2]);
	say("%f,%f,%f\n",tmp->camera.vr[0], tmp->camera.vr[1], tmp->camera.vr[2]);
	say("%f,%f,%f\n",tmp->camera.vb[0], tmp->camera.vb[1], tmp->camera.vb[2]);
	say("%f,%f,%f\n",tmp->camera.vu[0], tmp->camera.vu[1], tmp->camera.vu[2]);
	say("\n");

	carvefrustum(root, &tmp->camera);
}




static void portal_read_pixel(
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
static void portal_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tc,tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carveline_rect(win, 0xffffff, act->target.vc, act->target.vr, act->target.vu);

	struct glsrc* src = (void*)(pin->foot[0]);
	float (*vbuf)[6] = (void*)(src->vbuf);

	vbuf[0][0] = vc[0] - vr[0] - vu[0];
	vbuf[0][1] = vc[1] - vr[1] - vu[1];
	vbuf[0][2] = vc[2] - vr[2] - vu[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vu[0];
	vbuf[1][1] = vc[1] + vr[1] + vu[1];
	vbuf[1][2] = vc[2] + vr[2] + vu[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vu[0];
	vbuf[2][1] = vc[1] - vr[1] + vu[1];
	vbuf[2][2] = vc[2] - vr[2] + vu[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vu[0];
	vbuf[3][1] = vc[1] + vr[1] + vu[1];
	vbuf[3][2] = vc[2] + vr[2] + vu[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vu[0];
	vbuf[4][1] = vc[1] - vr[1] - vu[1];
	vbuf[4][2] = vc[2] - vr[2] - vu[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vu[0];
	vbuf[5][1] = vc[1] + vr[1] - vu[1];
	vbuf[5][2] = vc[2] + vr[2] - vu[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
	portalcamera(act, pin, 0, sty, win, 0);
}
static void portal_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void portal_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void portal_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void portal_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void portal_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)portal_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)portal_read_tui(win, sty, act, pin);
	else if(fmt == _html_)portal_read_html(win, sty, act, pin);
	else if(fmt == _json_)portal_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)portal_read_vbo(win, sty, act, pin);
	else portal_read_pixel(win, sty, act, pin);
}
static void portal_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void portal_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void portal_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void portal_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void portal_start(
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
	src->vs = portal_glsl_v;
	src->fs = portal_glsl_f;
	if(twig){if(_fg2d_ == twig->fmt)src->vs = portal_glsl2d_v;}

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

	say("tex_rgb=%x\n", tmp->tex_color);
	dst->tex[0] = tmp->tex_color;

	//tar
	leaf->target.vc[0] = -1000;
	leaf->target.vc[1] = 0;
	leaf->target.vc[2] = 0;

	leaf->target.vr[0] = 0;
	leaf->target.vr[1] = -250;
	leaf->target.vr[2] = 0.0;

	leaf->target.vf[0] = 250.0;
	leaf->target.vf[1] = 0;
	leaf->target.vf[2] = 0;

	leaf->target.vu[0] = 0;
	leaf->target.vu[1] = 0;
	leaf->target.vu[2] = 250;
}
static void portal_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void portal_create(struct actor* act, void* str)
{
	void* win;
	if(0 == act)return;

	win = arenacreate(_fbo_, 0);
	if(win)relationcreate(win, 0, _win_, act, 0, _act_);
}




void portal_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'o', 'r', 't', 'a', 'l', 0, 0);

	p->oncreate = (void*)portal_create;
	p->ondelete = (void*)portal_delete;
	p->onstart  = (void*)portal_start;
	p->onstop   = (void*)portal_stop;
	p->oncread  = (void*)portal_cread;
	p->oncwrite = (void*)portal_cwrite;
	p->onsread  = (void*)portal_sread;
	p->onswrite = (void*)portal_swrite;
}
