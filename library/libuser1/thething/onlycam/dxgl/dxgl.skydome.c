#include "libuser.h"
#define CTXBUF buf0
void carveskydome(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);




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



static void skydome_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{/*
	u32 tmp;
	u32* dst;
	u32* src;
	int x,y,xmax,ymax,stride;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

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
	}*/
}
static void skydome_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* wnd, struct style* sty)
{
	struct glsrc* src = act->CTXBUF;
	if(0 == src)return;

	void* vbuf = src->vtx[0].vbuf;
	void* ibuf = src->vtx[0].ibuf;
	float* vc = sty->fs.vc;
	float* vr = sty->fs.vr;
	float* vf = sty->fs.vf;
	float* vu = sty->fs.vt;
	carveskydome(vbuf, ibuf, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;

	gl41data_insert(wnd, 's', src, 1);
}
static void skydome_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void skydome_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void skydome_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void skydome_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("skydome(%x,%x,%x)\n",win,act,sty);
}




static void skydome_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void skydome_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void skydome_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void skydome_linkup(struct halfrel* self, struct halfrel* peer)
{
/*
	//
	pair = alloc_winobj(win, 's');
	src = &pair->src;
	dst = &pair->dst;
	pin->foot[0] = (u64)src;
	sty->foot[0] = (u64)dst;

	//shader
	src->vs = skydome_glsl_v;
	src->fs = skydome_glsl_f;
	src->shader_enq = 42;

	//texture
	src->tex_name[0] = "tex0";
	src->tex_fmt[0] = hex32('r','g','b','a');
	src->tex_data[0] = act->buf;
	src->tex_w[0] = act->width;
	src->tex_h[0] = act->height;
	src->tex_enq[0] = 42;

	//vertex
	src->geometry = 3;
	src->opaque = 0;

	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*6;
	src->vbuf_h = 128+1;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);

	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 256;
	src->ibuf_len = (src->ibuf_w) * (src->ibuf_h);
	src->ibuf = memorycreate(src->ibuf_len, 0);
*/
}




static void skydome_search(struct entity* act)
{
}
static void skydome_modify(struct entity* act)
{
}
static void skydome_delete(struct entity* act)
{
	if(0 == act)return;
}
static void skydome_create(struct entity* act)
{
	if(0 == act)return;
	act->CTXBUF = memorycreate(0x400, 0);
}




void skydome_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'k', 'y', 'd', 'o', 'm', 'e', 0);

	p->oncreate = (void*)skydome_create;
	p->ondelete = (void*)skydome_delete;
	p->onsearch = (void*)skydome_search;
	p->onmodify = (void*)skydome_modify;

	p->onlinkup = (void*)skydome_linkup;
	p->ondiscon = (void*)skydome_discon;
	p->ontaking = (void*)skydome_taking;
	p->ongiving = (void*)skydome_giving;
}
