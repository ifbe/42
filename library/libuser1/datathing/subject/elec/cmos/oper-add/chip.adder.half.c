#include "libuser.h"
void gl41line_pmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void halfadd_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(ctx, 0x404040, vc, vr, vf);
}




static void halfadd_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void halfadd_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void halfadd_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void halfadd_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void halfadd_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void halfadd_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	halfadd_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void halfadd_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		halfadd_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void halfadd_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	u8 tmp;
	say("@halfaddgate_write:%x\n",buf[0]);

	if('0' == buf[0])ent->whdf.ix0 = 0;
	else if('1' == buf[0])ent->whdf.ix0 = 1;
	else return;

	ent->whdf.iy0 = !ent->whdf.ix0;
	tmp = ent->whdf.iy0 + 0x30;
	give_data_into_peer(ent,'o', stack,sp, 0,0, &tmp,1);
}
static void halfadd_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void halfadd_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void halfadd_search(_obj* act, u8* buf)
{
}
static void halfadd_modify(_obj* act, u8* buf)
{
}
static void halfadd_delete(_obj* act, u8* buf)
{
}
static void halfadd_create(_obj* act, u8* buf)
{
	act->whdf.ix0 = getrandom()&1;
	act->whdf.iy0 = !act->whdf.ix0;
}




void halfadd_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('h','a','l','f','a','d','d', 0);

	p->oncreate = (void*)halfadd_create;
	p->ondelete = (void*)halfadd_delete;
	p->onsearch = (void*)halfadd_search;
	p->onmodify = (void*)halfadd_modify;

	p->onattach = (void*)halfadd_attach;
	p->ondetach = (void*)halfadd_detach;
	p->ontaking = (void*)halfadd_taking;
	p->ongiving = (void*)halfadd_giving;
}
