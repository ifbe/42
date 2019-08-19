#include "libuser.h"




static u64 algtype[] = {
	hex32('m','d','5',0),
	hex32('s','h','a','1'),
	0
};
static u8 buffer[16];




static void algorithm_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int cx = sty->f.vc[0];
	int cy = sty->f.vc[1];
	int ww = sty->f.vr[0];
	int hh = sty->f.vf[1];

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
static void algorithm_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void algorithm_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void algorithm_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"algo\" style=\"width:50%%;height:100px;float:left;background-color:#123456;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void algorithm_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void algorithm_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	say("algorithm(%x,%x,%x)\n",win,act,sty);
}
static void algorithm_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)algorithm_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)algorithm_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)algorithm_draw_html(act, pin, win, sty);
	else if(fmt == _json_)algorithm_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)algorithm_draw_vbo(act, pin, win, sty);
	else algorithm_draw_pixel(act, pin, win, sty);
}
static void algorithm_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
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




static void algorithm_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//algorithm_draw(act, pin, win, sty);
}
static void algorithm_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//algorithm_event(act, pin, win, sty, ev, 0);
}
static void algorithm_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void algorithm_start(struct halfrel* self, struct halfrel* peer)
{
}




static void algorithm_search(struct actor* act)
{
}
static void algorithm_modify(struct actor* act)
{
}
static void algorithm_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void algorithm_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(16);
}




void algorithm_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('a', 'l', 'g', 'o');

	p->oncreate = (void*)algorithm_create;
	p->ondelete = (void*)algorithm_delete;
	p->onsearch = (void*)algorithm_search;
	p->onmodify = (void*)algorithm_modify;

	p->onstart = (void*)algorithm_start;
	p->onstop  = (void*)algorithm_stop;
	p->onread  = (void*)algorithm_read;
	p->onwrite = (void*)algorithm_write;
}
