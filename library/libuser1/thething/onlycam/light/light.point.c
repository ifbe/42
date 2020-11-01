#include "libuser.h"
#define _fbo_ hex32('f','b','o',0)
//
#define ONOFF iw0
//
#define CAMBUF buf0
#define LITBUF buf1
#define CTXBUF buf2
#define OWNBUF buf3
struct sunbuf{
	mat4 mvp;
	vec4 rgb;
	u32 u_rgb;
};




static void pointlight_light(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;
	struct gl41data* data = act->LITBUF;
	if(0 == data)return;

	data->dst.routine_name = "passtype";
	data->dst.routine_detail = "pointlight";

	data->dst.arg[0].fmt = 'v';
	data->dst.arg[0].name = "sunxyz";
	data->dst.arg[0].data = geom->fs.vc;

	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "sunrgb";
	data->dst.arg[1].data = sun->rgb;

	wnd->glfull_light[0] = act->LITBUF;
}
static void pointlight_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;
	gl41opaque_sphere(ctx, 0x80000000|sun->u_rgb, vc, vr, vf, vt);
}




static void pointlight_read_bycam(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(0 == stack)return;

	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	if(_gl41full_ == wnd->fmt){
		if('v' == key){
			pointlight_light(ent,slot, wor,geom, wnd,area);
			pointlight_draw_gl41(ent,slot, wor,geom, wnd,area);
		}
		if('?' == key){
			//search for myown fbo

			//update matrix for fbo

			//wnd.data -> fbo.texture

			//fbo.texture -> my.data -> wnd.data
		}
	}
}
static void pointlight_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pointlight_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pointlight_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pointlight_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pointlight_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void pointlight_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == ent->ONOFF)return;
	pointlight_read_bycam(ent,foot, stack,sp, arg,key, buf,len);
}
static void pointlight_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	u8* in = buf;
	say("@pointlight_write:%x\n",in[0]);

	if('0' == in[0])ent->ONOFF = 0;
	if('1' == in[0])ent->ONOFF = 1;
}
static void pointlight_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void pointlight_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void pointlight_search(struct entity* act)
{
}
static void pointlight_modify(struct entity* act)
{
}
static void pointlight_delete(struct entity* act)
{
}
static void pointlight_create(struct entity* act, void* str)
{
	struct sunbuf* sun;
	if(0 == act)return;

	sun = act->OWNBUF = memorycreate(0x1000, 0);
	sun->u_rgb = 0x0000ff;
	sun->rgb[0] = ((sun->u_rgb >>16) & 0xff) / 255.0;
	sun->rgb[1] = ((sun->u_rgb >> 8) & 0xff) / 255.0;
	sun->rgb[2] = ((sun->u_rgb >> 0) & 0xff) / 255.0;

	act->LITBUF = memorycreate(0x400, 0);

	act->ONOFF = 1;
}




void pointlight_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('p','o','i','n','t','l', 'i', 't');

	p->oncreate = (void*)pointlight_create;
	p->ondelete = (void*)pointlight_delete;
	p->onsearch = (void*)pointlight_search;
	p->onmodify = (void*)pointlight_modify;

	p->onlinkup = (void*)pointlight_linkup;
	p->ondiscon = (void*)pointlight_discon;
	p->ontaking = (void*)pointlight_taking;
	p->ongiving = (void*)pointlight_giving;
}
