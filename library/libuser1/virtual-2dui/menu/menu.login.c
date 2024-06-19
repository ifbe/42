#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void login_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void login_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41solid_rect(ctx, 0x808080, vc, vr ,vf);

	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]/2 +vt[j]/1000;
		tr[j] = vr[j] / 4;
		tf[j] = vf[j] / 4;
	}
	gl41string_center(ctx, 0xffffff, tc, tr, tf, (void*)"welcome!", 8);

	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]/8 +vt[j]/1000;
		tr[j] = vr[j] / 4;
		tf[j] = vf[j] / 16;
	}
	gl41solid_rect(ctx, 0x202020, tc, tr, tf);
	for(j=0;j<3;j++){
		tc[j] += vt[j]/1000;
		tr[j] /= 4;
	}
	gl41string_center(ctx, 0xffffff, tc, tr, tf, (void*)"username", 8);

	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]*3/8 +vt[j]/1000;
		tr[j] = vr[j] / 4;
		tf[j] = vf[j] / 16;
	}
	gl41solid_rect(ctx, 0x202020, tc, tr, tf);
	for(j=0;j<3;j++){
		tc[j] += vt[j]/1000;
		tr[j] /= 4;
	}
	gl41string_center(ctx, 0xffffff, tc, tr, tf, (void*)"password", 8);
}
static void login_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void login_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void login_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void login_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void login_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	login_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void login_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->type){
	case _wnd_:
	case _render_:
		break;
	default:
		login_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void login_giving(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void login_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void login_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void login_search(_obj* act)
{
}
static void login_modify(_obj* act)
{
}
static void login_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memoryfree(act->buf);
}
static void login_create(_obj* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memoryalloc(256, 0);
}




void login_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('l', 'o', 'g', 'i', 'n', 0, 0, 0);

	p->oncreate = (void*)login_create;
	p->ondelete = (void*)login_delete;
	p->onreader = (void*)login_search;
	p->onwriter = (void*)login_modify;

	p->onattach = (void*)login_attach;
	p->ondetach = (void*)login_detach;
	p->ontaking = (void*)login_taking;
	p->ongiving = (void*)login_giving;
}
