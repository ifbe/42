#include "libuser.h"
#define _mic_ hex32('m','i','c',0)
#define _pcm_ hex32('p','c','m',0)
#define slice 16
#define TABBUF buf2
#define TABLEN data3




static void voxel_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void voxel_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int x,y,z;
	int r,g,b;
	float dx,dy,dz,dw;
	void** tab;
	short* buf;

	vec3 tc,tr,tf,tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	gl41line_prism4(wnd, 0xffff00, vc, vr, vf, vu);

	tab = act->TABBUF;
	if(0 == tab)return;
	//printmemory(tab, 8*4);

	buf = tab[0];
	if(0 == buf)return;

	for(z=0;z<10;z++){
	for(y=0;y<10;y++){
	for(x=0;x<10;x++){
		dw = buf[z*100 + y*10 + x];
		if(dw < 8)continue;

		dx = x / 5.0 - 0.9;
		dy = y / 5.0 - 0.9;
		dz = z / 5.0 - 0.9;
		dw = dw / 327680.0;
		tc[0] = vc[0] + vr[0]*dx + vf[0]*dy + vu[0]*dz;
		tc[1] = vc[1] + vr[1]*dx + vf[1]*dy + vu[1]*dz;
		tc[2] = vc[2] + vr[2]*dx + vf[2]*dy + vu[2]*dz;
		tr[0] = vr[0]*dw;
		tr[1] = vr[1]*dw;
		tr[2] = vr[2]*dw;
		tf[0] = vf[0]*dw;
		tf[1] = vf[1]*dw;
		tf[2] = vf[2]*dw;
		tu[0] = vu[0]*dw;
		tu[1] = vu[1]*dw;
		tu[2] = vu[2]*dw;

		r = z*26+13;
		g = y*26+13;
		b = x*26+13;
		gl41solid_prism4(wnd, (r<<16)|(g<<8)|b, tc, tr, tf, tu);
	}
	}
	}
}
static void voxel_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void voxel_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void voxel_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void voxel_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("voxel(%x,%x,%x)\n",win,act,sty);
}




void voxel_data(struct entity* act, int type, void* buf, int len)
{
	int j,idx;
	void** tab;
	say("@voxel_write.pcm: %d,%llx\n", len, buf);

	idx = act->TABLEN;
	tab = act->TABBUF;

	for(j=15;j>0;j--)tab[j] = tab[j-1];
	tab[0] = buf;

	act->TABLEN = (idx+1) % slice;
}




static void voxel_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		voxel_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void voxel_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(_pcm_ == foot){
		voxel_data(ent, 0, buf, len);
	}
}
static void voxel_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void voxel_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@voxel_linkup\n");
}




static void voxel_search(struct entity* act)
{
}
static void voxel_modify(struct entity* act)
{
}
static void voxel_delete(struct entity* act)
{
}
static void voxel_create(struct entity* act)
{
	act->TABBUF = memorycreate(0x1000, 0);
	act->TABLEN = 0;
}




void voxel_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('v', 'o', 'x', 'e', 'l', 0, 0, 0);

	p->oncreate = (void*)voxel_create;
	p->ondelete = (void*)voxel_delete;
	p->onsearch = (void*)voxel_search;
	p->onmodify = (void*)voxel_modify;

	p->onlinkup = (void*)voxel_linkup;
	p->ondiscon = (void*)voxel_discon;
	p->onread  = (void*)voxel_read;
	p->onwrite = (void*)voxel_write;
}
