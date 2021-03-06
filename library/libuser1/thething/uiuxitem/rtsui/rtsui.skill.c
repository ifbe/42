#include "libuser.h"




static void skill_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	vec4 tc,tr,tf;
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	int x,y,j;
	int w = 4;
	int h = 3;
	for(y=0;y<h;y++){
		for(x=0;x<w;x++){
			for(j=0;j<3;j++){
				tr[j] = 0.9*vr[j] / w;
				tf[j] = 0.9*vf[j] / h;
				tc[j] = vc[j] + (2*x-w+1)*vr[j]/w + (2*y-h+1)*vf[j]/h;
				gl41solid_rect(ctx,0x00ffff, tc,tr,tf);
			}
		}
	}
}
static void skill_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void skill_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void skill_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void skill_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void skill_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void skill_read_bycam(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	skill_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void skill_read_byuiux(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* uuu;struct style* area;
	struct entity* wnd;struct style* rect;
	uuu = stack[sp-2].pchip;area = stack[sp-2].pfoot;
	wnd = stack[sp-4].pchip;rect = stack[sp-4].pfoot;

	float x0,y0,xn,yn;
	x0 = area->fs.vc[0];
	y0 = area->fs.vc[1];
	xn = area->fs.vc[0]+area->fs.vq[0];
	yn = area->fs.vc[1]+area->fs.vq[1];
	//say("%f,%f,%f,%f\n",x0,y0,dx,dy);

	struct fstyle fs;
	fs.vc[0] = (x0+xn)/2;fs.vc[1] = (y0+yn)/2;fs.vc[2] = 0.5;
	fs.vr[0] = (xn-x0)/2;fs.vr[1] =       0.0;fs.vr[2] = 0.0;
	fs.vf[0] =       0.0;fs.vf[1] = (yn-y0)/2;fs.vf[2] = 0.0;
	fs.vt[0] =       0.0;fs.vt[1] =       0.0;fs.vt[2] =-0.5;

	skill_draw_gl41(ent, 0, 0,(void*)&fs, wnd,rect);
}
static void skill_read_bywnd(_ent* ent,struct style* slot, _ent* wnd,struct style* area)
{
}




static void skill_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
//say("fmt=%.8s\n", &sup->fmt);

	switch(wnd->fmt){
	case _gl41list_:
		skill_read_bywnd(ent,slot, wnd,area);
		break;
	case _virtual_:
		skill_read_byuiux(ent,slot, stack,sp);
		break;
	default:
		skill_read_bycam(ent,slot, stack,sp, arg,key);
		break;
	}
}
static void skill_giving(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void skill_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void skill_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void skill_search(struct entity* act)
{
}
static void skill_modify(struct entity* act)
{
}
static void skill_delete(struct entity* act)
{
}
static void skill_create(struct entity* act, u8* arg)
{
}




void skill_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'k', 'i', 'l', 'l', 0, 0, 0);

	p->oncreate = (void*)skill_create;
	p->ondelete = (void*)skill_delete;
	p->onsearch = (void*)skill_search;
	p->onmodify = (void*)skill_modify;

	p->onlinkup = (void*)skill_linkup;
	p->ondiscon = (void*)skill_discon;
	p->ontaking = (void*)skill_taking;
	p->ongiving = (void*)skill_giving;
}
