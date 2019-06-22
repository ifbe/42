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
	"mediump vec3 c = 0.8*texture(tex0, uvw).rgb + vec3(0.2, 0.2, 0.2);\n"
	"FragColor = vec4(c, 1.0);\n"
"}\n";



/*
void mat4_vector(mat4 m, float* v);
void fixview(mat4 viewmatrix, struct arena* win);
void mirrorfrustum(struct arena* win, struct style* mir)
{
	mat4 view;
	vec3 lb,rt;
	float* vc = mir->vc;
	float* vr = mir->vr;
	float* vf = mir->vf;

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
void mirrorcamera(
	struct actor* leaf, struct style* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{/*
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

	say("tex_rgb=%x\n", fbo->tex_color);
	dst->tex[0] = fbo->tex_color;


	//mirror.n
	x = -tf->f.vf[0];
	y = -tf->f.vf[1];
	z = -tf->f.vf[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//op*cos(on,op): t = op * mirror.n
	t = (root->camera.vc[0] - tf->f.vc[0])*x
	  + (root->camera.vc[1] - tf->f.vc[1])*y
	  + (root->camera.vc[2] - tf->f.vc[2])*z;

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
	x = -tf->f.vr[0];
	y = -tf->f.vr[1];
	z = -tf->f.vr[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//l.len = (l-q) * nr
	t = (tf->f.vc[0] + tf->f.vr[0] - fbo->camera.vq[0]) * x
	  + (tf->f.vc[1] + tf->f.vr[1] - fbo->camera.vq[1]) * y
	  + (tf->f.vc[2] + tf->f.vr[2] - fbo->camera.vq[2]) * z;
	fbo->camera.vl[0] = x * t;
	fbo->camera.vl[1] = y * t;
	fbo->camera.vl[2] = z * t;

	//r.len = (r-q) * nr
	t = (tf->f.vc[0] - tf->f.vr[0] - fbo->camera.vq[0]) * x
	  + (tf->f.vc[1] - tf->f.vr[1] - fbo->camera.vq[1]) * y
	  + (tf->f.vc[2] - tf->f.vr[2] - fbo->camera.vq[2]) * z;
	fbo->camera.vr[0] = x * t;
	fbo->camera.vr[1] = y * t;
	fbo->camera.vr[2] = z * t;


	//mirror.t
	x = tf->f.vt[0];
	y = tf->f.vt[1];
	z = tf->f.vt[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//b.len =  = (b-q) * nt
	t = (tf->f.vc[0] - tf->f.vt[0] - fbo->camera.vq[0]) * x
	  + (tf->f.vc[1] - tf->f.vt[1] - fbo->camera.vq[1]) * y
	  + (tf->f.vc[2] - tf->f.vt[2] - fbo->camera.vq[2]) * z;
	fbo->camera.vb[0] = x * t;
	fbo->camera.vb[1] = y * t;
	fbo->camera.vb[2] = z * t;

	//t.len = (u-q) * nt
	t = (tf->f.vc[0] + tf->f.vt[0] - fbo->camera.vq[0]) * x
	  + (tf->f.vc[1] + tf->f.vt[1] - fbo->camera.vq[1]) * y
	  + (tf->f.vc[2] + tf->f.vt[2] - fbo->camera.vq[2]) * z;
	fbo->camera.vt[0] = x * t;
	fbo->camera.vt[1] = y * t;
	fbo->camera.vt[2] = z * t;

	carvefrustum(root, &fbo->camera);
*/
/*
	say("%f,%f,%f\n",root->camera.vc[0], root->camera.vc[1], root->camera.vc[2]);
	say("%f,%f,%f\n",fbo->camera.vc[0], fbo->camera.vc[1], fbo->camera.vc[2]);
	say("%f,%f,%f\n",fbo->camera.vn[0], fbo->camera.vn[1], fbo->camera.vn[2]);
	say("%f,%f,%f\n",fbo->camera.vl[0], fbo->camera.vl[1], fbo->camera.vl[2]);
	say("%f,%f,%f\n",fbo->camera.vr[0], fbo->camera.vr[1], fbo->camera.vr[2]);
	say("%f,%f,%f\n",fbo->camera.vb[0], fbo->camera.vb[1], fbo->camera.vb[2]);
	say("%f,%f,%f\n",fbo->camera.vu[0], fbo->camera.vu[1], fbo->camera.vu[2]);
	say("\n");

	carveline_rect(root, 0xffffff, tf->vc, tf->vr, tf->vf);
	p[0] = tf->vc[0] - tf->vr[0] - tf->vf[0];
	p[1] = tf->vc[1] - tf->vr[1] - tf->vf[1];
	p[2] = tf->vc[2] - tf->vr[2] - tf->vf[2];
	q[0] = 2*p[0] - fbo->camera.vc[0];
	q[1] = 2*p[1] - fbo->camera.vc[1];
	q[2] = 2*p[2] - fbo->camera.vc[2];
	carveline(root, 0xffffff, fbo->camera.vc, q);
	p[0] = tf->vc[0] + tf->vr[0] - tf->vf[0];
	p[1] = tf->vc[1] + tf->vr[1] - tf->vf[1];
	p[2] = tf->vc[2] + tf->vr[2] - tf->vf[2];
	q[0] = 2*p[0] - fbo->camera.vc[0];
	q[1] = 2*p[1] - fbo->camera.vc[1];
	q[2] = 2*p[2] - fbo->camera.vc[2];
	carveline(root, 0xffffff, fbo->camera.vc, q);
	p[0] = tf->vc[0] - tf->vr[0] + tf->vf[0];
	p[1] = tf->vc[1] - tf->vr[1] + tf->vf[1];
	p[2] = tf->vc[2] - tf->vr[2] + tf->vf[2];
	q[0] = 2*p[0] - fbo->camera.vc[0];
	q[1] = 2*p[1] - fbo->camera.vc[1];
	q[2] = 2*p[2] - fbo->camera.vc[2];
	carveline(root, 0xffffff, fbo->camera.vc, q);
	p[0] = tf->vc[0] + tf->vr[0] + tf->vf[0];
	p[1] = tf->vc[1] + tf->vr[1] + tf->vf[1];
	p[2] = tf->vc[2] + tf->vr[2] + tf->vf[2];
	q[0] = 2*p[0] - fbo->camera.vc[0];
	q[1] = 2*p[1] - fbo->camera.vc[1];
	q[2] = 2*p[2] - fbo->camera.vc[2];
	carveline(root, 0xffffff, fbo->camera.vc, q);
*/
}




static void mirror_draw_vbo(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	struct glsrc* src = (void*)(pin->foot[0]);
	float (*vbuf)[6] = (void*)(src->vbuf);
	//carvesolid_rect(win, 0xffffff, vc, vr, vf);

	vbuf[0][0] = vc[0] - vr[0] - vu[0];
	vbuf[0][1] = vc[1] - vr[1] - vu[1];
	vbuf[0][2] = vc[2] - vr[2] - vu[2];
	vbuf[0][3] = 1.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vu[0];
	vbuf[1][1] = vc[1] + vr[1] + vu[1];
	vbuf[1][2] = vc[2] + vr[2] + vu[2];
	vbuf[1][3] = 0.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vu[0];
	vbuf[2][1] = vc[1] - vr[1] + vu[1];
	vbuf[2][2] = vc[2] - vr[2] + vu[2];
	vbuf[2][3] = 1.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vu[0];
	vbuf[3][1] = vc[1] + vr[1] + vu[1];
	vbuf[3][2] = vc[2] + vr[2] + vu[2];
	vbuf[3][3] = 0.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vu[0];
	vbuf[4][1] = vc[1] - vr[1] - vu[1];
	vbuf[4][2] = vc[2] - vr[2] - vu[2];
	vbuf[4][3] = 1.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vu[0];
	vbuf[5][1] = vc[1] + vr[1] - vu[1];
	vbuf[5][2] = vc[2] + vr[2] - vu[2];
	vbuf[5][3] = 0.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
	//mirrorcamera(act, pin, 0, sty, win, 0);
}
static void mirror_draw_pixel(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void mirror_draw_json(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void mirror_draw_html(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void mirror_draw_tui(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void mirror_draw_cli(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void mirror_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)mirror_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)mirror_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)mirror_draw_html(act, pin, win, sty);
	else if(fmt == _json_)mirror_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)mirror_draw_vbo(act, pin, win, sty);
	else mirror_draw_pixel(act, pin, win, sty);
}




static void mirror_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	mirror_draw(act, pin, win, sty);
}
static void mirror_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void mirror_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void mirror_start(struct halfrel* self, struct halfrel* peer)
{
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;

	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
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

	//shader
	src->vs = mirror_glsl_v;
	src->fs = mirror_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_enq = 0;

	//texture
	src->tex_name[0] = "tex0";
	src->tex_enq[0] = 0;
}




static void mirror_search(struct actor* act)
{
}
static void mirror_modify(struct actor* act)
{
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
	if(win)relationcreate(win, 0, _win_, 0, act, 0, _act_, 0);
}




void mirror_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'i', 'r', 'r', 'o', 'r', 0, 0);

	p->oncreate = (void*)mirror_create;
	p->ondelete = (void*)mirror_delete;
	p->onsearch = (void*)mirror_search;
	p->onmodify = (void*)mirror_modify;

	p->onstart = (void*)mirror_start;
	p->onstop  = (void*)mirror_stop;
	p->onread  = (void*)mirror_read;
	p->onwrite = (void*)mirror_write;
}
