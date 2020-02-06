#include "libuser.h"




static void poker_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int j, cx, cy, ww, hh;
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void poker_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void poker_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void poker_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void poker_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void poker_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void poker_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void poker_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void poker_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void poker_search(struct entity* act)
{
}
static void poker_modify(struct entity* act)
{
}
static void poker_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->buf0){
		memorydelete(act->buf0);
		act->buf0 = 0;
	}
}
static void poker_create(struct entity* act)
{
	if(0 == act)return;
	act->buf0 = memorycreate(108, 0);
}




void poker_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'o', 'k', 'e', 'r', 0, 0, 0);

	p->oncreate = (void*)poker_create;
	p->ondelete = (void*)poker_delete;
	p->onsearch = (void*)poker_search;
	p->onmodify = (void*)poker_modify;

	p->onlinkup = (void*)poker_linkup;
	p->ondiscon = (void*)poker_discon;
	p->onread  = (void*)poker_read;
	p->onwrite = (void*)poker_write;
}
