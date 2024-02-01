#include "libuser.h"
#define CTXBUF listptr.buf0
void carveskydome(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);




char* skydome_glsl_v =
GLSL_VERSION
GLSL_PRECISION
"layout(location = 0)in vec3 vertex;\n"
"layout(location = 1)in vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out vec2 uvw;\n"
"void main()\n"
"{\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";
char* skydome_glsl_t = 0;
char* skydome_glsl_g = 0;
char* skydome_glsl_f = 
GLSL_VERSION
GLSL_PRECISION
"uniform sampler2D tex0;\n"
"in vec2 uvw;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
	"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
"}\n";



static void skydome_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
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
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}

	xmax = act->whdf.width;
	if(xmax >= ww*2)xmax = ww*2;
	ymax = act->whdf.height;
	if(ymax >= hh*2)ymax = hh*2;
	stride = win->stride;
	for(y=0;y<ymax;y++)
	{
		dst = (win->buf) + (cy-hh+y)*stride*4 + (cx-ww)*4;
		src = (act->buf) + 4*y*(act->whdf.width);
		//logtoall("y=%d,%llx,%llx\n",y,dst,src);
		if('b' == ((win->type)&0xff))
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
	_obj* act, struct style* pin,
	_obj* wnd, struct style* sty)
{
	struct mysrc* src = act->CTXBUF;
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
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void skydome_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void skydome_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void skydome_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("skydome(%x,%x,%x)\n",win,act,sty);
}




static void skydome_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
}
static void skydome_read_byworld_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
}
static void skydome_read_bywnd(_obj* ent,void* slot, _obj* wnd,void* area)
{
	switch(wnd->vfmt){
	case _bgra8888_:
	case _rgba8888_:
		break;
	case _gl41list_:
		break;
	}
}




static void skydome_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
		skydome_read_bywnd(ent,slot, caller,area);
		break;
	default:
		skydome_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void skydome_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void skydome_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void skydome_attach(struct halfrel* self, struct halfrel* peer)
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
	src->tex_w[0] = act->whdf.width;
	src->tex_h[0] = act->whdf.height;
	src->tex_enq[0] = 42;

	//vertex
	src->geometry = 3;
	src->opaque = 0;

	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*6;
	src->vbuf_h = 128+1;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memoryalloc(src->vbuf_len, 0);

	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 256;
	src->ibuf_len = (src->ibuf_w) * (src->ibuf_h);
	src->ibuf = memoryalloc(src->ibuf_len, 0);
*/
}




static void skydome_search(_obj* act)
{
}
static void skydome_modify(_obj* act)
{
}
static void skydome_delete(_obj* act)
{
	if(0 == act)return;
}
static void skydome_create(_obj* act)
{
	if(0 == act)return;
	act->CTXBUF = memoryalloc(0x1000, 0);
}




void skydome_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('s', 'k', 'y', 'd', 'o', 'm', 'e', 0);

	p->oncreate = (void*)skydome_create;
	p->ondelete = (void*)skydome_delete;
	p->onreader = (void*)skydome_search;
	p->onwriter = (void*)skydome_modify;

	p->onattach = (void*)skydome_attach;
	p->ondetach = (void*)skydome_detach;
	p->ontaking = (void*)skydome_taking;
	p->ongiving = (void*)skydome_giving;
}
