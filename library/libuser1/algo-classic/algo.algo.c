#include "libuser.h"




static u64 algtype[] = {
	hex32('m','d','5',0),
	hex32('s','h','a','1'),
	0
};
static u8 buffer[16];




static void algorithm_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
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
	_obj* act, struct style* slot,
	_obj* wrl, struct style* geom,
	_obj* wnd, struct style* area)
{
}
static void algorithm_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void algorithm_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void algorithm_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void algorithm_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("algorithm(%x,%x,%x)\n",win,act,sty);
}




static void algorithm_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
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




static void algorithm_read_bywnd(_obj* ent,void* slot, _obj* wnd,void* area)
{
	switch(wnd->vfmt){
	case _bgra8888_:
	case _rgba8888_:
		break;
	case _gl41list_:
		break;
	}
}
static void algorithm_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	algorithm_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void algorithm_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		algorithm_read_bywnd(ent,foot, caller,area);
		break;
	default:
		algorithm_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void algorithm_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void algorithm_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void algorithm_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void algorithm_search(_obj* act)
{
}
static void algorithm_modify(_obj* act)
{
}
static void algorithm_delete(_obj* act)
{
}
static void algorithm_create(_obj* act)
{
}




void algorithm_register(_obj* p)
{
	p->type = hex32('a', 'l', 'g', 'o');
	p->vfmt = _orig_;

	p->oncreate = (void*)algorithm_create;
	p->ondelete = (void*)algorithm_delete;
	p->onreader = (void*)algorithm_search;
	p->onwriter = (void*)algorithm_modify;

	p->onattach = (void*)algorithm_attach;
	p->ondetach = (void*)algorithm_detach;
	p->ontaking = (void*)algorithm_taking;
	p->ongiving = (void*)algorithm_giving;
}
