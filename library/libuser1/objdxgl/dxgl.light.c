#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void actorcreatefromfile(struct actor* act, char* name);
void fixmatrix(mat4 mvp, struct arena* win);




char* light_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec3 normal;\n"
"out mediump vec3 norm;\n"
"out mediump vec3 vert;\n"
"out mediump vec3 uvw;\n"
"uniform mat4 mapmvp;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"vert = vertex;\n"
	"norm = normal;\n"

	"mediump vec4 tmp = mapmvp * vec4(vertex, 1.0);\n"
	"tmp /= tmp.w;\n"
	"tmp = (tmp+1.0)*0.5;\n"
	"uvw = vec3(tmp.x, tmp.y, tmp.z);\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* light_glsl_f =
GLSL_VERSION
"in mediump vec3 vert;\n"
"in mediump vec3 norm;\n"
"in mediump vec3 uvw;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"mediump vec3 camxyz = vec3(0.0, -1000.0, 1000.0);\n"
"mediump vec3 ambient = vec3(0.25, 0.25, 0.25);\n"
"mediump vec3 lightcolor = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 lightposition = vec3(500.0, 250.0, 1000.0);\n"
"vec3 phong(){\n"
	"mediump vec3 N = normalize(norm);\n"
	"mediump vec3 L = normalize(vec3(lightposition - vert));\n"
	"mediump vec3 E = normalize(camxyz - vert);\n"
	"mediump vec3 R = reflect(-L, N);\n"
	"mediump float SN = max(dot(N, L), 0.0);\n"
	"mediump float RV = max(dot(R, E), 0.0);\n"
	"mediump vec3 diffuse = lightcolor * SN;\n"
	"mediump vec3 specular = vec3(0.0, 0.0, 0.0);\n"
	"if(SN>0.0)specular = lightcolor * pow(RV, 16.0);\n"
	"return diffuse + specular;\n"
"}\n"
"float shadow(){\n"
	"if(uvw.z - texture(tex0, uvw.xy).r > 0.000001)return 0.1;\n"
	"return 1.0;\n"
"}\n"
"void main(){\n"
	"FragColor = vec4(ambient + phong()*shadow(), 1.0);\n"
"}\n";
/*
char* light_glsl_v =
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
*/




void fixfbo(struct style* cam, struct style* tar)
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
	cam->vu[0] = x*25.0;
	cam->vu[1] = y*25.0;
	cam->vu[2] = z*25.0;
	cam->vb[0] = -x*25.0;
	cam->vb[1] = -y*25.0;
	cam->vb[2] = -z*25.0;
}




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

	float a,c,s;
	void* mvp;
	struct relation* rel;
	struct arena* tmp;
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
	vbuf[1][3] = 0.0;	//1.0;
	vbuf[1][4] = 0.0;	//1.0;
	vbuf[1][5] = 1.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 0.0;	//1.0;
	vbuf[2][5] = 1.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 0.0;	//1.0;
	vbuf[3][4] = 0.0;	//1.0;
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
	vbuf[5][3] = 0.0;	//1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 1.0;

	vbuf[6][0] = vc[0] + vu[0];
	vbuf[6][1] = vc[1] + vu[1];
	vbuf[6][2] = vc[2] + vu[2];
	vbuf[6][3] = 1.0;
	vbuf[6][4] = 1.0;
	vbuf[6][5] = 1.0;

	vbuf[7][0] = vc[0] + vr[0]/2 + vu[0] / 3.0;
	vbuf[7][1] = vc[1] + vr[1]/2 + vu[1] / 3.0;
	vbuf[7][2] = vc[2] + vr[2]/2 + vu[2] / 3.0;
	vbuf[7][3] = 1.0;
	vbuf[7][4] = 1.0;
	vbuf[7][5] = 1.0;

	vbuf[8][0] = vc[0] + vf[0]/2 + vu[0] / 3.0;
	vbuf[8][1] = vc[1] + vf[1]/2 + vu[1] / 3.0;
	vbuf[8][2] = vc[2] + vf[2]/2 + vu[2] / 3.0;
	vbuf[8][3] = 1.0;
	vbuf[8][4] = 1.0;
	vbuf[8][5] = 1.0;

	vbuf[9][0] = vc[0];
	vbuf[9][1] = vc[1];
	vbuf[9][2] = vc[2];
	vbuf[9][3] = 0.0;
	vbuf[9][4] = 0.0;
	vbuf[9][5] = 1.0;

	vbuf[10][0] = vc[0] - vr[0]/2 + vu[0] * 2.0 / 3.0;
	vbuf[10][1] = vc[1] - vr[1]/2 + vu[1] * 2.0 / 3.0;
	vbuf[10][2] = vc[2] - vr[2]/2 + vu[2] * 2.0 / 3.0;
	vbuf[10][3] = 0.0;
	vbuf[10][4] = 0.0;
	vbuf[10][5] = 1.0;

	vbuf[11][0] = vc[0] - vf[0]/2 + vu[0] * 2.0 / 3.0;
	vbuf[11][1] = vc[1] - vf[1]/2 + vu[1] * 2.0 / 3.0;
	vbuf[11][2] = vc[2] - vf[2]/2 + vu[2] * 2.0 / 3.0;
	vbuf[11][3] = 0.0;
	vbuf[11][4] = 0.0;
	vbuf[11][5] = 1.0;

	src->vbuf_enq += 1;




	rel = act->orel0;
	if(0 == rel)return;

	tmp = (void*)(rel->dstchip);
	if(0 == tmp)return;
	if(_fbo_ != tmp->fmt)return;

	a = tau * timeread() / 10000000.0;
	c = cosine(a);
	s = sine(a);

	tmp->camera.vc[0] = sty->vc[0] + 1000.0 * c;
	tmp->camera.vc[1] = sty->vc[1] + 1000.0 * s;
	tmp->camera.vc[2] = sty->vc[2] + 1000.0;
	fixfbo(&tmp->camera, sty);

	mvp = (void*)(src->arg_data[0]);
	fixmatrix(mvp, tmp);
	mat4_transpose(mvp);

	src->arg_enq[0] += 1;




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
	void* mvp;
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

	mvp = memorycreate(4*4*4);
	src->arg[0] = "mapmvp";
	src->arg_data[0] = (u64)mvp;

	//vertex
	src->vbuf = memorycreate(4*6*12);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 12;
	src->method = 'v';

	//special
	rel = leaf->orel0;
	if(0 == rel)return;

	tmp = (void*)(rel->dstchip);
	if(0 == tmp)return;
	if(_fbo_ != tmp->fmt)return;

	say("tex=%x\n", tmp->tex_depth);
	dst->tex[0] = tmp->tex_depth;

	//send!
	src->shader_enq[0] = 42;
	src->arg_enq[0] = 0;
	src->tex_enq[0] = 0;
	src->vbuf_enq = 0;
	src->ibuf_enq = 0;
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
