#include "libuser.h"




char* particle_glsl_v =
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
char* particle_glsl_f =
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"mediump float k = uvw.x*uvw.x+uvw.y*uvw.y;\n"
		"FragColor = vec4(vec3(1.0,1.0,1.0)*k, 1.0);\n"
		//"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
		//"FragColor = vec4(uvw, 1.0, 1.0);\n"
	"}\n";




static void particle_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void particle_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void particle_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j;
	float x,y,z;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

	struct datapair* mod = win->mod;
	struct glsrc* src = &mod[point3d].src;
	int vlen = src->vbuf_h;

	float* sbuf = act->buf;
	float* dbuf = (src->vbuf) + (24*vlen);
	src->vbuf_h += 30000;

	for(j=0;j<30000;j++)
	{
		x = sbuf[9*j + 0] + sbuf[9*j + 3];
		if((x < -1.0) | (x > 1.0))sbuf[9*j + 3] = - sbuf[9*j + 3];
		sbuf[9*j + 0] += sbuf[9*j + 3];

		y = sbuf[9*j + 1] + sbuf[9*j + 4];
		if((y < -1.0) | (y > 1.0))sbuf[9*j + 4] = - sbuf[9*j + 4];
		sbuf[9*j + 1] += sbuf[9*j + 4];

		z = sbuf[9*j + 2] + sbuf[9*j + 5];
		if((z < -1.0) | (z > 1.0))sbuf[9*j + 5] = - sbuf[9*j + 5];
		sbuf[9*j + 2] += sbuf[9*j + 5];

		x = sbuf[9*j + 0];
		y = sbuf[9*j + 1];
		z = sbuf[9*j + 2];
		dbuf[6*j + 0] = vc[0] + x*vr[0] + y*vf[0] + z*vu[0];
		dbuf[6*j + 1] = vc[1] + x*vr[1] + y*vf[1] + z*vu[1];
		dbuf[6*j + 2] = vc[2] + x*vr[2] + y*vf[2] + z*vu[2];

		dbuf[6*j + 3] = sbuf[9*j + 6];
		dbuf[6*j + 4] = sbuf[9*j + 7];
		dbuf[6*j + 5] = sbuf[9*j + 8];
	}
}
static void particle_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void particle_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void particle_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void particle_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("particle(%x,%x,%x)\n",win,act,sty);
}
static void particle_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)particle_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)particle_read_tui(win, sty, act, pin);
	else if(fmt == _html_)particle_read_html(win, sty, act, pin);
	else if(fmt == _json_)particle_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)particle_read_vbo2d(win, sty, act, pin);
		else particle_read_vbo3d(win, sty, act, pin);
	}
	else particle_read_pixel(win, sty, act, pin);
}
static void particle_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void particle_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void particle_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void particle_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void particle_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
	int j;
	float* vbuf = leaf->buf;
	for(j=0;j<30000;j++)
	{
		//vertex
		vbuf[9*j + 0] = (getrandom()%8192)/4096.0 - 1.0;
		vbuf[9*j + 1] = (getrandom()%8192)/4096.0 - 1.0;
		vbuf[9*j + 2] = (getrandom()%8192)/4096.0 - 1.0;

		//speed
		vbuf[9*j + 3] = (getrandom()%8191 - 4095)/4096000.0;
		vbuf[9*j + 4] = (getrandom()%8191 - 4095)/4096000.0;
		vbuf[9*j + 5] = (getrandom()%8191 - 4095)/4096000.0;

		//colour
		vbuf[9*j + 6] = (getrandom()%8192)/8192.0;
		vbuf[9*j + 7] = (getrandom()%8192)/8192.0;
		vbuf[9*j + 8] = (getrandom()%8192)/8192.0;
	}
}
static void particle_delete(struct actor* act)
{
	if(0 == act)return;
	if(act->buf)
	{
		memorydelete(act->buf);
		act->buf = 0;
	}
}
static void particle_create(struct actor* act)
{
	if(0 == act)return;
	act->buf = memorycreate(4*9 * 30000);
}




void particle_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'a', 'r', 't', 'i', 'c', 'l', 'e');

	p->oncreate = (void*)particle_create;
	p->ondelete = (void*)particle_delete;
	p->onstart  = (void*)particle_start;
	p->onstop   = (void*)particle_stop;
	p->onget    = (void*)particle_cread;
	p->onpost   = (void*)particle_cwrite;
	p->onread   = (void*)particle_sread;
	p->onwrite  = (void*)particle_swrite;
}
