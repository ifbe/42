#include "libuser.h"
void actorcreatefromfile(struct actor* act, char* name);
void carveskydome(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);




char* skydome_glsl_v =
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
char* skydome_glsl_t = 0;
char* skydome_glsl_g = 0;
char* skydome_glsl_f = 
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
	"}\n";



static void skydome_read_pixel(
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
static void skydome_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	void* vbuf;
	void* ibuf;
	struct glsrc* src;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	if(0 == act->buf)return;

	src = (void*)(pin->foot[0]);
	vbuf = (void*)(src->vbuf);
	ibuf = (void*)(src->ibuf);
	carveskydome(vbuf, ibuf, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;
}
static void skydome_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void skydome_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"skydome\" style=\"width:50%%;height:100px;float:left;background-color:#3368a9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void skydome_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void skydome_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("skydome(%x,%x,%x)\n",win,act,sty);
}
static void skydome_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)skydome_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)skydome_read_tui(win, sty, act, pin);
	else if(fmt == _html_)skydome_read_html(win, sty, act, pin);
	else if(fmt == _json_)skydome_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)skydome_read_vbo(win, sty, act, pin);
	else skydome_read_pixel(win, sty, act, pin);
}
static void skydome_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void skydome_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void skydome_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void skydome_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void skydome_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	struct glsrc* src;
	if(0 == lf)return;

	//
	src = alloc_winobj(root, 's');
	lf->foot[0] = (u64)src;

	//
	src->geometry = 3;
	src->method = 'i';

	//shader
	src->vs = skydome_glsl_v;
	src->fs = skydome_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*6;
	src->vbuf_h = 128+1;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len);

	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 256;
	src->ibuf_len = (src->ibuf_w) * (src->ibuf_h);
	src->ibuf = memorycreate(src->ibuf_len);

	//texture
	src->tex_name[0] = "tex0";
	src->tex_data[0] = leaf->buf;
	src->tex_fmt[0] = hex32('r','g','b','a');
	src->tex_w[0] = leaf->width;
	src->tex_h[0] = leaf->height;
	src->tex_enq[0] = 42;
}
static void skydome_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void skydome_create(struct actor* act)
{
	if(0 == act)return;
	actorcreatefromfile(act, "jpg/skydome.jpg");
}




void skydome_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'k', 'y', 'd', 'o', 'm', 'e', 0);

	p->oncreate = (void*)skydome_create;
	p->ondelete = (void*)skydome_delete;
	p->onstart  = (void*)skydome_start;
	p->onstop   = (void*)skydome_stop;
	p->oncread  = (void*)skydome_cread;
	p->oncwrite = (void*)skydome_cwrite;
	p->onsread  = (void*)skydome_sread;
	p->onswrite = (void*)skydome_swrite;
}
