#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void actorcreatefromfile(struct actor* act, char* name);
void fixmatrix(mat4 mvp, struct fstyle* sty);




char* shadowmap_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 t;\n"
"out mediump vec3 xyz;\n"
"out mediump vec3 rgb;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"xyz = v;\n"
	"rgb = t;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char* shadowmap_glsl_f =
GLSL_VERSION
"in mediump vec3 xyz;\n"
"in mediump vec3 rgb;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"uniform mat4 mapmvp;\n"
"float shadow(){\n"
	"mediump vec4 tmp = mapmvp * vec4(xyz, 1.0);\n"
	"tmp /= tmp.w;\n"
	"tmp = (tmp+1.0)*0.5;\n"
	"if(tmp.z - texture(tex0, tmp.xy).r > 0.001)return 0.5;\n"
	"return 1.0;\n"
"}\n"
"float colour(){"
	"mediump float n = 1.0;"
	"mediump float f = 100.0;"
	"mediump float d = texture(tex0, rgb.xy).r;"
	"mediump float c = (2.0 * n) / (f + n - d * (f - n));"
	"return c;\n"
"}\n"
"void main(){\n"
	"mediump float t = shadow();\n"
	"mediump float c = colour();\n"
	"FragColor = vec4(vec3(c)*vec3(t), 1.0);\n"
"}\n";
/*
char* shadowmap_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main()\n"
"{\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* shadowmap_glsl_f =
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
*/




void fixcam(struct fstyle* cam, struct fstyle* tar)
{
	//a X b = [ay*bz - az*by, az*bx-ax*bz, ax*by-ay*bx]
	float x,y,z,norm;

	//near
	x = tar->vc[0] - cam->vc[0];
	y = tar->vc[1] - cam->vc[1];
	z = tar->vc[2] - cam->vc[2];
	norm = squareroot(x*x + y*y + z*z);
	x /= norm;
	y /= norm;
	z /= norm;
	cam->vn[0] = x*25.0;
	cam->vn[1] = y*25.0;
	cam->vn[2] = z*25.0;

	//right = cross(near, (0,0,1))
	x = cam->vn[1]*1 - cam->vn[2]*0;
	y = cam->vn[2]*0 - cam->vn[0]*1;
	z = cam->vn[0]*0 - cam->vn[1]*0;
	norm = squareroot(x*x + y*y + z*z);
	x /= norm;
	y /= norm;
	z /= norm;
	cam->vr[0] = x*25.0;
	cam->vr[1] = y*25.0;
	cam->vr[2] = z*25.0;
	cam->vl[0] = -x*25.0;
	cam->vl[1] = -y*25.0;
	cam->vl[2] = -z*25.0;

	//upper = cross(right, near)
	x = cam->vr[1]*cam->vn[2] - cam->vr[2]*cam->vn[1];
	y = cam->vr[2]*cam->vn[0] - cam->vr[0]*cam->vn[2];
	z = cam->vr[0]*cam->vn[1] - cam->vr[1]*cam->vn[0];
	norm = squareroot(x*x + y*y + z*z);
	x /= norm;
	y /= norm;
	z /= norm;
	cam->vt[0] = x*25.0;
	cam->vt[1] = y*25.0;
	cam->vt[2] = z*25.0;
	cam->vb[0] = -x*25.0;
	cam->vb[1] = -y*25.0;
	cam->vb[2] = -z*25.0;
}




static void shadowmap_draw_pixel(
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
static void shadowmap_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	vec3 tr,tf,tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	float a,c,s;
	float x,y,z,n;
	void* mvp;
	struct relation* rel;
	struct actor* tmp;
	struct gldst* dst = (void*)(sty->foot[0]);
	struct glsrc* src = (void*)(pin->foot[0]);
	float (*vbuf)[6] = (void*)(src->vbuf);
	//carvesolid_rect(win, 0xffffff, vc, vr, vf);

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 1.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 1.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 1.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 1.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 1.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 1.0;




	vbuf[6][0] = vc[0] + vu[0];
	vbuf[6][1] = vc[1] + vu[1];
	vbuf[6][2] = vc[2] + vu[2];
	vbuf[6][3] = 0.0;
	vbuf[6][4] = 0.0;
	vbuf[6][5] = 0.0;

	vbuf[7][0] = vc[0] + vr[0];
	vbuf[7][1] = vc[1] + vr[1];
	vbuf[7][2] = vc[2] + vr[2];
	vbuf[7][3] = 0.0;
	vbuf[7][4] = 0.0;
	vbuf[7][5] = 0.0;

	vbuf[8][0] = vc[0] + vf[0];
	vbuf[8][1] = vc[1] + vf[1];
	vbuf[8][2] = vc[2] + vf[2];
	vbuf[8][3] = 0.0;
	vbuf[8][4] = 0.0;
	vbuf[8][5] = 0.0;

	vbuf[9][0] = vc[0] + vu[0]/2;
	vbuf[9][1] = vc[1] + vu[1]/2;
	vbuf[9][2] = vc[2] + vu[2]/2;
	vbuf[9][3] = 0.0;
	vbuf[9][4] = 0.0;
	vbuf[9][5] = 0.0;

	vbuf[10][0] = vc[0] - vr[0] + vu[0]/2;
	vbuf[10][1] = vc[1] - vr[1] + vu[1]/2;
	vbuf[10][2] = vc[2] - vr[2] + vu[2]/2;
	vbuf[10][3] = 0.0;
	vbuf[10][4] = 0.0;
	vbuf[10][5] = 0.0;

	vbuf[11][0] = vc[0] - vf[0] + vu[0]/2;
	vbuf[11][1] = vc[1] - vf[1] + vu[1]/2;
	vbuf[11][2] = vc[2] - vf[2] + vu[2]/2;
	vbuf[11][3] = 0.0;
	vbuf[11][4] = 0.0;
	vbuf[11][5] = 0.0;

	src->vbuf_enq += 1;



/*
	rel = act->orel0;
	if(0 == rel)return;

	tmp = (void*)(rel->dstchip);
	if(0 == tmp)return;
	if(_fbo_ != tmp->fmt)return;

	dst->tex[0] = tmp->tex_depth;

	//light camera
	a = tau * timeread() / 10000000.0;
	c = cosine(a);
	s = sine(a);
	tmp->camera.vc[0] = sty->f.vc[0] + sty->f.vr[0]*c + sty->f.vf[0]*s + sty->f.vt[0]*2;
	tmp->camera.vc[1] = sty->f.vc[1] + sty->f.vr[1]*c + sty->f.vf[1]*s + sty->f.vt[1]*2;
	tmp->camera.vc[2] = sty->f.vc[2] + sty->f.vr[2]*c + sty->f.vf[2]*s + sty->f.vt[2]*2;
	fixcam(&tmp->camera, &sty->f);


	//viewer camera
	c = cosine(a + PI/2);
	s = sine(a + PI/2);
	act->camera.vc[0] = sty->f.vc[0] + sty->f.vr[0]*c + sty->f.vf[0]*s + sty->f.vt[0]*2;
	act->camera.vc[1] = sty->f.vc[1] + sty->f.vr[1]*c + sty->f.vf[1]*s + sty->f.vt[1]*2;
	act->camera.vc[2] = sty->f.vc[2] + sty->f.vr[2]*c + sty->f.vf[2]*s + sty->f.vt[2]*2;
	fixcam(&act->camera, &sty->f);


	//calc matrix
	mvp = (void*)(src->arg_data[0]);
	fixmatrix(mvp, &tmp->camera);
	mat4_transpose(mvp);


	//display light
	tr[0] = 20.0;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 20.0;
	tf[2] = 0.0;
	tu[0] = 0.0;
	tu[1] = 0.0;
	tu[2] = 20.0;
	carvesolid_sphere(win, 0xffff00, tmp->camera.vc, tr, tf, tu);
*/
}
static void shadowmap_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void shadowmap_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void shadowmap_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void shadowmap_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void shadowmap_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)shadowmap_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)shadowmap_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)shadowmap_draw_html(act, pin, win, sty);
	else if(fmt == _json_)shadowmap_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)shadowmap_draw_vbo(act, pin, win, sty);
	else shadowmap_draw_pixel(act, pin, win, sty);
}




static void shadowmap_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//shadowmap_draw(act, pin, win, sty);
}
static void shadowmap_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void shadowmap_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void shadowmap_start(struct halfrel* self, struct halfrel* peer)
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
	src->vs = shadowmap_glsl_v;
	src->fs = shadowmap_glsl_f;
	src->shader_enq = 42;

	src->arg_name[0] = "mapmvp";
	src->arg_data[0] = memorycreate(4*4*4);
	src->arg_fmt[0] = 'm';

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 12;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_enq = 0;

	//texture
	src->tex_name[0] = "tex0";
	src->tex_enq[0] = 0;
*/
}




static void shadowmap_search(struct actor* act)
{
}
static void shadowmap_modify(struct actor* act)
{
}
static void shadowmap_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void shadowmap_create(struct actor* act, void* str)
{
	void* win;
	if(0 == act)return;

	//win = actorcreate(_fbo_, 0);
	//if(win)relationcreate(win, 0, _win_, 0, act, 0, _act_, 0);
}




void shadowmap_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'h', 'a', 'd', 'o', 'w', 0, 0);

	p->oncreate = (void*)shadowmap_create;
	p->ondelete = (void*)shadowmap_delete;
	p->onsearch = (void*)shadowmap_search;
	p->onmodify = (void*)shadowmap_modify;

	p->onstart = (void*)shadowmap_start;
	p->onstop  = (void*)shadowmap_stop;
	p->onread  = (void*)shadowmap_read;
	p->onwrite = (void*)shadowmap_write;
}
