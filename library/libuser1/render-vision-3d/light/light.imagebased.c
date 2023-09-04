#include "libuser.h"
#define CTXBUF listptr.buf0
#define LITBUF listptr.buf1
void carveplanet(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);




char* imagelight_glsl_v =
GLSL_VERSION
GLSL_PRECISION
"layout(location = 0)in vec3 vertex;\n"
"layout(location = 1)in vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out vec2 uvw;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";
char* imagelight_glsl_t = 0;
char* imagelight_glsl_g = 0;
char* imagelight_glsl_f = 
GLSL_VERSION
GLSL_PRECISION
"in vec2 uvw;\n"
"out vec4 FragColor;\n"
"uniform sampler2D iblenvmap;\n"
"void main(){\n"
	"FragColor = vec4(texture(iblenvmap, uvw).bgr, 1.0);\n"
	//"FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\n";




void imagelight_litforwnd(struct gl41data* data, char* str)
{
	data->dst.lighttype = _imagelight_;

	//texture
	data->dst.texname[0] = "iblenvmap";
	data->src.tex[0].fmt = hex32('r','g','b','a');
	data->src.tex[0].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(&data->src.tex[0], str);
	data->src.tex_enq[0] = 42;
	//say("w=%d,h=%d\n",data->src.tex[0].w, data->src.tex[0].h);
}
static void imagelight_lightupdate(
	_obj* act, struct style* slot,
	_obj* wnd, struct style* area)
{
	wnd->gl41list.world[0].light[0] = act->LITBUF;
}



void imagelight_ctxforwnd(struct mysrc* src, char* str)
{
	//shader
	src->vs = imagelight_glsl_v;
	src->fs = imagelight_glsl_f;
	src->shader_enq = 42;

#define accx 64
#define accy 63
	struct vertex* vtx = src->vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 4*6;
	vtx->vbuf_h = accx*accy+(accx-1)*2;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);

	vtx->ibuf_fmt = 0x222;
	vtx->ibuf_w = 2*3;
	vtx->ibuf_h = accy*(accx-1)*2;
	vtx->ibuf_len = (vtx->ibuf_w) * (vtx->ibuf_h);
	vtx->ibuf = memorycreate(vtx->ibuf_len, 0);

	src->vbuf_enq = 0;
	src->ibuf_enq = 0;
}
static void imagelight_draw_gl41(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* none)
{
	struct mysrc* src = act->CTXBUF;
	if(0 == src)return;

	void* vbuf = src->vtx[0].vbuf;
	void* ibuf = src->vtx[0].ibuf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	carveplanet(vbuf, ibuf, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;

	gl41data_insert(ctx, 's', src, 1);
}




static void imagelight_draw_pixel(
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
		//say("y=%d,%llx,%llx\n",y,dst,src);
		if('b' == ((win->hfmt)&0xff))
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
static void imagelight_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void imagelight_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void imagelight_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void imagelight_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("imagelight(%x,%x,%x)\n",win,act,sty);
}
static void imagelight_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
}




static void imagelight_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	imagelight_draw_gl41(ent,slot, wor,geom, wnd,area);
	imagelight_lightupdate(ent,slot, wnd,area);
}
static void imagelight_wnd(_obj* ent,void* foot, _syn* stack,int sp)
{
}




static void imagelight_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		imagelight_wnd(ent,slot, stack,sp);
		break;
	default:
		imagelight_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void imagelight_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void imagelight_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void imagelight_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@imagelight_attach\n");
}




static void imagelight_search(_obj* act)
{
}
static void imagelight_modify(_obj* act)
{
}
static void imagelight_delete(_obj* act)
{
	if(0 == act)return;
}
static void imagelight_create(_obj* act, void* str)
{
	void* buf;
	if(0 == act)return;

	buf = memorycreate(0x1000, 0);
	if(0 == buf)return;

	act->CTXBUF = buf;
	act->LITBUF = buf+0x800;

	if(0 == str)str = "datafile/jpg/texball-earth.jpg";
	imagelight_ctxforwnd(act->CTXBUF, str);
	imagelight_litforwnd(act->LITBUF, str);
}




void imagelight_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('i','m','a','g','e','l','i','t');

	p->oncreate = (void*)imagelight_create;
	p->ondelete = (void*)imagelight_delete;
	p->onreader = (void*)imagelight_search;
	p->onwriter = (void*)imagelight_modify;

	p->onattach = (void*)imagelight_attach;
	p->ondetach = (void*)imagelight_detach;
	p->ontaking = (void*)imagelight_taking;
	p->ongiving = (void*)imagelight_giving;
}
