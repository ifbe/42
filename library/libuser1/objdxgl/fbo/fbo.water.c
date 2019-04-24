#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void actorcreatefromfile(struct actor* act, char* name);




char* water_glsl2d_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec2 t;\n"
"out mediump vec3 normal;\n"
"out mediump vec2 texuvw;\n"
"void main(){\n"
	"texuvw = t;\n"
	"normal = vec3(0.0, 0.0, 1.0);\n"
	"gl_Position = vec4(v, 1.0);\n"
"}\n";

char* water_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec2 t;\n"
"out mediump vec3 normal;\n"
"out mediump vec2 texuvw;\n"
"out mediump vec3 angle0;\n"
"uniform vec3 camxyz;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"texuvw = t;\n"
	"normal = vec3(0.0, 0.0, 1.0);\n"
	"angle0 = camxyz - v;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char* water_glsl_f =
GLSL_VERSION
"in mediump vec3 normal;\n"
"in mediump vec2 texuvw;\n"
"in mediump vec3 angle0;\n"
"out mediump vec4 FragColor;\n"
"uniform mediump float time;\n"
"uniform sampler2D dudvmap;\n"
"uniform sampler2D reflect;\n"
"void main(){\n"
	"mediump vec2 du = texture(dudvmap, vec2(texuvw.x+time, texuvw.y)).rg;\n"
	"du = (2.0*du - vec2(1.0, 1.0)) / 64.0;\n"
	"mediump vec2 dv = texture(dudvmap, vec2(texuvw.x+du.y, texuvw.y-time)).rg;\n"
	"dv = (2.0*dv - vec2(1.0, 1.0)) / 64.0;\n"
	"mediump vec2 uv = texuvw + du + dv;\n"
	"uv = vec2(clamp(uv.x, 0.001, 0.999), clamp(uv.y, 0.001, 0.999));\n"

	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 A = normalize(angle0);\n"
	"mediump vec3 c = texture(reflect, uv).rgb;\n"
	"FragColor = vec4(c, 1.0 - 0.9*abs(dot(N, A)));\n"
"}\n";




void watercamera(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	float x,y,z,t;
	vec3 p,q;
	struct relation* rel;
	struct arena* fbo;
	struct glsrc* src = (void*)(lf->foot[0]);
	struct gldst* dst = (void*)(tf->foot[0]);

	rel = leaf->orel0;
	if(0 == rel)return;

	fbo = (void*)(rel->dstchip);
	if(0 == fbo)return;
	if(_fbo_ != fbo->fmt)return;

	//say("tex_rgb=%x\n", fbo->tex_color);
	dst->tex[1] = fbo->tex_color;


	//mirror.n
	x = tf->vu[0];
	y = tf->vu[1];
	z = tf->vu[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//op*cos(on,op): t = op * mirror.n
	t = (root->camera.vc[0] - tf->vc[0])*x
	  + (root->camera.vc[1] - tf->vc[1])*y
	  + (root->camera.vc[2] - tf->vc[2])*z;

	//dir*len: fbo.n = t*mirror.n + offset
	fbo->camera.vn[0] = x * t * 1.001;
	fbo->camera.vn[1] = y * t * 1.001;
	fbo->camera.vn[2] = z * t * 1.001;

	//foot of a perpendicular: fbo.q = p - t*mirror.n
	fbo->camera.vq[0] = root->camera.vc[0] - t*x;
	fbo->camera.vq[1] = root->camera.vc[1] - t*y;
	fbo->camera.vq[2] = root->camera.vc[2] - t*z;

	//reflected point: p' = p - 2*t*mirror.n
	fbo->camera.vc[0] = root->camera.vc[0] - 2*t*x;
	fbo->camera.vc[1] = root->camera.vc[1] - 2*t*y;
	fbo->camera.vc[2] = root->camera.vc[2] - 2*t*z;


	//r = -mirror.r
	x = -tf->vr[0];
	y = -tf->vr[1];
	z = -tf->vr[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//l.len = (l-q) * nr
	t = (tf->vc[0] + tf->vr[0] - fbo->camera.vq[0]) * x
	  + (tf->vc[1] + tf->vr[1] - fbo->camera.vq[1]) * y
	  + (tf->vc[2] + tf->vr[2] - fbo->camera.vq[2]) * z;
	fbo->camera.vl[0] = x * t;
	fbo->camera.vl[1] = y * t;
	fbo->camera.vl[2] = z * t;

	//r.len = (r-q) * nr
	t = (tf->vc[0] - tf->vr[0] - fbo->camera.vq[0]) * x
	  + (tf->vc[1] - tf->vr[1] - fbo->camera.vq[1]) * y
	  + (tf->vc[2] - tf->vr[2] - fbo->camera.vq[2]) * z;
	fbo->camera.vr[0] = x * t;
	fbo->camera.vr[1] = y * t;
	fbo->camera.vr[2] = z * t;


	//mirror.t
	x = tf->vf[0];
	y = tf->vf[1];
	z = tf->vf[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//b.len =  = (b-q) * nt
	t = (tf->vc[0] - tf->vf[0] - fbo->camera.vq[0]) * x
	  + (tf->vc[1] - tf->vf[1] - fbo->camera.vq[1]) * y
	  + (tf->vc[2] - tf->vf[2] - fbo->camera.vq[2]) * z;
	fbo->camera.vb[0] = x * t;
	fbo->camera.vb[1] = y * t;
	fbo->camera.vb[2] = z * t;

	//t.len = (u-q) * nt
	t = (tf->vc[0] + tf->vf[0] - fbo->camera.vq[0]) * x
	  + (tf->vc[1] + tf->vf[1] - fbo->camera.vq[1]) * y
	  + (tf->vc[2] + tf->vf[2] - fbo->camera.vq[2]) * z;
	fbo->camera.vu[0] = x * t;
	fbo->camera.vu[1] = y * t;
	fbo->camera.vu[2] = z * t;

	carvefrustum(root, &fbo->camera);
/*
	say("%f,%f,%f\n",root->camera.vc[0], root->camera.vc[1], root->camera.vc[2]);
	say("%f,%f,%f\n",tmp->camera.vc[0], tmp->camera.vc[1], tmp->camera.vc[2]);
	say("%f,%f,%f\n",tmp->camera.vn[0], tmp->camera.vn[1], tmp->camera.vn[2]);
	say("%f,%f,%f\n",tmp->camera.vl[0], tmp->camera.vl[1], tmp->camera.vl[2]);
	say("%f,%f,%f\n",tmp->camera.vr[0], tmp->camera.vr[1], tmp->camera.vr[2]);
	say("%f,%f,%f\n",tmp->camera.vb[0], tmp->camera.vb[1], tmp->camera.vb[2]);
	say("%f,%f,%f\n",tmp->camera.vu[0], tmp->camera.vu[1], tmp->camera.vu[2]);
	say("\n");

	carveline_rect(root, 0xffffff, tf->vc, tf->vr, tf->vf);
	p[0] = tf->vc[0] - tf->vr[0] - tf->vf[0];
	p[1] = tf->vc[1] - tf->vr[1] - tf->vf[1];
	p[2] = tf->vc[2] - tf->vr[2] - tf->vf[2];
	q[0] = 2*p[0] - tmp->camera.vc[0];
	q[1] = 2*p[1] - tmp->camera.vc[1];
	q[2] = 2*p[2] - tmp->camera.vc[2];
	carveline(root, 0xffffff, tmp->camera.vc, q);
	p[0] = tf->vc[0] + tf->vr[0] - tf->vf[0];
	p[1] = tf->vc[1] + tf->vr[1] - tf->vf[1];
	p[2] = tf->vc[2] + tf->vr[2] - tf->vf[2];
	q[0] = 2*p[0] - tmp->camera.vc[0];
	q[1] = 2*p[1] - tmp->camera.vc[1];
	q[2] = 2*p[2] - tmp->camera.vc[2];
	carveline(root, 0xffffff, tmp->camera.vc, q);
	p[0] = tf->vc[0] - tf->vr[0] + tf->vf[0];
	p[1] = tf->vc[1] - tf->vr[1] + tf->vf[1];
	p[2] = tf->vc[2] - tf->vr[2] + tf->vf[2];
	q[0] = 2*p[0] - tmp->camera.vc[0];
	q[1] = 2*p[1] - tmp->camera.vc[1];
	q[2] = 2*p[2] - tmp->camera.vc[2];
	carveline(root, 0xffffff, tmp->camera.vc, q);
	p[0] = tf->vc[0] + tf->vr[0] + tf->vf[0];
	p[1] = tf->vc[1] + tf->vr[1] + tf->vf[1];
	p[2] = tf->vc[2] + tf->vr[2] + tf->vf[2];
	q[0] = 2*p[0] - tmp->camera.vc[0];
	q[1] = 2*p[1] - tmp->camera.vc[1];
	q[2] = 2*p[2] - tmp->camera.vc[2];
	carveline(root, 0xffffff, tmp->camera.vc, q);
*/
}




static void water_read_pixel(
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
static void water_read_vbo(
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

	act->target.vq[0] = (timeread()%10000000)/10000000.0;
	watercamera(act, pin, 0, sty, win, 0);

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = 1.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 0.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = 1.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 0.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = 1.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 0.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
}
static void water_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void water_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void water_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void water_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void water_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)water_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)water_read_tui(win, sty, act, pin);
	else if(fmt == _html_)water_read_html(win, sty, act, pin);
	else if(fmt == _json_)water_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)water_read_vbo(win, sty, act, pin);
	else water_read_pixel(win, sty, act, pin);
}
static void water_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void water_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void water_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void water_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void water_start(
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
	src->method = 'v';
	src->opaque = 1;

	//
	src->vs = water_glsl_v;
	src->fs = water_glsl_f;
	if(twig){if(_fg2d_ == twig->fmt)src->vs = water_glsl2d_v;}
	src->shader_enq = 42;

	//vertex
	src->vbuf = memorycreate(4*6*6);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len);

	//argument
	src->arg_name[0] = "time";
	src->arg_data[0] = &leaf->target.vq[0];
	src->arg_fmt[0] = 'f';

	//texture0
	src->tex_name[0] = "dudvmap";
	src->tex_data[0] = leaf->buf;
	src->tex_fmt[0] = hex32('r','g','b','a');
	src->tex_w[0] = leaf->width;
	src->tex_h[0] = leaf->height;
	src->tex_enq[0] = 42;

	//texture1
	src->tex_name[1] = "reflect";
}
static void water_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void water_create(struct actor* act, void* str)
{
	void* win;
	if(0 == act)return;

	act->buf = memorycreate(0x1000000);
	actorcreatefromfile(act, "datafile/jpg/dudvmap.jpg");

	win = arenacreate(_fbo_, 0);
	if(win)relationcreate(win, 0, _win_, act, 0, _act_);
}




void water_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('w', 'a', 't', 'e', 'r', 0, 0, 0);

	p->oncreate = (void*)water_create;
	p->ondelete = (void*)water_delete;
	p->onstart  = (void*)water_start;
	p->onstop   = (void*)water_stop;
	p->oncread  = (void*)water_cread;
	p->oncwrite = (void*)water_cwrite;
	p->onsread  = (void*)water_sread;
	p->onswrite = (void*)water_swrite;
}
