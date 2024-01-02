#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void house_draw_pixel(
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
	drawline(win, 0x404040, cx-ww, cy-hh/3, cx, cy-hh);
	drawline(win, 0x404040, cx+ww, cy-hh/3, cx, cy-hh);
	drawline_rect(win, 0x404040, cx-ww, cy-hh/3, cx+ww, cy+hh);
	drawline_rect(win, 0x404040, cx-ww/3, cy+hh/3, cx+ww/3, cy+hh);
	drawline_rect(win, 0x404040, cx-ww*5/6, cy-hh*1/6, cx-ww*3/6, cy+hh*1/6);
	drawline_rect(win, 0x404040, cx+ww*3/6, cy-hh*1/6, cx+ww*5/6, cy+hh*1/6);
}
static void house_draw_gl41(
	_obj* act, struct style* slot,
	_obj* wrl, struct style* geom,
	_obj* wnd, struct style* area)
{
	vec3 tc, tr, tf, tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41solid_rect(wnd, 0x404040, vc, vr, vf);

	//underground
	tu[0] = vu[0]/4;
	tu[1] = vu[1]/4;
	tu[2] = vu[2]/4;
	tc[0] = vc[0] - vu[0]*3/4;
	tc[1] = vc[1] - vu[1]*3/4;
	tc[2] = vc[2] - vu[2]*3/4;
	gl41solid_prism4(wnd, 0x101010, tc, vr, vf, tu);
	tc[0] = vc[0] - vu[0]/4;
	tc[1] = vc[1] - vu[1]/4;
	tc[2] = vc[2] - vu[2]/4;
	gl41solid_prism4(wnd, 0x964b00, tc, vr, vf, tu);

	//left
	tc[0] = vc[0]-vr[0]+vu[0]*3/8;
	tc[1] = vc[1]-vr[1]+vu[1]*3/8;
	tc[2] = vc[2]-vr[2]+vu[2]*3/8;
	tu[0] = vu[0]*3/8;
	tu[1] = vu[1]*3/8;
	tu[2] = vu[2]*3/8;
	gl41solid_rect(wnd, 0xc0c0c0, tc, vf, tu);

	tr[0] = vc[0]-vr[0]+vu[0];
	tr[1] = vc[1]-vr[1]+vu[1];
	tr[2] = vc[2]-vr[2]+vu[2];
	tf[0] = vc[0]-vr[0]-vf[0]+vu[0]*3/4;
	tf[1] = vc[1]-vr[1]-vf[1]+vu[1]*3/4;
	tf[2] = vc[2]-vr[2]-vf[2]+vu[2]*3/4;
	tu[0] = vc[0]-vr[0]+vf[0]+vu[0]*3/4;
	tu[1] = vc[1]-vr[1]+vf[1]+vu[1]*3/4;
	tu[2] = vc[2]-vr[2]+vf[2]+vu[2]*3/4;
	gl41solid_triangle(wnd, 0xc0c0c0, tr, tf, tu);

	//right
	tc[0] = vc[0]+vr[0]+vu[0]*3/8;
	tc[1] = vc[1]+vr[1]+vu[1]*3/8;
	tc[2] = vc[2]+vr[2]+vu[2]*3/8;
	tr[0] = -vf[0];
	tr[1] = -vf[1];
	tr[2] = -vf[2];
	tu[0] = vu[0]*3/8;
	tu[1] = vu[1]*3/8;
	tu[2] = vu[2]*3/8;
	gl41solid_rect(wnd, 0xc0c0c0, tc, tr, tu);

	tr[0] = vc[0]+vr[0]+vu[0];
	tr[1] = vc[1]+vr[1]+vu[1];
	tr[2] = vc[2]+vr[2]+vu[2];
	tf[0] = vc[0]+vr[0]+vf[0]+vu[0]*3/4;
	tf[1] = vc[1]+vr[1]+vf[1]+vu[1]*3/4;
	tf[2] = vc[2]+vr[2]+vf[2]+vu[2]*3/4;
	tu[0] = vc[0]+vr[0]-vf[0]+vu[0]*3/4;
	tu[1] = vc[1]+vr[1]-vf[1]+vu[1]*3/4;
	tu[2] = vc[2]+vr[2]-vf[2]+vu[2]*3/4;
	gl41solid_triangle(wnd, 0xc0c0c0, tr, tf, tu);

	//back
	tc[0] = vc[0]+vf[0]+vu[0]*3/8;
	tc[1] = vc[1]+vf[1]+vu[1]*3/8;
	tc[2] = vc[2]+vf[2]+vu[2]*3/8;
	tf[0] = vu[0]*3/8;
	tf[1] = vu[1]*3/8;
	tf[2] = vu[2]*3/8;
	gl41solid_rect(wnd, 0xc0c0c0, tc, vr, tf);

	tc[0] = vc[0]+vf[0]/2+vu[0]*7/8;
	tc[1] = vc[1]+vf[1]/2+vu[1]*7/8;
	tc[2] = vc[2]+vf[2]/2+vu[2]*7/8;
	tr[0] = -vr[0];
	tr[1] = -vr[1];
	tr[2] = -vr[2];
	tf[0] = vu[0]/8-vf[0]/2;
	tf[1] = vu[1]/8-vf[1]/2;
	tf[2] = vu[2]/8-vf[2]/2;
	gl41solid_rect(wnd, 0xffffff, tc, tr, tf);

	//front
	tc[0] = vc[0]-vf[0]+vu[0]*3/8;
	tc[1] = vc[1]-vf[1]+vu[1]*3/8;
	tc[2] = vc[2]-vf[2]+vu[2]*3/8;
	tf[0] = vu[0]*3/8;
	tf[1] = vu[1]*3/8;
	tf[2] = vu[2]*3/8;
	gl41solid_rect(wnd, 0xc0c0c0, tc, vr, tf);

	tc[0] = vc[0]-vf[0]/2+vu[0]*7/8;
	tc[1] = vc[1]-vf[1]/2+vu[1]*7/8;
	tc[2] = vc[2]-vf[2]/2+vu[2]*7/8;
	tf[0] = vu[0]/8+vf[0]/2;
	tf[1] = vu[1]/8+vf[1]/2;
	tf[2] = vu[2]/8+vf[2]/2;
	gl41solid_rect(wnd, 0xffffff, tc, vr, tf);
}
static void house_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void house_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void house_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void house_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void house_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	house_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void house_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		house_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void house_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void house_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void house_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void house_search(_obj* act)
{
}
static void house_modify(_obj* act)
{
}
static void house_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memoryfree(act->buf);
}
static void house_create(_obj* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memoryalloc(256, 0);
}




void house_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('h', 'o', 'u', 's', 'e', 0, 0, 0);

	p->oncreate = (void*)house_create;
	p->ondelete = (void*)house_delete;
	p->onreader = (void*)house_search;
	p->onwriter = (void*)house_modify;

	p->onattach = (void*)house_attach;
	p->ondetach = (void*)house_detach;
	p->ontaking = (void*)house_taking;
	p->ongiving = (void*)house_giving;
}
