#include "libuser.h"
void actorcreatefromfile(struct actor* act, char* name);
void carveplanet(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);




char* texball_glsl2d_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec2 texuvw;\n"
	"out mediump vec2 uvw;\n"
	"void main()\n"
	"{\n"
		"uvw = texuvw;\n"
		"gl_Position = vec4(vertex, 1.0);\n"
	"}\n";
char* texball_glsl_v =
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
char* texball_glsl_t = 0;
char* texball_glsl_g = 0;
char* texball_glsl_f = 
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
	"}\n";



static void texball_draw_pixel(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u32 tmp;
	u32* dst;
	u32* src;
	int x,y,xmax,ymax,stride;
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
static void texball_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	void* vbuf;
	void* ibuf;
	struct glsrc* src;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	if(0 == act->buf)return;

	src = (void*)(pin->foot[0]);
	vbuf = (void*)(src->vbuf);
	ibuf = (void*)(src->ibuf);
	carveplanet(vbuf, ibuf, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;
}
static void texball_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	void* vbuf;
	void* ibuf;
	struct glsrc* src;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	if(0 == act->buf)return;

	src = (void*)(pin->foot[0]);
	vbuf = (void*)(src->vbuf);
	ibuf = (void*)(src->ibuf);
	carveplanet(vbuf, ibuf, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;
}
static void texball_draw_json(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void texball_draw_html(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"texball\" style=\"width:50%%;height:100px;float:left;background-color:#3368a9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void texball_draw_tui(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void texball_draw_cli(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	say("texball(%x,%x,%x)\n",win,act,sty);
}
static void texball_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)texball_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)texball_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)texball_draw_html(act, pin, win, sty);
	else if(fmt == _json_)texball_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)texball_draw_vbo2d(act, pin, win, sty);
		else texball_draw_vbo3d(act, pin, win, sty);
	}
	else texball_draw_pixel(act, pin, win, sty);
}
static void texball_event(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	struct glsrc* src = (void*)(pin->foot[0]);

	if(_char_ == ev->what)
	{
		switch(ev->why)
		{
			case '1':
			{
				actorcreatefromfile(act, "jpg/universe.jpg");
				src->tex_data[0] = act->buf;
				src->tex_w[0] = act->width;
				src->tex_h[0] = act->height;
				src->tex_enq[0] += 1;
				break;
			}
			case '2':
			{
				actorcreatefromfile(act, "jpg/skysphere.jpg");
				src->tex_data[0] = act->buf;
				src->tex_w[0] = act->width;
				src->tex_h[0] = act->height;
				src->tex_enq[0] += 1;
				break;
			}
			case '3':
			{
				actorcreatefromfile(act, "jpg/thesun.jpg");
				src->tex_data[0] = act->buf;
				src->tex_w[0] = act->width;
				src->tex_h[0] = act->height;
				src->tex_enq[0] += 1;
				break;
			}
			case '4':
			{
				actorcreatefromfile(act, "jpg/earth.jpg");
				src->tex_data[0] = act->buf;
				src->tex_w[0] = act->width;
				src->tex_h[0] = act->height;
				src->tex_enq[0] += 1;
				break;
			}
		}
	}
}




static void texball_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	texball_draw(act, pin, win, sty);
}
static void texball_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	texball_event(act, pin, win, sty, ev, 0);
}
static void texball_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void texball_start(struct halfrel* self, struct halfrel* peer)
{
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
say("@texball_start\n");
	//
	pair = alloc_winobj(win, 's');
	src = &pair->src;
	dst = &pair->dst;
	pin->foot[0] = (u64)src;
	sty->foot[0] = (u64)dst;

	//
	src->geometry = 3;
	src->method = 'i';

	//shader
	src->vs = texball_glsl_v;
	src->fs = texball_glsl_f;
	src->shader_enq = 42;

#define accx 64
#define accy 63
	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*6;
	src->vbuf_h = accx*accy+(accx-1)*2;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_enq = 0;

	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = accy*(accx-1)*2;
	src->ibuf_len = (src->ibuf_w) * (src->ibuf_h);
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_enq = 0;

	//texture
	src->tex_name[0] = "tex0";
	src->tex_fmt[0] = hex32('r','g','b','a');
	src->tex_data[0] = act->buf;
	src->tex_w[0] = act->width;
	src->tex_h[0] = act->height;
	src->tex_enq[0] = 42;
}




static void texball_search(struct actor* act)
{
}
static void texball_modify(struct actor* act)
{
}
static void texball_delete(struct actor* act)
{
	if(0 == act)return;
	if(0 == act->buf){
		memorydelete(act->buf);
		act->buf = 0;
	}
}
static void texball_create(struct actor* act, void* str)
{
	if(0 == act)return;

	//max=16MB
	if(0 == act->buf)act->buf = memorycreate(2048*2048*4);

	if(0 == str)str = "datafile/jpg/earth.jpg";
	actorcreatefromfile(act, str);
}




void texball_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'e', 'x', 'b', 'a', 'l', 'l', 0);

	p->oncreate = (void*)texball_create;
	p->ondelete = (void*)texball_delete;
	p->onsearch = (void*)texball_search;
	p->onmodify = (void*)texball_modify;

	p->onstart = (void*)texball_start;
	p->onstop  = (void*)texball_stop;
	p->onread  = (void*)texball_read;
	p->onwrite = (void*)texball_write;
}
