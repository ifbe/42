#include "libuser.h"




static void poker_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int j, cx, cy, ww, hh;
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
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	for(j=0;j<108/4;j++)
	{
		drawsolid_rect(
			win, 0xc0c0c0,
			cx + (j-13)*ww/27 - (ww/4),
			cy - (j/2) + (hh/2),
			cx + (j-13)*ww/27 + (ww/4),
			cy - (j/2) + hh
		);
		drawline_rect(
			win, 0x404040,
			cx + (j-13)*ww/27 - (ww/4),
			cy - (j/2) + (hh/2),
			cx + (j-13)*ww/27 + (ww/4),
			cy - (j/2) + hh
		);
/*
		drawascii(
			win, 0xffffff,
			cx + (j-13)*ww/27 - (ww/4),
			cy - (j/2) + (hh/2),
			symbol[j/2]
		);
*/
	}
}
static void poker_draw_gl41(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void poker_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void poker_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void poker_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void poker_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void poker_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void poker_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void poker_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void poker_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void poker_search(_obj* act)
{
}
static void poker_modify(_obj* act)
{
}
static void poker_delete(_obj* act)
{
	if(0 == act)return;
	if(act->listptr.buf0){
		memorydelete(act->listptr.buf0);
		act->listptr.buf0 = 0;
	}
}
static void poker_create(_obj* act)
{
	if(0 == act)return;
	act->listptr.buf0 = memorycreate(108, 0);
}




void poker_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('p', 'o', 'k', 'e', 'r', 0, 0, 0);

	p->oncreate = (void*)poker_create;
	p->ondelete = (void*)poker_delete;
	p->onsearch = (void*)poker_search;
	p->onmodify = (void*)poker_modify;

	p->onlinkup = (void*)poker_linkup;
	p->ondiscon = (void*)poker_discon;
	p->ontaking = (void*)poker_taking;
	p->ongiving = (void*)poker_giving;
}
