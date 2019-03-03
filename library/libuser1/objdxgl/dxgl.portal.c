#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void actorcreatefromfile(struct actor* act, char* name);




char* portal_glsl_v =
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
char* portal_glsl_f =
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"mediump vec4 tmp = texture(tex0, uvw);"
		"if(tmp.a < 0.1)discard;"
		"FragColor = tmp.bgra;\n"
	"}\n";




static void portal_read_pixel(
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
			for(x=0;x<xmax;x++)
			{
				tmp = src[x];
				if(tmp < 0x10)continue;
				dst[x] = tmp;
			}
		}
		else
		{
			for(x=0;x<xmax;x++)
			{
				tmp = src[x];
				if(tmp < 0x10)continue;
				dst[x] = 0xff000000 | (tmp&0xff00) | ((tmp>>16)&0xff) | ((tmp&0xff)<<16);
			}
		}
	}
}
static void portal_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j;
	float (*vbuf)[6];
	struct glsrc* src;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	if(0 == act->buf)return;

	src = (void*)(pin->foot[0]);
	vbuf = (void*)(src->vbuf);

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 1.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 0.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 0.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 0.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 1.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 1.0;
	vbuf[5][5] = 0.0;

	for(j=0;j<6;j++)
	{
		vbuf[6+j][0] = vbuf[j][0] + vu[0]*2;
		vbuf[6+j][1] = vbuf[j][1] + vu[1]*2;
		vbuf[6+j][2] = vbuf[j][2] + vu[2]*2;
		vbuf[6+j][3] = vbuf[j][3];
		vbuf[6+j][4] = vbuf[j][4];
		vbuf[6+j][5] = vbuf[j][5];
	}
	src->vbuf_enq += 1;
}
static void portal_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void portal_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void portal_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void portal_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void portal_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)portal_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)portal_read_tui(win, sty, act, pin);
	else if(fmt == _html_)portal_read_html(win, sty, act, pin);
	else if(fmt == _json_)portal_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)portal_read_vbo(win, sty, act, pin);
	else portal_read_pixel(win, sty, act, pin);
}
static void portal_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void portal_get()
{
}
static void portal_post()
{
}
static void portal_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void portal_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
	struct glsrc* src;
	if(0 == lf)return;

	//
	src = alloc_winobj(root);
	src->vs = portal_glsl_v;
	src->fs = portal_glsl_f;

	//texture
	src->tex[0] = leaf->buf;
	src->tex_fmt[0] = hex32('r','g','b','a');
	src->tex_w[0] = leaf->width;
	src->tex_h[0] = leaf->height;

	//vertex
	src->vbuf = memorycreate(4*6*6);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 12;
	src->method = 'v';

	//send!
	src->shader_enq[0] = 42;
	src->arg_enq[0] = 0;
	src->tex_enq[0] = 42;
	src->vbuf_enq = 0;
	src->ibuf_enq = 0;

	lf->foot[0] = (u64)src;
}
static void portal_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void portal_create(struct actor* act)
{
	if(0 == act)return;
	actorcreatefromfile(act, "png/portal.png");
}




void portal_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('p', 'o', 'r', 't', 'a', 'l', 0, 0);

	p->oncreate = (void*)portal_create;
	p->ondelete = (void*)portal_delete;
	p->onstart  = (void*)portal_start;
	p->onstop   = (void*)portal_stop;
	p->onget    = (void*)portal_get;
	p->onpost   = (void*)portal_post;
	p->onread   = (void*)portal_read;
	p->onwrite  = (void*)portal_write;
}
