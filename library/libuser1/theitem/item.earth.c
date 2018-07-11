#include "libuser.h"
void* allocofoot();
void actorcreatefromfile(struct actor* act, char* name);
void carveearth(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);




char* earth_glsl_v =
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec2 texuvw;\n"
	"uniform mat4 cammvp;\n"
	"out mediump vec2 uvw;\n"
	"void main()\n"
	"{\n"
		"uvw = texuvw;\n"
		"gl_Position = cammvp * vec4(vertex, 1.0);\n"
	"}\n";
char* earth_glsl_t = 0;
char* earth_glsl_g = 0;
char* earth_glsl_f = 
	"#version 300 es\n"
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
	"}\n";



static void earth_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 tmp;
	u32* dst;
	u32* src;
	int x,y,xmax,ymax,stride;
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
	if(0 == act->buf)return;

	xmax = act->width;
	if(xmax >= ww*2)xmax = ww*2;
	ymax = act->height;
	if(ymax >= hh*2)ymax = hh*2;
	stride = win->stride;
	for(y=0;y<ymax;y++)
	{
		dst = (win->buf) + (cy-hh+y)*stride*4 + (cx-ww)*4;
		src = (act->buf) + 4*y*(act->width);
		//say("y=%d,%llx,%llx\n",y,dst,src);
		if('b' == ((win->fmt)&0xff))
		{
			for(x=0;x<xmax;x++)dst[x] = src[x];
		}
		else
		{
			for(x=0;x<xmax;x++)
			{
				tmp = src[x];
				dst[x] = 0xff000000 | (tmp&0xff00) | ((tmp>>16)&0xff) | ((tmp&0xff)<<16);
			}
		}
	}
}
static void earth_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	void* vbuf;
	void* ibuf;
	struct ofoot* opin;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	if(0 == act->buf)return;

	opin = (void*)(pin->foot[0]);
	vbuf = (void*)(opin->vbuf);
	ibuf = (void*)(opin->ibuf);
	carveearth(vbuf, ibuf, vc, vr, vf, vu);
	opin->vbuf_enq += 1;
	opin->ibuf_enq += 1;
}
static void earth_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void earth_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"earth\" style=\"width:50%%;height:100px;float:left;background-color:#3368a9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void earth_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void earth_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("earth(%x,%x,%x)\n",win,act,sty);
}
static void earth_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)earth_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)earth_read_tui(win, sty, act, pin);
	else if(fmt == _html_)earth_read_html(win, sty, act, pin);
	else if(fmt == _json_)earth_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)earth_read_vbo(win, sty, act, pin);
	else earth_read_pixel(win, sty, act, pin);
}




static void earth_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}




static void earth_list()
{
}
static void earth_change()
{
}
static void earth_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void earth_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	struct ofoot* opin;
	if(0 == pin)return;

	//
	opin = allocofoot();

	//shader
	opin->vs = (u64)earth_glsl_v;
	opin->fs = (u64)earth_glsl_f;

	//texture
	opin->tex[0] = (u64)(act->buf);
	opin->tex_fmt[0] = hex32('r','g','b','a');
	opin->tex_w[0] = act->width;
	opin->tex_h[0] = act->height;

#define accx 16
#define accy 15
	//vertex
	opin->vbuf = (u64)memorycreate(4*6*(accx*accy+(accx-1)*2));
	opin->vbuf_fmt = vbuffmt_33;
	opin->vbuf_w = 4*6;
	opin->vbuf_h = accx*accy+(accx-1)*2;
	opin->ibuf = (u64)memorycreate(2*3*accy*(accx-1)*2);
	opin->ibuf_fmt = 0x222;
	opin->ibuf_w = 2*3;
	opin->ibuf_h = accy*(accx-1)*2;
	opin->method = 'i';

	//send!
	opin->shader_enq[0] = 42;
	opin->arg_enq[0] = 0;
	opin->tex_enq[0] = 42;
	opin->vbuf_enq = 0;
	opin->ibuf_enq = 0;
	pin->foot[0] = (u64)opin;
}
static void earth_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void earth_create(struct actor* act)
{
	if(0 == act)return;
	actorcreatefromfile(act, "earth/earth.jpg");
}




void earth_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('e', 'a', 'r', 't', 'h', 0, 0, 0);

	p->oncreate = (void*)earth_create;
	p->ondelete = (void*)earth_delete;
	p->onstart  = (void*)earth_start;
	p->onstop   = (void*)earth_stop;
	p->onlist   = (void*)earth_list;
	p->onchoose = (void*)earth_change;
	p->onread   = (void*)earth_read;
	p->onwrite  = (void*)earth_write;
}
