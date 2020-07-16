#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void login_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void login_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void login_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void login_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void login_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void login_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		login_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void login_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void login_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void login_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void login_search(struct entity* act)
{
}
static void login_modify(struct entity* act)
{
}
static void login_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void login_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void login_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('l', 'o', 'g', 'i', 'n', 0, 0, 0);

	p->oncreate = (void*)login_create;
	p->ondelete = (void*)login_delete;
	p->onsearch = (void*)login_search;
	p->onmodify = (void*)login_modify;

	p->onlinkup = (void*)login_linkup;
	p->ondiscon = (void*)login_discon;
	p->ontaking = (void*)login_taking;
	p->ongiving = (void*)login_giving;
}
