#include "libuser.h"




static void hbridge_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void hbridge_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	u8 gate[3][2] = {{0,1},{1,0},{0,0}};
	int x,y,z,j;
	int s,rgb;
	vec3 tc,tr,tf,tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	gl41line_prism4(ctx, 0xffffff, vc, vr, vf, vu);

	//board
	for(j=0;j<3;j++)tc[j] = vc[j] - vu[j];
	gl41solid_rect(ctx, 0x0000ff, tc, vr, vf);
	for(j=0;j<3;j++)tc[j] = vc[j] + vu[j];
	gl41solid_rect(ctx, 0xff0000, tc, vr, vf);

	//6 mosfets
	for(z=-1;z<2;z+=2)
	{
		if(z > 0)s = 1;
		else s = -1;
		for(y=-1;y<2;y++)
		{
			//mos
			if(0 == gate[y+1][(z+1)/2])rgb = 0x808080;
			else if(z < 0)rgb = 0x000080;
			else if(z > 0)rgb = 0x800000;
			for(j=0;j<3;j++){
				tc[j] = vc[j] + y*vf[j]/2 + z*vu[j]/2;
				tr[j] = vr[j]/8;
				tf[j] = vf[j]/8;
				tu[j] = vu[j]/8;
			}
			gl41solid_prism4(ctx, rgb, tc, tr, tf, tu);

			//g
			for(j=0;j<3;j++){
				tc[j] = vc[j] + y*vf[j]/2 + z*vu[j]/2;
				tu[j] = tc[j] - vr[j]/2;
			}
			if(gate[y+1][(z+1)/2] == 1)rgb = 0xff0000;
			else rgb = 0xffffff;
			gl41line(ctx, rgb, tc, tu);

			//d,s
			for(j=0;j<3;j++)tu[j] = tc[j] - s*vu[j]/2;
			gl41line(ctx, 0xffffff, tc, tu);

			//d,s
			for(j=0;j<3;j++)tu[j] = tc[j] + s*vu[j]/2;
			if(s>0.0)rgb = 0xff0000;
			else rgb = 0x0000ff;
			gl41line(ctx, rgb, tc, tu);
		}
	}

	for(y=-1;y<2;y++)
	{
		for(j=0;j<3;j++){
			tc[j] = vc[j] + y*vf[j]/2;
			tu[j] = tc[j] + vr[j]/2;
		}

		rgb = 0xffffff;
		if((gate[y+1][0] > 0)&&(gate[y+1][1] < 1))rgb = 0x0000ff;
		if((gate[y+1][0] < 1)&&(gate[y+1][1] > 0))rgb = 0xff0000;
		gl41line(ctx, rgb, tc, tu);
	}
}
static void hbridge_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void hbridge_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void hbridge_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void hbridge_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void hbridge_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		hbridge_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void hbridge_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void hbridge_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void hbridge_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void hbridge_search(struct entity* act, u8* buf)
{
}
static void hbridge_modify(struct entity* act, u8* buf)
{
}
static void hbridge_delete(struct entity* act, u8* buf)
{
}
static void hbridge_create(struct entity* act, u8* buf)
{
}




void hbridge_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('h','b','r','i','d','g','e',0);

	p->oncreate = (void*)hbridge_create;
	p->ondelete = (void*)hbridge_delete;
	p->onsearch = (void*)hbridge_search;
	p->onmodify = (void*)hbridge_modify;

	p->onlinkup = (void*)hbridge_linkup;
	p->ondiscon = (void*)hbridge_discon;
	p->onread  = (void*)hbridge_read;
	p->onwrite = (void*)hbridge_write;
}
