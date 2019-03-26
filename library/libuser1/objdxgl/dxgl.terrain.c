#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void actorcreatefromfile(struct actor* act, char* name);




char* terrain_glsl_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec2 texuvw;\n"
	"uniform mat4 cammvp;\n"
	"out mediump vec2 uvw;\n"
	"void main()\n"
	"{\n"
		//"uvw = vec2(mod(texuvw.x, 1.0), mod(texuvw.y, 1.0));\n"
		"uvw = texuvw;\n"
		"gl_Position = cammvp * vec4(vertex, 1.0);\n"
	"}\n";
char* terrain_glsl_f =
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
	"}\n";




static void terrain_read_pixel(
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
static void terrain_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	if(0 == act->buf)return;

	struct glsrc* src = (void*)(pin->foot[0]);
	float (*vbuf)[6] = (void*)(src->vbuf);

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2] - 1.0;
	vbuf[0][3] =-vr[0] / 1000.0;
	vbuf[0][4] =-vf[1] / 1000.0;
	vbuf[0][5] = vr[0];

	vbuf[1][0] = vc[0] + vr[0] - vf[0];
	vbuf[1][1] = vc[1] + vr[1] - vf[1];
	vbuf[1][2] = vc[2] + vr[2] - vf[2] - 1.0;
	vbuf[1][3] = vr[0] / 1000.0;
	vbuf[1][4] =-vf[1] / 1000.0;
	vbuf[1][5] = vr[0];

	vbuf[2][0] = vc[0] + vr[0] + vf[0];
	vbuf[2][1] = vc[1] + vr[1] + vf[1];
	vbuf[2][2] = vc[2] + vr[2] + vf[2] - 1.0;
	vbuf[2][3] = vr[0] / 1000.0;
	vbuf[2][4] = vf[1] / 1000.0;
	vbuf[2][5] = vr[0];

	vbuf[3][0] = vc[0] - vr[0] - vf[0];
	vbuf[3][1] = vc[1] - vr[1] - vf[1];
	vbuf[3][2] = vc[2] - vr[2] - vf[2] - 1.0;
	vbuf[3][3] =-vr[0] / 1000.0;
	vbuf[3][4] =-vf[1] / 1000.0;
	vbuf[3][5] = vr[0];

	vbuf[4][0] = vc[0] - vr[0] + vf[0];
	vbuf[4][1] = vc[1] - vr[1] + vf[1];
	vbuf[4][2] = vc[2] - vr[2] + vf[2] - 1.0;
	vbuf[4][3] =-vr[0] / 1000.0;
	vbuf[4][4] = vf[1] / 1000.0;
	vbuf[4][5] = vr[0];

	vbuf[5][0] = vc[0] + vr[0] + vf[0];
	vbuf[5][1] = vc[1] + vr[1] + vf[1];
	vbuf[5][2] = vc[2] + vr[2] + vf[2] - 1.0;
	vbuf[5][3] = vr[0] / 1000.0;
	vbuf[5][4] = vf[1] / 1000.0;
	vbuf[5][5] = vr[0];

	src->vbuf_enq += 1;
/*
	for(y=-4;y<=4;y++)
	{
		for(x=-4;x<=4;x++)
		{
			t = 9*(y+4)*6 + (x+4)*6;
			vbuf[t+0][0] = 1000*x - 500;
			vbuf[t+0][1] = 1000*y - 500;
			vbuf[t+0][2] = 0;
			vbuf[t+0][3] = 0.0;
			vbuf[t+0][4] = 1.0;
			vbuf[t+0][5] = 0.0;

			vbuf[t+1][0] = 1000*x + 500;
			vbuf[t+1][1] = 1000*y + 500;
			vbuf[t+1][2] = 0;
			vbuf[t+1][3] = 1.0;
			vbuf[t+1][4] = 0.0;
			vbuf[t+1][5] = 0.0;

			vbuf[t+2][0] = 1000*x - 500;
			vbuf[t+2][1] = 1000*y + 500;
			vbuf[t+2][2] = 0;
			vbuf[t+2][3] = 0.0;
			vbuf[t+2][4] = 0.0;
			vbuf[t+2][5] = 0.0;

			vbuf[t+3][0] = 1000*x + 500;
			vbuf[t+3][1] = 1000*y + 500;
			vbuf[t+3][2] = 0;
			vbuf[t+3][3] = 1.0;
			vbuf[t+3][4] = 0.0;
			vbuf[t+3][5] = 0.0;

			vbuf[t+4][0] = 1000*x - 500;
			vbuf[t+4][1] = 1000*y - 500;
			vbuf[t+4][2] = 0;
			vbuf[t+4][3] = 0.0;
			vbuf[t+4][4] = 1.0;
			vbuf[t+4][5] = 0.0;

			vbuf[t+5][0] = 1000*x + 500;
			vbuf[t+5][1] = 1000*y - 500;
			vbuf[t+5][2] = 0;
			vbuf[t+5][3] = 1.0;
			vbuf[t+5][4] = 1.0;
			vbuf[t+5][5] = 0.0;
		}
	}
*/
}
static void terrain_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrain_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrain_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrain_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrain_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)terrain_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)terrain_read_tui(win, sty, act, pin);
	else if(fmt == _html_)terrain_read_html(win, sty, act, pin);
	else if(fmt == _json_)terrain_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)terrain_read_vbo(win, sty, act, pin);
	else terrain_read_pixel(win, sty, act, pin);
}
static void terrain_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void terrain_get()
{
}
static void terrain_post()
{
}
static void terrain_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void terrain_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	if(0 == lf)return;

	pair = alloc_winobj(root);
	src = &pair->src;
	dst = &pair->dst;
	lf->foot[0] = (u64)src;
	tf->foot[0] = (u64)dst;

	//shader
	src->vs = terrain_glsl_v;
	src->fs = terrain_glsl_f;

	//texture
	src->tex[0] = leaf->buf;
	src->tex_fmt[0] = hex32('r','g','b','a');
	src->tex_w[0] = leaf->width;
	src->tex_h[0] = leaf->height;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf = memorycreate(4*6 * 6);
	src->vbuf_w = 6*4;		//sizeof(float) * 6info
	src->vbuf_h = 6;		//6vert * 81blocks
	src->method = 'v';

	//send!
	src->shader_enq[0] = 42;
	src->arg_enq[0] = 0;
	src->tex_enq[0] = 42;
	src->vbuf_enq = 0;
	src->ibuf_enq = 0;
}
static void terrain_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void terrain_create(struct actor* act)
{
	if(0 == act)return;
	actorcreatefromfile(act, "datafile/jpg/terrain.jpg");
}




void terrain_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('t', 'e', 'r', 'r', 'a', 'i', 'n', 0);

	p->oncreate = (void*)terrain_create;
	p->ondelete = (void*)terrain_delete;
	p->onstart  = (void*)terrain_start;
	p->onstop   = (void*)terrain_stop;
	p->onget    = (void*)terrain_get;
	p->onpost   = (void*)terrain_post;
	p->onread   = (void*)terrain_read;
	p->onwrite  = (void*)terrain_write;
}
