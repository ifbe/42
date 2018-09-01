#include "libuser.h"
struct uartterm
{
	u8* buf;
	u32 len;
	u32 fg;
	u32 bg;

	int width;
	int height;
	int vimw;
	int vimh;

	int left;
	int right;
	int top;
	int bottom;

	int curx;
	int cury;
};
int listuart(void*, int);
int startuart(void*, int);
int writeuart(int fd, int off, char* buf, int len);
//
int listshell(void*, int);
int startshell();
int writeshell(int fd, int off, char* buf, int len);
//
void drawterm(struct arena* win, void* term, int x0, int y0, int x1, int y1);
void terminal_serverinput(struct uartterm* term, u8* buf, int len);




static u8 charbuf[0x100];
static int charlen = 0;




static void terminal_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
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
	drawhyaline_rect(win, 0x111111, cx-ww, cy-hh, cx+ww, cy+hh);
	drawterm(win, act->idx, cx-ww, cy-hh, cx+ww, cy+hh);
}
static void terminal_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terminal_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terminal_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"term\" style=\"width:50%%;height:100px;float:left;background-color:#f0ac2b;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void terminal_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x, y, w, h;
	u32* p;
	u32* q;
	u8* buf;
	struct uartterm* term;

	term = act->idx;
	w = win->width;
	h = win->height;
	if(w > term->width)w = term->width;
	if(h > term->height)h = term->height;

	p = (void*)(win->buf);
	q = (void*)(term->buf);
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			p[(win->stride)*y + x] = q[(term->width)*(term->top+y) + x];
		}
	}
}
static void terminal_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u8* p;
	int enq, deq;
	//say("terminal(%x,%x,%x)\n",win,act,sty);
}
static void terminal_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)terminal_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)terminal_read_tui(win, sty, act, pin);
	else if(fmt == _html_)terminal_read_html(win, sty, act, pin);
	else if(fmt == _json_)terminal_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)terminal_read_vbo(win, sty, act, pin);
	else terminal_read_pixel(win, sty, act, pin);
}




static void terminal_write_event(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev)
{
	int j;
	u64 why;
	void* dc;
	void* df;
	struct relation* irel;

	why = ev->why;
	if(ev->what == _kbd_)
	{
		j = 3;
		if(why == 0x1b)j = 1;
		else if(why == 0x25)why = 0x445b1b;
		else if(why == 0x26)why = 0x415b1b;
		else if(why == 0x27)why = 0x435b1b;
		else if(why == 0x28)why = 0x425b1b;
		else return;
	}
	else if(ev->what == _char_)j = 1;

	irel = act->irel0;
	if(0 == irel)return;

	while(1)
	{
		if(0 == irel)break;

		dc = (void*)(irel->srcchip);
		df = (void*)(irel->srcfoot);
		if(_fd_ == irel->srctype)
		{
			systemwrite(dc, df, act, pin, &why, j);
		}

		irel = samedstnextsrc(irel);
	}
}
static void terminal_write_data(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	say("%.*s", len, buf);
	terminal_serverinput(act->idx, buf, len);
}
static void terminal_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	void* buf, int len)
{
	if(0 == win)terminal_write_event(act, pin, win, sty, buf);
	else terminal_write_data(act, pin, win, sty, buf, len);
}




static void terminal_list()
{
}
static void terminal_change()
{
}
static void terminal_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terminal_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terminal_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
}
static void terminal_create(struct actor* act)
{
	struct uartterm* term;
	if(0 == act)return;

	act->idx = memorycreate(sizeof(struct uartterm));
	act->buf = memorycreate(0x100000);

	term = act->idx;
	term->curx = 0;
	term->cury = 0;
	term->left = 0;
	term->right = 0;
	term->top = 0;
	term->bottom = 0;
	term->vimw = 128;
	term->vimh = 24;
	term->width = 128;
	term->height = 24;
	term->bg = 0;
	term->fg = 7;
	term->len = 0x100000;
	term->buf = act->buf;
}




void terminal_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex32('t', 'e', 'r', 'm');

	p->oncreate = (void*)terminal_create;
	p->ondelete = (void*)terminal_delete;
	p->onstart  = (void*)terminal_start;
	p->onstop   = (void*)terminal_stop;
	p->onlist   = (void*)terminal_list;
	p->onchoose = (void*)terminal_change;
	p->onread   = (void*)terminal_read;
	p->onwrite  = (void*)terminal_write;
}
