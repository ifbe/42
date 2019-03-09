#include "libuser.h"
int windowread(int type, void* buf);
int windowwrite(int type, void* buf);
void actorcreatefromfile(struct actor* act, char* name);




char* model_glsl2d_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 normal;\n"
	"uniform mat4 objmat;\n"
	"out mediump vec3 vcolor;\n"
	"void main()\n"
	"{\n"
		"vcolor = normal;\n"
		"gl_Position = objmat * vec4(vertex, 1.0);\n"
	"}\n";
char* model_glsl_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 normal;\n"
	"uniform mediump vec3 eyepos;\n"
	"uniform mat4 cammvp;\n"
	"uniform mat4 objmat;\n"
	"mediump vec3 ambient = vec3(0.25, 0.25, 0.25);\n"
	"mediump vec3 lightcolor = vec3(1.0, 1.0, 1.0);\n"
	"mediump vec3 lightposition = vec3(0.0, 0.0, 1000.0);\n"
	"out mediump vec3 vcolor;\n"
	"void main()\n"
	"{\n"
		"mediump vec3 N = normalize(normal);\n"
		"mediump vec3 L = normalize(vec3(lightposition - vertex));\n"
		"mediump vec3 E = normalize(eyepos-vertex);\n"
		"mediump vec3 R = reflect(-L, N);\n"
		"mediump float SN = max(dot(N, L), 0.0);\n"
		"mediump float RV = max(dot(R, E), 0.0);\n"
		"mediump vec3 diffuse = lightcolor * SN;\n"
		"mediump vec3 specular = vec3(0.0, 0.0, 0.0);\n"
		"if(SN>0.0)specular = lightcolor * pow(RV, 4.0);\n"
		"vcolor = ambient + diffuse + specular;\n"
		"gl_Position = cammvp * objmat * vec4(vertex, 1.0);\n"
	"}\n";
char* model_glsl_f =
	GLSL_VERSION
	"in mediump vec3 vcolor;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vcolor, 1.0);\n"
	"}\n";
void sty_sty_mat(struct style* src, struct style* dst, mat4 mat)
{
	float x,y,z,max;
	float nr, nf, nu;
	mat4 tmp;

	//move: (0,0,0) -> dst
	mat[0][0] = 1.0;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = dst->vc[0];
	mat[1][0] = 0.0;
	mat[1][1] = 1.0;
	mat[1][2] = 0.0;
	mat[1][3] = dst->vc[1];
	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 1.0;
	mat[2][3] = dst->vc[2];
	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;

	//rotate: 1 -> dst

	//scale: 1 -> dst
	x = dst->vr[0];
	y = dst->vr[1];
	z = dst->vr[2];
	nr = squareroot(x*x+y*y+z*z);

	x = dst->vf[0];
	y = dst->vf[1];
	z = dst->vf[2];
	nf = squareroot(x*x+y*y+z*z);

	x = dst->vu[0];
	y = dst->vu[1];
	z = dst->vu[2];
	nu = squareroot(x*x+y*y+z*z);

	max = nr;
	if(max < nf)max = nf;
	if(max < nu)max = nu;

	tmp[0][0] = max;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = 0.0;
	tmp[1][0] = 0.0;
	tmp[1][1] = max;
	tmp[1][2] = 0.0;
	tmp[1][3] = 0.0;
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = max;
	tmp[2][3] = 0.0;
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);

	//scale: src -> 1
	x = src->vr[0];
	y = src->vr[1];
	z = src->vr[2];
	nr = squareroot(x*x+y*y+z*z);

	x = src->vf[0];
	y = src->vf[1];
	z = src->vf[2];
	nf = squareroot(x*x+y*y+z*z);

	x = src->vu[0];
	y = src->vu[1];
	z = src->vu[2];
	nu = squareroot(x*x+y*y+z*z);

	max = nr;
	if(max < nf)max = nf;
	if(max < nu)max = nu;

	tmp[0][0] = 1.0 / max;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = 0.0;
	tmp[1][0] = 0.0;
	tmp[1][1] = 1.0 / max;
	tmp[1][2] = 0.0;
	tmp[1][3] = 0.0;
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = 1.0 / max;
	tmp[2][3] = 0.0;
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);

	//rotate: src -> 1

	//move: src -> (0,0,0)
	tmp[0][0] = 1.0;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = - src->vc[0];
	tmp[1][0] = 0.0;
	tmp[1][1] = 1.0;
	tmp[1][2] = 0.0;
	tmp[1][3] = - src->vc[1];
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = 1.0;
	tmp[2][3] = - src->vc[2];
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);

	mat4_transpose(mat);
}



static void model_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* p;
	float f;
	float v[3][3];
	int j,ret;
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
/*
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);
	if(0 == (act->buf))return;
	if(0 == (act->len))return;

	if((act->width) > (act->height))f = 2.0*ww/(act->width);
	else f = 2.0*hh/(act->height);

	ret = *(u32*)((act->buf)+80);
	ret = ret % ((0x800000-0x84)/50);
	for(j=0;j<ret;j++)
	{
		p = (void*)(act->buf) + 84 + j*50;

		v[0][0] = cx + (p[ 3]-(act->cx))*f;
		v[0][1] = cy + (p[ 4]-(act->cy))*f;
		v[1][0] = cx + (p[ 6]-(act->cx))*f;
		v[1][1] = cy + (p[ 7]-(act->cy))*f;
		v[2][0] = cx + (p[ 9]-(act->cx))*f;
		v[2][1] = cy + (p[10]-(act->cy))*f;

		drawline(win, 0xffffff, v[0][0], v[0][1], v[1][0], v[1][1]);
		drawline(win, 0xffffff, v[0][0], v[0][1], v[2][0], v[2][1]);
		drawline(win, 0xffffff, v[1][0], v[1][1], v[2][0], v[2][1]);
	}
*/
}
static void model_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	struct glsrc* src;
	if(act->buf == 0)return;
	if(0 == sty)sty = defaultstyle_vbo2d();

	src = (void*)(pin->foot[0]);
	sty_sty_mat(&act->target, sty, (void*)src->arg_data[0]); 
	src->arg_enq[0] += 1;
}
static void model_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	struct glsrc* src;
	if(act->buf == 0)return;

	src = (void*)(pin->foot[0]);
	sty_sty_mat(&act->target, sty, (void*)src->arg_data[0]); 
	src->arg_enq[0] += 1;
}
static void model_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void model_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void model_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void model_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("model(%x,%x,%x)\n",win,act,sty);
}
static void model_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)model_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)model_read_tui(win, sty, act, pin);
	else if(fmt == _html_)model_read_html(win, sty, act, pin);
	else if(fmt == _json_)model_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)model_read_vbo2d(win, sty, act, pin);
		else model_read_vbo3d(win, sty, act, pin);
	}
	else model_read_pixel(win, sty, act, pin);
}




static void model_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int j,ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(0x2d70 == type)
	{
		int x = key&0xffff;
		int y = (key>>16)&0xffff;
	}
	else if(_drag_ == type)
	{
		char buffer[0x1000];
		ret = windowread(type, buffer);
		say("%s", buffer);

		for(j=0;j<ret;j++)
		{
			if(buffer[j] < 0x20)
			{
				buffer[j] = 0;
				break;
			}
		}
		actorcreatefromfile(act, buffer);
	}
}




static void model_get()
{
}
static void model_post()
{
}
static void model_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void model_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	if(0 == lf)return;

	//alloc
	pair = alloc_winobj(root);
	src = &pair->src;
	dst = &pair->dst;
	lf->foot[0] = (u64)src;
	tf->foot[0] = (u64)dst;

	//shader
	src->vs = model_glsl_v;
	src->fs = model_glsl_f;
	if(twig){if(_fg2d_ == twig->fmt)src->vs = model_glsl2d_v;}

	//argument
	src->arg[0] = "objmat";
	src->arg_data[0] = (u64)memorycreate(4*4*4);

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf = leaf->buf;
	src->vbuf_w = leaf->width;
	src->vbuf_h = leaf->height;
	src->method = 'v';

	//send!
	src->shader_enq[0] = 42;
	src->arg_enq[0] = 0;
	src->tex_enq[0] = 0;
	src->vbuf_enq = 42;
	src->ibuf_enq = 0;
}
static void model_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void model_create(struct actor* act)
{
	if(0 == act)return;
	actorcreatefromfile(act, "42.stl");
}




void model_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('m', 'o', 'd', 'e', 'l', 0, 0, 0);

	p->oncreate = (void*)model_create;
	p->ondelete = (void*)model_delete;
	p->onstart  = (void*)model_start;
	p->onstop   = (void*)model_stop;
	p->onget    = (void*)model_get;
	p->onpost   = (void*)model_post;
	p->onread   = (void*)model_read;
	p->onwrite  = (void*)model_write;
}
