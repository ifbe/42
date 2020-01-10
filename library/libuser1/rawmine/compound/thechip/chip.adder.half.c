#include "libuser.h"
void gl41line_pmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void halfadd_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41line_rect(ctx, 0x404040, vc, vr, vf);
}




static void halfadd_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void halfadd_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void halfadd_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void halfadd_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void halfadd_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void halfadd_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> halfadd
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)halfadd_draw_gl41(act,slot, win,geom, wnd,area);
	}
}
static void halfadd_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	u8 tmp;
	struct entity* ent = self->pchip;
	say("@halfaddgate_write:%x\n",buf[0]);

	if('0' == buf[0])ent->ix0 = 0;
	else if('1' == buf[0])ent->ix0 = 1;
	else return;

	ent->iy0 = !ent->ix0;
	tmp = ent->iy0 + 0x30;
	relationwrite(ent, 'o', 0, 0, &tmp, 1);
}
static void halfadd_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void halfadd_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void halfadd_search(struct entity* act, u8* buf)
{
}
static void halfadd_modify(struct entity* act, u8* buf)
{
}
static void halfadd_delete(struct entity* act, u8* buf)
{
}
static void halfadd_create(struct entity* act, u8* buf)
{
	act->ix0 = getrandom()&1;
	act->iy0 = !act->ix0;
}




void halfadd_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('h','a','l','f','a','d','d', 0);

	p->oncreate = (void*)halfadd_create;
	p->ondelete = (void*)halfadd_delete;
	p->onsearch = (void*)halfadd_search;
	p->onmodify = (void*)halfadd_modify;

	p->onlinkup = (void*)halfadd_linkup;
	p->ondiscon = (void*)halfadd_discon;
	p->onread  = (void*)halfadd_read;
	p->onwrite = (void*)halfadd_write;
}
