#include "libuser.h"
#define nbuf buf0
#define wbuf buf1
#define vbuf buf2
#define ibuf buf3
#define _data_ hex32('d','a','t','a')
struct perobj{
	void* nbuf;
	void* wbuf;
	void* vbuf;
	void* ibuf;

	int nlen;
	int wlen;
	int vlen;
	int ilen;

	float xmin;
	float xmax;
	float ymin;
	float ymax;
	float zmin;
	float zmax;
};




char* calib3d_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char* calib3d_glsl_f =
GLSL_VERSION
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(1.0, 0.0, 1.0, 1.0);\n"
"}\n";




static void calib3d_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
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
}
static void calib3d_draw_gl41(
	_obj* act, struct style* slot,
	_obj* wrl, struct style* geom,
	_obj* wnd, struct style* area)
{
}
static void calib3d_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void calib3d_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void calib3d_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void calib3d_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void calib3d_data(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	float* buf, int len)
{
	int j;
	struct perobj* perobj = (void*)act->priv_256b;
	int t = perobj->vlen;
	float* f = perobj->vbuf;
	//logtoall("@calib3d_data:%d\n", len);

	for(j=0;j<len;j+=3){
		f[3*t + j+0] = buf[j+0];
		f[3*t + j+1] = buf[j+1];
		f[3*t + j+2] = buf[j+2];
	}
	perobj->vlen = (perobj->vlen + len/3) % 0x10000;

	if(buf[0] < perobj->xmin)perobj->xmin = buf[0];
	if(buf[0] > perobj->xmax)perobj->xmax = buf[0];
	if(buf[1] < perobj->ymin)perobj->ymin = buf[1];
	if(buf[1] > perobj->ymax)perobj->ymax = buf[1];
	if(buf[2] < perobj->zmin)perobj->zmin = buf[2];
	if(buf[2] > perobj->zmax)perobj->zmax = buf[2];
	logtoall("%f,%f,%f,%f,%f,%f\n",perobj->xmin,perobj->xmax,perobj->ymin,perobj->ymax,perobj->zmin,perobj->zmax);
}




static void calib3d_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	calib3d_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void calib3d_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		break;
	default:
		calib3d_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void calib3d_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void calib3d_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void calib3d_attach(struct halfrel* self, struct halfrel* peer)
{
	struct datapair* pair;
	struct mysrc* src;
	struct gldst* dst;
	_obj* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	_obj* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	if(_data_ == self->foottype)return;
/*
	//
	pair = alloc_winobj(win, 's');
	src = &pair->src;
	dst = &pair->dst;
	pin->foot[0] = (u64)src;
	sty->foot[0] = (u64)dst;

	//
	src->geometry = 2;
	src->method = 'i';

	//
	src->vs = calib3d_glsl_v;
	src->fs = calib3d_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf = act->vbuf;
	src->vbuf_fmt = vbuffmt_3;
	src->vbuf_len = 4*3*0x10000;
	src->vbuf_w = 4*3;
	src->vbuf_h = 0x10000;
	src->vbuf_enq = 1;

	//index
	src->ibuf = act->ibuf;
	src->ibuf_fmt = 0x22;
	src->ibuf_len = 2*2*0x10000;
	src->ibuf_w = 2*2;
	src->ibuf_h = 0xffff;
	src->ibuf_enq = 1;
*/
}




static void calib3d_search(_obj* act)
{
}
static void calib3d_modify(_obj* act)
{
}
static void calib3d_delete(_obj* act)
{
	if(0 == act)return;

	struct perobj* perobj = (void*)act->priv_256b;
	if(perobj->ibuf){
		memoryfree(perobj->ibuf);
		perobj->ibuf = 0;
	}
	if(perobj->vbuf){
		memoryfree(perobj->vbuf);
		perobj->vbuf = 0;
	}
}
static void calib3d_create(_obj* act, void* str)
{
	int j;
	u16* uu;
	float* ff;
	struct mag_minmax* priv;
	if(0 == act)return;

	struct perobj* perobj = (void*)act->priv_256b;
	if(perobj->vbuf)return;
	if(perobj->ibuf)return;

	perobj->vlen = 0;

	ff = perobj->vbuf = memoryalloc(4*3*0x10000, 0);
	for(j=0;j<0x10000;j++){
		ff[j*3 + 0] = 0.0;
		ff[j*3 + 1] = 0.0;
		ff[j*3 + 2] = j;
	}

	uu = perobj->ibuf = memoryalloc(2*2*0x10000, 0);
	for(j=0;j<0xffff;j++){
		uu[j*2 + 0] = j;
		uu[j*2 + 1] = j+1;
	}

	perobj->xmin = 99999.0;
	perobj->xmax = -99999.0;
	perobj->ymin = 99999.0;
	perobj->ymax = -99999.0;
	perobj->zmin = 99999.0;
	perobj->zmax = -99999.0;
}




void calib3d_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('c', 'a', 'l', 'i', 'b', '3', 'd', 0);

	p->oncreate = (void*)calib3d_create;
	p->ondelete = (void*)calib3d_delete;
	p->onreader = (void*)calib3d_search;
	p->onwriter = (void*)calib3d_modify;

	p->onattach = (void*)calib3d_attach;
	p->ondetach = (void*)calib3d_detach;
	p->ontaking = (void*)calib3d_taking;
	p->ongiving = (void*)calib3d_giving;
}
