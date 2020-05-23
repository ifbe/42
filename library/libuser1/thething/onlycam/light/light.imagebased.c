#include "libuser.h"
#define CTXBUF buf0
#define LITBUF buf1
void carveplanet(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);




char* imagelight_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out mediump vec2 uvw;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";
char* imagelight_glsl_t = 0;
char* imagelight_glsl_g = 0;
char* imagelight_glsl_f = 
GLSL_VERSION
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D iblenvmap;\n"
"void main(){\n"
	"FragColor = vec4(texture(iblenvmap, uvw).bgr, 1.0);\n"
	//"FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\n";




void imagelight_litforwnd(struct glsrc* src, char* str)
{
	src->routine_name = "passtype";
	src->routine_detail = "imagelight";

	//texture
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].name = "iblenvmap";
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(&src->tex[0], str);
	src->tex[0].enq = 42;
	//say("w=%d,h=%d\n",src->tex[0].w, src->tex[0].h);
}
static void imagelight_lightupdate(
	struct entity* act, struct style* slot,
	struct entity* wnd, struct style* area)
{
	wnd->gl_light[0] = act->LITBUF;
}



void imagelight_ctxforwnd(struct glsrc* src, char* str)
{
	//shader
	src->vs = imagelight_glsl_v;
	src->fs = imagelight_glsl_f;
	src->shader_enq = 42;

#define accx 64
#define accy 63
	//vertex
	src->geometry = 3;
	src->opaque = 0;

	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*6;
	src->vbuf_h = accx*accy+(accx-1)*2;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
	src->vbuf_enq = 0;

	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = accy*(accx-1)*2;
	src->ibuf_len = (src->ibuf_w) * (src->ibuf_h);
	src->ibuf = memorycreate(src->ibuf_len, 0);
	src->ibuf_enq = 0;
}
static void imagelight_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* none)
{
	void* vbuf;
	void* ibuf;
	struct glsrc* src;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;

	src = act->CTXBUF;
	if(0 == src)return;

	vbuf = (void*)(src->vbuf);
	ibuf = (void*)(src->ibuf);
	carveplanet(vbuf, ibuf, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;

	gl41data_insert(ctx, 's', src, 1);
}




static void imagelight_draw_pixel(
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
static void imagelight_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void imagelight_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void imagelight_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void imagelight_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("imagelight(%x,%x,%x)\n",win,act,sty);
}
static void imagelight_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
}




static void imagelight_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		imagelight_draw_gl41(ent,slot, wor,geom, wnd,area);
		imagelight_lightupdate(ent,slot, wnd,area);
	}
}
static void imagelight_read_bywnd(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}




static void imagelight_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct supply* sup = stack[sp-2].pchip;
	switch(sup->fmt){
	case _gl41fbog_:
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		if('v' != key)break;
		imagelight_read_bywnd(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	default:{
		imagelight_read_bycam(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	}
}
static void imagelight_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void imagelight_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void imagelight_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@imagelight_linkup\n");
}




static void imagelight_search(struct entity* act)
{
}
static void imagelight_modify(struct entity* act)
{
}
static void imagelight_delete(struct entity* act)
{
	if(0 == act)return;
}
static void imagelight_create(struct entity* act, void* str)
{
	void* buf;
	if(0 == act)return;

	buf = memorycreate(0x1000, 0);
	if(0 == buf)return;

	act->CTXBUF = buf;
	act->LITBUF = buf+0x400;

	if(0 == str)str = "datafile/jpg/texball-earth.jpg";
	imagelight_ctxforwnd(act->CTXBUF, str);
	imagelight_litforwnd(act->LITBUF, str);
}




void imagelight_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('i','m','a','g','e','l','i','t');

	p->oncreate = (void*)imagelight_create;
	p->ondelete = (void*)imagelight_delete;
	p->onsearch = (void*)imagelight_search;
	p->onmodify = (void*)imagelight_modify;

	p->onlinkup = (void*)imagelight_linkup;
	p->ondiscon = (void*)imagelight_discon;
	p->onread  = (void*)imagelight_read;
	p->onwrite = (void*)imagelight_write;
}
