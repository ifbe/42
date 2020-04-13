#include "libuser.h"




static void gaafet_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void gaafet_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void gaafet_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void gaafet_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void gaafet_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void gaafet_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41line_rect(wnd, 0xffffff, vc,vr,vf);
}
static void gaafet_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		gaafet_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void gaafet_read_bywnd(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}




static void gaafet_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct supply* sup = stack[sp-2].pchip;
	switch(sup->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		if('v' != key)break;
		gaafet_read_bywnd(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	default:{
		gaafet_read_bycam(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	}
}
static void gaafet_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void gaafet_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void gaafet_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void gaafet_search(struct entity* act, u8* buf)
{
}
static void gaafet_modify(struct entity* act, u8* buf)
{
}
static void gaafet_delete(struct entity* act, u8* buf)
{
}
static void gaafet_create(struct entity* act, void* arg, int argc, u8** argv)
{
	act->ix0 = 0;	//D
	act->iy0 = 0;	//S
	act->iz0 = 0;	//G
	act->iw0 = 0;	//B
}




void gaafet_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('g','a','a','f','e','t', 0, 0);

	p->oncreate = (void*)gaafet_create;
	p->ondelete = (void*)gaafet_delete;
	p->onsearch = (void*)gaafet_search;
	p->onmodify = (void*)gaafet_modify;

	p->onlinkup = (void*)gaafet_linkup;
	p->ondiscon = (void*)gaafet_discon;
	p->onread  = (void*)gaafet_read;
	p->onwrite = (void*)gaafet_write;
}
