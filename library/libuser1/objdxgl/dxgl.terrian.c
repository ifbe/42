#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void actorcreatefromfile(struct actor* act, char* name);




char* terrian_glsl_v =
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
char* terrian_glsl_f =
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
	"}\n";




static void terrian_read_pixel(
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
static void terrian_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,t;
	float (*vbuf)[6];
	struct glsrc* src;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	if(0 == act->buf)return;

	src = (void*)(pin->foot[0]);
	vbuf = (void*)(src->vbuf);
	src->vbuf_enq += 1;

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
}
static void terrian_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrian_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrian_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrian_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrian_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)terrian_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)terrian_read_tui(win, sty, act, pin);
	else if(fmt == _html_)terrian_read_html(win, sty, act, pin);
	else if(fmt == _json_)terrian_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)terrian_read_vbo(win, sty, act, pin);
	else terrian_read_pixel(win, sty, act, pin);
}
static void terrian_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void terrian_get()
{
}
static void terrian_post()
{
}
static void terrian_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrian_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	if(0 == pin)return;

	//
	pair = alloc_winobj(win);
	src = &pair->src;
	dst = &pair->dst;
	sty->foot[0] = (u64)dst;
	pin->foot[0] = (u64)src;

	//shader
	src->vs = terrian_glsl_v;
	src->fs = terrian_glsl_f;

	//texture
	src->tex[0] = act->buf;
	src->tex_fmt[0] = hex32('r','g','b','a');
	src->tex_w[0] = act->width;
	src->tex_h[0] = act->height;

	//vertex
	src->vbuf = memorycreate(4*6 * 6*81);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;		//sizeof(float) * 6info
	src->vbuf_h = 6*81;		//6vert * 81blocks
	src->method = 'v';

	//send!
	src->shader_enq[0] = 42;
	src->arg_enq[0] = 0;
	src->tex_enq[0] = 42;
	src->vbuf_enq = 0;
	src->ibuf_enq = 0;
}
static void terrian_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void terrian_create(struct actor* act)
{
	if(0 == act)return;
	actorcreatefromfile(act, "jpg/terrian.jpg");
}




void terrian_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('t', 'e', 'r', 'r', 'i', 'a', 'n', 0);

	p->oncreate = (void*)terrian_create;
	p->ondelete = (void*)terrian_delete;
	p->onstart  = (void*)terrian_start;
	p->onstop   = (void*)terrian_stop;
	p->onget    = (void*)terrian_get;
	p->onpost   = (void*)terrian_post;
	p->onread   = (void*)terrian_read;
	p->onwrite  = (void*)terrian_write;
}
