#include "libuser.h"
int qrcode_generate(void* src, void* dst, int len);




static int slen;
static u8 databuf[49*49];




static void qrcode_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 color;
	int x,y;
	int x1,y1,x2,y2;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

	for(y=0;y<slen;y++)
	{
		for(x=0;x<slen;x++)
		{
			if( databuf[(y*slen)+x] == 0 )color=0;
			else color=0xffffffff;

			x1 = cx + (x*ww*2/slen) - ww;
			y1 = cy + (y*hh*2/slen) - hh;
			x2 = cx + ((x+1)*ww*2/slen) - ww;
			y2 = cy + ((y+1)*hh*2/slen) - hh;
			drawsolid_rect(
				win, color,
				x1, y1, x2, y2
			);
		}
//say("\n");
	}
}
static void qrcode_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void qrcode_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void qrcode_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"qrcode\" style=\"width:50%%;height:100px;float:left;background-color:#e127a9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void qrcode_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	int width = win->stride;
	int height = win->height;
	u8* p = (u8*)(win->buf);

	for(y=0;y<100;y++)
	{
		if(y >= slen)break;
		if(y >= height)break;
		for(x=0;x<100;x++)
		{
			if(x >= slen)break;
			if(x >= width/2)break;
			if( databuf[(y*slen)+x] != 0 )continue;

			p[( (y*width+x*2)<<2 ) + 3] = 7;
			p[( (y*width+x*2)<<2 ) + 7] = 7;
		}
	}
}
static void qrcode_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("qrcode(%x,%x,%x)\n",win,act,sty);
}
static void qrcode_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)qrcode_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)qrcode_read_tui(win, sty, act, pin);
	else if(fmt == _html_)qrcode_read_html(win, sty, act, pin);
	else if(fmt == _json_)qrcode_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)qrcode_read_vbo(win, sty, act, pin);
	else qrcode_read_pixel(win, sty, act, pin);
}
static void qrcode_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void qrcode_list()
{
}
static void qrcode_into()
{
}
static void qrcode_stop(struct actor* act, struct pinid* pin)
{
}
static void qrcode_start(struct actor* act, struct pinid* pin)
{
	slen=49;
	qrcode_generate("haha",databuf,slen);
}
static void qrcode_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void qrcode_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = databuf;
	if(_copy_ == act->type)act->buf = memorycreate(49*49);
}




void qrcode_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('q', 'r', 'c', 'o', 'd', 'e', 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)qrcode_create;
	p->ondelete = (void*)qrcode_delete;
	p->onstart  = (void*)qrcode_start;
	p->onstop   = (void*)qrcode_stop;
	p->onlist   = (void*)qrcode_list;
	p->onchoose = (void*)qrcode_into;
	p->onread   = (void*)qrcode_read;
	p->onwrite  = (void*)qrcode_write;
}
