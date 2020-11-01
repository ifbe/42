#include "libuser.h"
#define _car_ hex32('c','a','r',0)
int gpioread(int,int,void*,int);
int gpiowrite(int,int,void*,int);




static void rccar_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void rccar_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void rccar_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void rccar_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void rccar_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void rccar_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
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
}




static void rccar_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		rccar_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void rccar_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void rccar_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void rccar_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void rccar_search(struct entity* act)
{
}
static void rccar_modify(struct entity* act)
{
}
static void rccar_delete(struct entity* act)
{
	if(0 == act)return;
}
static void rccar_create(struct entity* act)
{
	if(0 == act)return;
}




void rccar_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('r', 'c', 'c', 'a', 'r', 0, 0, 0);

	p->oncreate = (void*)rccar_create;
	p->ondelete = (void*)rccar_delete;
	p->onsearch = (void*)rccar_search;
	p->onmodify = (void*)rccar_modify;

	p->onlinkup = (void*)rccar_linkup;
	p->ondiscon = (void*)rccar_discon;
	p->ontaking = (void*)rccar_taking;
	p->ongiving = (void*)rccar_giving;
}
