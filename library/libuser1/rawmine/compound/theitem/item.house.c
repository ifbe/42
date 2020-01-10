#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void house_draw_pixel(
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
	drawline(win, 0x404040, cx-ww, cy-hh/3, cx, cy-hh);
	drawline(win, 0x404040, cx+ww, cy-hh/3, cx, cy-hh);
	drawline_rect(win, 0x404040, cx-ww, cy-hh/3, cx+ww, cy+hh);
	drawline_rect(win, 0x404040, cx-ww/3, cy+hh/3, cx+ww/3, cy+hh);
	drawline_rect(win, 0x404040, cx-ww*5/6, cy-hh*1/6, cx-ww*3/6, cy+hh*1/6);
	drawline_rect(win, 0x404040, cx+ww*3/6, cy-hh*1/6, cx+ww*5/6, cy+hh*1/6);
}
static void house_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	vec3 tc, tr, tf, tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	gl41solid_rect(win, 0x404040, vc, vr, vf);

	//underground
	tu[0] = vu[0]/4;
	tu[1] = vu[1]/4;
	tu[2] = vu[2]/4;
	tc[0] = vc[0] - vu[0]*3/4;
	tc[1] = vc[1] - vu[1]*3/4;
	tc[2] = vc[2] - vu[2]*3/4;
	gl41solid_prism4(win, 0x101010, tc, vr, vf, tu);
	tc[0] = vc[0] - vu[0]/4;
	tc[1] = vc[1] - vu[1]/4;
	tc[2] = vc[2] - vu[2]/4;
	gl41solid_prism4(win, 0x964b00, tc, vr, vf, tu);

	//left
	tc[0] = vc[0]-vr[0]+vu[0]*3/8;
	tc[1] = vc[1]-vr[1]+vu[1]*3/8;
	tc[2] = vc[2]-vr[2]+vu[2]*3/8;
	tu[0] = vu[0]*3/8;
	tu[1] = vu[1]*3/8;
	tu[2] = vu[2]*3/8;
	gl41solid_rect(win, 0xc0c0c0, tc, vf, tu);

	tr[0] = vc[0]-vr[0]+vu[0];
	tr[1] = vc[1]-vr[1]+vu[1];
	tr[2] = vc[2]-vr[2]+vu[2];
	tf[0] = vc[0]-vr[0]-vf[0]+vu[0]*3/4;
	tf[1] = vc[1]-vr[1]-vf[1]+vu[1]*3/4;
	tf[2] = vc[2]-vr[2]-vf[2]+vu[2]*3/4;
	tu[0] = vc[0]-vr[0]+vf[0]+vu[0]*3/4;
	tu[1] = vc[1]-vr[1]+vf[1]+vu[1]*3/4;
	tu[2] = vc[2]-vr[2]+vf[2]+vu[2]*3/4;
	gl41solid_triangle(win, 0xc0c0c0, tr, tf, tu);

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
	gl41solid_rect(win, 0xc0c0c0, tc, tr, tu);

	tr[0] = vc[0]+vr[0]+vu[0];
	tr[1] = vc[1]+vr[1]+vu[1];
	tr[2] = vc[2]+vr[2]+vu[2];
	tf[0] = vc[0]+vr[0]+vf[0]+vu[0]*3/4;
	tf[1] = vc[1]+vr[1]+vf[1]+vu[1]*3/4;
	tf[2] = vc[2]+vr[2]+vf[2]+vu[2]*3/4;
	tu[0] = vc[0]+vr[0]-vf[0]+vu[0]*3/4;
	tu[1] = vc[1]+vr[1]-vf[1]+vu[1]*3/4;
	tu[2] = vc[2]+vr[2]-vf[2]+vu[2]*3/4;
	gl41solid_triangle(win, 0xc0c0c0, tr, tf, tu);

	//back
	tc[0] = vc[0]+vf[0]+vu[0]*3/8;
	tc[1] = vc[1]+vf[1]+vu[1]*3/8;
	tc[2] = vc[2]+vf[2]+vu[2]*3/8;
	tf[0] = vu[0]*3/8;
	tf[1] = vu[1]*3/8;
	tf[2] = vu[2]*3/8;
	gl41solid_rect(win, 0xc0c0c0, tc, vr, tf);

	tc[0] = vc[0]+vf[0]/2+vu[0]*7/8;
	tc[1] = vc[1]+vf[1]/2+vu[1]*7/8;
	tc[2] = vc[2]+vf[2]/2+vu[2]*7/8;
	tr[0] = -vr[0];
	tr[1] = -vr[1];
	tr[2] = -vr[2];
	tf[0] = vu[0]/8-vf[0]/2;
	tf[1] = vu[1]/8-vf[1]/2;
	tf[2] = vu[2]/8-vf[2]/2;
	gl41solid_rect(win, 0xffffff, tc, tr, tf);

	//front
	tc[0] = vc[0]-vf[0]+vu[0]*3/8;
	tc[1] = vc[1]-vf[1]+vu[1]*3/8;
	tc[2] = vc[2]-vf[2]+vu[2]*3/8;
	tf[0] = vu[0]*3/8;
	tf[1] = vu[1]*3/8;
	tf[2] = vu[2]*3/8;
	gl41solid_rect(win, 0xc0c0c0, tc, vr, tf);

	tc[0] = vc[0]-vf[0]/2+vu[0]*7/8;
	tc[1] = vc[1]-vf[1]/2+vu[1]*7/8;
	tc[2] = vc[2]-vf[2]/2+vu[2]*7/8;
	tf[0] = vu[0]/8+vf[0]/2;
	tf[1] = vu[1]/8+vf[1]/2;
	tf[2] = vu[2]/8+vf[2]/2;
	gl41solid_rect(win, 0xffffff, tc, vr, tf);
}
static void house_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void house_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void house_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void house_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void house_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void house_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void house_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void house_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void house_search(struct entity* act)
{
}
static void house_modify(struct entity* act)
{
}
static void house_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void house_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void house_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('h', 'o', 'u', 's', 'e', 0, 0, 0);

	p->oncreate = (void*)house_create;
	p->ondelete = (void*)house_delete;
	p->onsearch = (void*)house_search;
	p->onmodify = (void*)house_modify;

	p->onlinkup = (void*)house_linkup;
	p->ondiscon = (void*)house_discon;
	p->onread  = (void*)house_read;
	p->onwrite = (void*)house_write;
}
