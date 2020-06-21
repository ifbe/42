#include "libuser.h"




static u64 algtype[] = {
	hex32('m','d','5',0),
	hex32('s','h','a','1'),
	0
};
static u8 buffer[16];




static void algorithm_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx = sty->fs.vc[0];
	int cy = sty->fs.vc[1];
	int ww = sty->fs.vr[0];
	int hh = sty->fs.vf[1];

	//left: origin
	drawsolid_rect(win, 0x0000ff,
		cx - ww, cy - hh*7/8,
		cx - ww/3, cy + hh*7/8
	);
	drawstring(win, 0xffffff,
		cx-ww, cy-8,
		(void*)"makefile", 0
	);

	//middle: algorithm
	drawsolid_rect(win, 0x00ff00,
		cx - ww/3, cy - hh/8,
		cx + ww/3, cy + hh/8
	);
	drawstring(win, 0xffffff,
		cx - ww/3, cy-8,
		(void*)"doit", 0
	);

	//right: result
	drawsolid_rect(win, 0xff0000,
		cx + ww/3, cy - hh*7/8,
		cx + ww, cy + hh*7/8
	);
/*
	drawstring(win, 0xffffff,
		cx + ww/3, cy-8,
		(void*)pl->priv, 0
	);
*/
}
static void algorithm_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void algorithm_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void algorithm_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void algorithm_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void algorithm_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("algorithm(%x,%x,%x)\n",win,act,sty);
}




static void algorithm_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	u64 key = ev->why;
	u64 type = ev->what;
	int x;
	int y;
	int ret;
	int temp[64];

	if(type==0x2d70)
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;
/*
		int ret = openreadclose("makefile", 0, buffer, 0x100000);
		if(algtype[this] == 0x35646d)
		{
			md5sum(temp, buffer, ret);
			datastr2hexstr(pl->priv, temp, 16);
			pl->priv[32] = 0;
			reslen = 32;
		}
		else if(algtype[this] == 0x31616873)
		{
			sha1sum(temp, buffer, ret);
			datastr2hexstr(pl->priv, temp, 20);
			pl->priv[40] = 0;
			reslen = 40;
		}
*/
	}
}




static void algorithm_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void algorithm_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void algorithm_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void algorithm_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void algorithm_search(struct entity* act)
{
}
static void algorithm_modify(struct entity* act)
{
}
static void algorithm_delete(struct entity* act)
{
}
static void algorithm_create(struct entity* act)
{
}




void algorithm_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('a', 'l', 'g', 'o');

	p->oncreate = (void*)algorithm_create;
	p->ondelete = (void*)algorithm_delete;
	p->onsearch = (void*)algorithm_search;
	p->onmodify = (void*)algorithm_modify;

	p->onlinkup = (void*)algorithm_linkup;
	p->ondiscon = (void*)algorithm_discon;
	p->ontaking = (void*)algorithm_taking;
	p->ongiving = (void*)algorithm_giving;
}
