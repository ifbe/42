#include "libuser.h"
int openreadclose(void*, u64, void*, u64);
int openwriteclose(void*, u64, void*, u64);




static u64 algtype[] = {
	hex32('m','d','5',0),
	hex32('s','h','a','1'),
	0
};
static u8 buffer[16];




static void algorithm_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx = sty->vc[0];
	int cy = sty->vc[1];
	int ww = sty->vr[0];
	int hh = sty->vf[1];

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
static void algorithm_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void algorithm_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void algorithm_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
static void algorithm_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void algorithm_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("algorithm(%x,%x,%x)\n",win,act,sty);
}
static void algorithm_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)algorithm_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)algorithm_read_tui(win, sty, act, pin);
	else if(fmt == _html_)algorithm_read_html(win, sty, act, pin);
	else if(fmt == _json_)algorithm_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)algorithm_read_vbo(win, sty, act, pin);
	else algorithm_read_pixel(win, sty, act, pin);
}
static void algorithm_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
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
static void algorithm_list()
{
}
static void algorithm_choose()
{
}
static void algorithm_stop(struct actor* act, struct pinid* pin)
{
}
static void algorithm_start(struct actor* act, struct pinid* pin)
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
	p->name = hex32('a', 'l', 'g', 'o');

	p->oncreate = (void*)algorithm_create;
	p->ondelete = (void*)algorithm_delete;
	p->onstart  = (void*)algorithm_start;
	p->onstop   = (void*)algorithm_stop;
	p->onlist   = (void*)algorithm_list;
	p->onchoose = (void*)algorithm_choose;
	p->onread   = (void*)algorithm_read;
	p->onwrite  = (void*)algorithm_write;
}
