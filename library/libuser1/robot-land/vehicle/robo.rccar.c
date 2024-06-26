#include "libuser.h"
#define _car_ hex32('c','a','r',0)
//int gpioread(int,int,void*,int);
//int gpiowrite(int,int,void*,int);




static void rccar_draw_pixel(
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
static void rccar_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int x,y;
	vec3 tc,tr,tf,tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;

	//wheel
	tf[0] = vf[0] / 4;
	tf[1] = vf[1] / 4;
	tf[2] = vf[2] / 4;
	tu[0] = vu[0] / 4;
	tu[1] = vu[1] / 4;
	tu[2] = vu[2] / 4;
	tr[0] = vr[0] / 8;
	tr[1] = vr[1] / 8;
	tr[2] = vr[2] / 8;
	for(y=-1;y<2;y+=2)
	{
		for(x=-1;x<2;x+=2)
		{
			tc[0] = vc[0] + x*vr[0]*0.875 + y*vf[0]*3/4 + tu[0];
			tc[1] = vc[1] + x*vr[1]*0.875 + y*vf[1]*3/4 + tu[1];
			tc[2] = vc[2] + x*vr[2]*0.875 + y*vf[2]*3/4 + tu[2];
			gl41solid_cylinder(ctx, 0x202020, tc, tf, tu, tr);
		}
	}

	//board
	tu[0] = vu[0] / 4;
	tu[1] = vu[1] / 4;
	tu[2] = vu[2] / 4;
	tc[0] = vc[0] + vu[0]/2;
	tc[1] = vc[1] + vu[1]/2;
	tc[2] = vc[2] + vu[2]/2;
	gl41solid_prism4(ctx, 0x808080, tc, vr, vf, tu);

	//tank
	tr[0] = vr[0] / 4;
	tr[1] = vr[1] / 4;
	tr[2] = vr[2] / 4;
	tf[0] = vf[0] / 4;
	tf[1] = vf[1] / 4;
	tf[2] = vf[2] / 4;
	tu[0] = vu[0] / 8;
	tu[1] = vu[1] / 8;
	tu[2] = vu[2] / 8;
	tc[0] = vc[0] + vu[0]*0.875;
	tc[1] = vc[1] + vu[1]*0.875;
	tc[2] = vc[2] + vu[2]*0.875;
	gl41solid_prism4(ctx, 0xc0c0c0, tc, tr, tf, tu);

	//cask
	tr[0] = vr[0] / 16;
	tr[1] = vr[1] / 16;
	tr[2] = vr[2] / 16;
	tf[0] = vf[0] / 2;
	tf[1] = vf[1] / 2;
	tf[2] = vf[2] / 2;
	tu[0] = vu[0] / 16;
	tu[1] = vu[1] / 16;
	tu[2] = vu[2] / 16;
	tc[0] = vc[0] + vf[0]/2 + vu[0]*0.875;
	tc[1] = vc[1] + vf[1]/2 + vu[1]*0.875;
	tc[2] = vc[2] + vf[2]/2 + vu[2]*0.875;
	gl41solid_cask(ctx, 0xffffff, tc, tr, tu, tf);
}
static void rccar_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void rccar_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void rccar_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void rccar_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void rccar_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
/*
	u8* p;
	short* t;
	if(len > 0)
	{
		p = (void*)ev;
		switch(p[0])
		{
			case 'a':gpiowrite(_car_, 0, "l", 0);break;
			case 'd':gpiowrite(_car_, 0, "r", 0);break;
			case 's':gpiowrite(_car_, 0, "n", 0);break;
			case 'w':gpiowrite(_car_, 0, "f", 0);break;
			default:gpiowrite(_car_, 0, " ", 0);
		}
		return;
	}

	if(_char_ == ev->what)
	{
		switch(ev->why)
		{
			case 'a':gpiowrite(_car_, 0, "l", 0);break;
			case 'd':gpiowrite(_car_, 0, "r", 0);break;
			case 's':gpiowrite(_car_, 0, "n", 0);break;
			case 'w':gpiowrite(_car_, 0, "f", 0);break;
			default:gpiowrite(_car_, 0, " ", 0);
		}
		return;
	}

	if('j' == (ev->what & 0xff))
	{
		if(joy_left == (ev->what & joy_mask))
		{
			t = (void*)ev;
			if(t[3] & joyl_left)		//x-
			{
				gpiowrite(_car_, 0, "l", 0);
				return;
			}
			if(t[3] & joyl_right)		//x+
			{
				gpiowrite(_car_, 0, "r", 0);
				return;
			}
			if(t[3] & joyl_down)		//y-
			{
				gpiowrite(_car_, 0, "n", 0);
				return;
			}
			if(t[3] & joyl_up)			//y+
			{
				gpiowrite(_car_, 0, "f", 0);
				return;
			}
			gpiowrite(_car_, 0, " ", 0);
		}
	}
*/
}




static void rccar_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	if(_camrts_ == wnd->type)wnd = stack[sp-8].pchip;

	rccar_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void rccar_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		break;
	default:
		rccar_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void rccar_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void rccar_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void rccar_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void rccar_search(_obj* act)
{
}
static void rccar_modify(_obj* act)
{
}
static void rccar_delete(_obj* act)
{
	if(0 == act)return;
}
static void rccar_create(_obj* act)
{
	if(0 == act)return;
}




void rccar_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('r', 'c', 'c', 'a', 'r', 0, 0, 0);

	p->oncreate = (void*)rccar_create;
	p->ondelete = (void*)rccar_delete;
	p->onreader = (void*)rccar_search;
	p->onwriter = (void*)rccar_modify;

	p->onattach = (void*)rccar_attach;
	p->ondetach = (void*)rccar_detach;
	p->ontaking = (void*)rccar_taking;
	p->ongiving = (void*)rccar_giving;
}
