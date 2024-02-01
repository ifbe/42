#include "libuser.h"




static void hbridge_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}
}
static void hbridge_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	u8 gate[3][2] = {{0,1},{1,0},{0,0}};
	int x,y,z,j;
	int s,rgb;
	vec3 tc,tr,tf,tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
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
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void hbridge_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void hbridge_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void hbridge_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void hbridge_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	hbridge_draw_gl41(ent, slot, wor,geom, wnd,area);
}
static void hbridge_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	_obj* caller = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	switch(caller->type){
	case _wnd_:
		break;
	default:
		hbridge_read_byworld_bycam_bywnd(ent,slot, stack,sp);
	}
}
static void hbridge_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void hbridge_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void hbridge_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void hbridge_search(_obj* act, u8* buf)
{
}
static void hbridge_modify(_obj* act, u8* buf)
{
}
static void hbridge_delete(_obj* act, u8* buf)
{
}
static void hbridge_create(_obj* act, u8* buf)
{
}




void hbridge_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('h','b','r','i','d','g','e',0);

	p->oncreate = (void*)hbridge_create;
	p->ondelete = (void*)hbridge_delete;
	p->onreader = (void*)hbridge_search;
	p->onwriter = (void*)hbridge_modify;

	p->onattach = (void*)hbridge_attach;
	p->ondetach = (void*)hbridge_detach;
	p->ontaking = (void*)hbridge_taking;
	p->ongiving = (void*)hbridge_giving;
}
