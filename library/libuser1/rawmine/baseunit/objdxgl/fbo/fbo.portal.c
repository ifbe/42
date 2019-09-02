#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void actorcreatefromfile(struct actor* act, char* name);



/*
char* portal_glsl2d_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = vec4(vertex, 1.0);\n"
"}\n";
*/
char* portal_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* portal_glsl_f =
GLSL_VERSION
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"void main(){\n"
	"mediump float t = 4.0*(uvw.x-0.5)*(uvw.x-0.5) + 4.0*(uvw.y-0.5)*(uvw.y-0.5);\n"
	"mediump vec3 c = texture(tex0, uvw).rgb;\n"
	"if(t > 1.0)discard;\n"
	"if(t > 0.8)c += 25.0*(t-0.8)*(t-0.8)*vec3(1.0, 1.0, 1.0);\n"
	"FragColor = vec4(c, 1.0);\n"
"}\n";




/*
void mat4_vector(mat4 m, float* v);
void fixview(mat4 viewmatrix, struct actor* win);
void portalfrustum(struct actor* win, struct style* por)
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
	struct actor* leaf, struct style* lf,
	struct actor* twig, struct style* tf,
	struct actor* root, struct style* rf)
{/*
	float x,y,z,t;
	float r,u,n;
	vec3 p,q;
	struct relation* rel;
	struct actor* tmp;
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
	q[0] = root->camera.vc[0] - tf->f.vc[0];
	q[1] = root->camera.vc[1] - tf->f.vc[1];
	q[2] = root->camera.vc[2] - tf->f.vc[2];

	//portal.n
	x = -tf->f.vf[0];
	y = -tf->f.vf[1];
	z = -tf->f.vf[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;
	n = x * q[0] + y * q[1] + z * q[1];

	//portal.r
	x = tf->f.vr[0];
	y = tf->f.vr[1];
	z = tf->f.vr[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;
	r = x * q[0] + y * q[1] + z * q[2];

	//portal.t
	x = tf->f.vt[0];
	y = tf->f.vt[1];
	z = tf->f.vt[2];
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
	x = leaf->target.vt[0];
	y = leaf->target.vt[1];
	z = leaf->target.vt[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;
	tmp->camera.vc[0] += x * u;
	tmp->camera.vc[1] += y * u;
	tmp->camera.vc[2] += z * u;
	tmp->camera.vb[0] = - leaf->target.vt[0] - u*x;
	tmp->camera.vb[1] = - leaf->target.vt[1] - u*y;
	tmp->camera.vb[2] = - leaf->target.vt[2] - u*z;
	tmp->camera.vt[0] = leaf->target.vt[0] - u*x;
	tmp->camera.vt[1] = leaf->target.vt[1] - u*y;
	tmp->camera.vt[2] = leaf->target.vt[2] - u*z;
	carvefrustum(root, &tmp->camera);
*/
/*
	say("%f,%f,%f\n",root->camera.vc[0], root->camera.vc[1], root->camera.vc[2]);
	say("%f,%f,%f\n",tmp->camera.vc[0], tmp->camera.vc[1], tmp->camera.vc[2]);
	say("%f,%f,%f\n",tmp->camera.vn[0], tmp->camera.vn[1], tmp->camera.vn[2]);
	say("%f,%f,%f\n",tmp->camera.vl[0], tmp->camera.vl[1], tmp->camera.vl[2]);
	say("%f,%f,%f\n",tmp->camera.vr[0], tmp->camera.vr[1], tmp->camera.vr[2]);
	say("%f,%f,%f\n",tmp->camera.vb[0], tmp->camera.vb[1], tmp->camera.vb[2]);
	say("%f,%f,%f\n",tmp->camera.vu[0], tmp->camera.vu[1], tmp->camera.vu[2]);
	say("\n");
*/
}




static void portal_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
static void portal_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	vec3 tc,tr,tf,tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carveline_rect(win, 0xffffff, act->target.vc, act->target.vr, act->target.vt);

	struct glsrc* src = (void*)(pin->data[0]);
	float (*vbuf)[6] = (void*)(src->vbuf);

	portalcamera(act, pin, 0, sty, win, 0);

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
}
static void portal_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void portal_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void portal_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void portal_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void portal_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)portal_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)portal_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)portal_draw_html(act, pin, win, sty);
	else if(fmt == _json_)portal_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)portal_draw_vbo(act, pin, win, sty);
	else portal_draw_pixel(act, pin, win, sty);
}




static void portal_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	portal_draw(act, pin, win, sty);
}
static void portal_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void portal_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void portal_start(struct halfrel* self, struct halfrel* peer)
{
	struct relation* rel;
	struct actor* tmp;

	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;

	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
/*
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
	src->vs = portal_glsl_v;
	src->fs = portal_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);

	//texture
	src->tex_name[0] = "tex0";




	//special
	rel = act->orel0;
	if(0 == rel)return;

	tmp = (void*)(rel->dstchip);
	if(0 == tmp)return;
	//if(_fbo_ != tmp->fmt)return;

	say("tex_rgb=%x\n", tmp->tex_color);
	dst->tex[0] = tmp->tex_color;

	//tar
	act->target.vc[0] = -1000;
	act->target.vc[1] = 0;
	act->target.vc[2] = 0;

	act->target.vr[0] = 0;
	act->target.vr[1] = -250;
	act->target.vr[2] = 0.0;

	act->target.vf[0] = 250.0;
	act->target.vf[1] = 0;
	act->target.vf[2] = 0;

	act->target.vt[0] = 0;
	act->target.vt[1] = 0;
	act->target.vt[2] = 250;
*/
}




static void portal_search(struct actor* act)
{
}
static void portal_modify(struct actor* act)
{
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

	//win = actorcreate(_fbo_, 0);
	//if(win)relationcreate(win, 0, _win_, 0, act, 0, _act_, 0);
}




void portal_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'o', 'r', 't', 'a', 'l', 0, 0);

	p->oncreate = (void*)portal_create;
	p->ondelete = (void*)portal_delete;
	p->onsearch = (void*)portal_search;
	p->onmodify = (void*)portal_modify;

	p->onstart = (void*)portal_start;
	p->onstop  = (void*)portal_stop;
	p->onread  = (void*)portal_read;
	p->onwrite = (void*)portal_write;
}
