#include "libuser.h"
#define _mic_ hex32('m','i','c',0)
#define _pcm_ hex32('p','c','m',0)
#define slice 16
#define TABBUF buf0
#define TABLEN data1




static void spectrum_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spectrum_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int x,y;
	int r,g,b;
	float dx,dy,dz;
	void** tab;
	short* buf;

	vec3 ta,tb;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	gl41line_prism4(ctx, 0xffff00, vc, vr, vf, vu);

	tab = act->TABBUF;
	if(0 == tab)return;
	//printmemory(tab, 8*4);

	for(y=0;y<slice;y++){
		buf = tab[y];
		if(0 == buf)break;

		for(x=0;x<512;x++){
			dx = (x-255.5) / 255.5;
			dy = y*2.0/slice-1.0;
			ta[0] = vc[0] + vr[0] * dx + vf[0] * dy;
			ta[1] = vc[1] + vr[1] * dx + vf[1] * dy;
			ta[2] = vc[2] + vr[2] * dx + vf[2] * dy;

			dz = buf[x] / 32768.0;
			tb[0] = ta[0] + vu[0] * dz;
			tb[1] = ta[1] + vu[1] * dz;
			tb[2] = ta[2] + vu[2] * dz;

			r = (x/100)*26+13;
			g = ((x%100)/10)*26+13;
			b = (x%10)*26+13;
			gl41line(ctx, (r<<16)|(g<<8)|b, ta, tb);
		}
	}
}
static void spectrum_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spectrum_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spectrum_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spectrum_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("spectrum(%x,%x,%x)\n",win,act,sty);
}




void spectrum_data(struct entity* act, int type, void* buf, int len)
{
	int j,idx;
	void** tab;
	say("@spectrum_write.pcm: %d,%llx\n", len, buf);

	idx = act->TABLEN;
	tab = act->TABBUF;

	for(j=15;j>0;j--)tab[j] = tab[j-1];
	tab[0] = buf;

	act->TABLEN = (idx+1) % slice;
}




static void spectrum_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		spectrum_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void spectrum_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(_pcm_ == foot){
		spectrum_data(ent, 0, buf, len);
	}
}
static void spectrum_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void spectrum_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@spectrum_linkup\n");
}




static void spectrum_search(struct entity* act)
{
}
static void spectrum_modify(struct entity* act)
{
}
static void spectrum_delete(struct entity* act)
{
}
static void spectrum_create(struct entity* act)
{
	act->TABBUF = memorycreate(0x1000, 0);
	act->TABLEN = 0;
}




void spectrum_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'p', 'e', 'c', 't', 'r', 'u', 'm');

	p->oncreate = (void*)spectrum_create;
	p->ondelete = (void*)spectrum_delete;
	p->onsearch = (void*)spectrum_search;
	p->onmodify = (void*)spectrum_modify;

	p->onlinkup = (void*)spectrum_linkup;
	p->ondiscon = (void*)spectrum_discon;
	p->onread  = (void*)spectrum_read;
	p->onwrite = (void*)spectrum_write;
}
