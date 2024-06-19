#include "libuser.h"
#define _fbo_ hex32('f','b','o',0)
//
#define ONOFF whdf.iw0
//
#define CAMBUF listptr.buf0
#define LITBUF listptr.buf1
#define CTXBUF listptr.buf2
#define OWNBUF listptr.buf3
struct sunbuf{
	mat4 mvp;
	vec4 rgb;
	u32 u_rgb;
};




static void pointlight_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void pointlight_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void pointlight_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void pointlight_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void pointlight_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void pointlight_light(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;
	struct gl41data* data = act->LITBUF;
	if(0 == data)return;

	data->dst.lighttype = _pointlight_;

	data->dst.arg[0].fmt = 'v';
	data->dst.arg[0].name = "sunxyz";
	data->dst.arg[0].data = geom->fs.vc;

	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "sunrgb";
	data->dst.arg[1].data = sun->rgb;

	wnd->gl41list.world[0].light[0] = act->LITBUF;
}
static void pointlight_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;
	gl41opaque_sphere(ctx, 0x80000000|sun->u_rgb, vc, vr, vf, vt);
}




static void pointlight_read_bywnd(_obj* ent,void* foot, _obj* wnd,void* area)
{
	switch(wnd->vfmt){
	case _rgba8888_:
		break;
	case _gl41list_:
		break;
	}
}
static void pointlight_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	if(0 == ent->ONOFF)return;

	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->vfmt){
	case _gl41list_:
		pointlight_light(ent,slot, wor,geom, wnd,area);
		pointlight_draw_gl41(ent,slot, wor,geom, wnd,area);
		break;
	}
}




static void pointlight_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
	case _render_:
		pointlight_read_bywnd(ent,slot, caller,area);
		break;
	default:
		pointlight_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void pointlight_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	u8* in = buf;
	logtoall("@pointlight_write:%x\n",in[0]);

	if('0' == in[0])ent->ONOFF = 0;
	if('1' == in[0])ent->ONOFF = 1;
}
static void pointlight_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void pointlight_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void pointlight_search(_obj* act)
{
}
static void pointlight_modify(_obj* act)
{
}
static void pointlight_delete(_obj* act)
{
}
static void pointlight_create(_obj* act, void* str)
{
	struct sunbuf* sun;
	if(0 == act)return;

	sun = act->OWNBUF = memoryalloc(0x1000, 0);
	sun->u_rgb = 0x0000ff;
	sun->rgb[0] = ((sun->u_rgb >>16) & 0xff) / 255.0;
	sun->rgb[1] = ((sun->u_rgb >> 8) & 0xff) / 255.0;
	sun->rgb[2] = ((sun->u_rgb >> 0) & 0xff) / 255.0;

	act->LITBUF = memoryalloc(0x400, 0);

	act->ONOFF = 1;
}




void pointlight_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('p','o','i','n','t','l', 'i', 't');

	p->oncreate = (void*)pointlight_create;
	p->ondelete = (void*)pointlight_delete;
	p->onreader = (void*)pointlight_search;
	p->onwriter = (void*)pointlight_modify;

	p->onattach = (void*)pointlight_attach;
	p->ondetach = (void*)pointlight_detach;
	p->ontaking = (void*)pointlight_taking;
	p->ongiving = (void*)pointlight_giving;
}
