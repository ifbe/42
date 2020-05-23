#include "libuser.h"
u32 getrandom();




static void example_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u32 bg,fg;
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

	bg = getrandom()&0xffffff;
	fg = (~bg)&0xffffff;
	drawsolid_rect(win, bg, cx-ww, cy-hh, cx+ww, cy+hh);
	drawhexadecimal(win, fg, cx, cy, bg);
}
static void example_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	tu[0] = vu[0]/2;
	tu[1] = vu[1]/2;
	tu[2] = vu[2]/2;

	tc[0] = vc[0]-vr[0]/2-vf[0]/2+vu[0]/2;
	tc[1] = vc[1]-vr[1]/2-vf[1]/2+vu[1]/2;
	tc[2] = vc[2]-vr[2]/2-vf[2]/2+vu[2]/2,
	gl41point_sphere(win, 0xffffff, tc, tr, tf, tu);

	tc[0] = vc[0]+vr[0]/2-vf[0]/2+vu[0]/2;
	tc[1] = vc[1]+vr[1]/2-vf[1]/2+vu[1]/2;
	tc[2] = vc[2]+vr[2]/2-vf[2]/2+vu[2]/2;
	gl41line_sphere(win, 0xffffff, tc, tr, tf, tu);

	tc[0] = vc[0]-vr[0]/2+vf[0]/2+vu[0]/2;
	tc[1] = vc[1]-vr[1]/2+vf[1]/2+vu[1]/2;
	tc[2] = vc[2]-vr[2]/2+vf[2]/2+vu[2]/2;
	gl41solid_sphere(win, 0xff00ff, tc, tr, tf, tu);

	tc[0] = vc[0]-vr[0]/2-vf[0]/2+vu[0]*3/2;
	tc[1] = vc[1]-vr[1]/2-vf[1]/2+vu[1]*3/2;
	tc[2] = vc[2]-vr[2]/2-vf[2]/2+vu[2]*3/2;
	gl41point_sphere(win, 0xffffff, tc, tr, tf, tu);

	tc[0] = vc[0]+vr[0]/2-vf[0]/2+vu[0]*3/2;
	tc[1] = vc[1]+vr[1]/2-vf[1]/2+vu[1]*3/2;
	tc[2] = vc[2]+vr[2]/2-vf[2]/2+vu[2]*3/2;
	gl41line_sphere(win, 0xffffff, tc, tr, tf, tu);

	tc[0] = vc[0]-vr[0]/2+vf[0]/2+vu[0]*3/2;
	tc[1] = vc[1]-vr[1]/2+vf[1]/2+vu[1]*3/2;
	tc[2] = vc[2]-vr[2]/2+vf[2]/2+vu[2]*3/2;
	gl41solid_sphere(win, 0x87cefa, tc, tr, tf, tu);
}
static void example_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void example_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void example_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void example_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void example_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void example_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void example_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void example_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void example_search(struct entity* act)
{
}
static void example_modify(struct entity* act)
{
}
static void example_delete(struct entity* act)
{
	if(0 == act)return;
}
static void example_create(struct entity* act)
{
	if(0 == act)return;
}




void example_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('e', 'x', 'a', 'm', 'p', 'l', 'e', 0);

	p->oncreate = (void*)example_create;
	p->ondelete = (void*)example_delete;
	p->onsearch = (void*)example_search;
	p->onmodify = (void*)example_modify;

	p->onlinkup = (void*)example_linkup;
	p->ondiscon = (void*)example_discon;
	p->ontaking = (void*)example_taking;
	p->ongiving = (void*)example_giving;
}
