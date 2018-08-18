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
void queue_copy(struct uartterm* term, u8* buf, int len);
void drawterm(struct arena* win, void* term, int x0, int y0, int x1, int y1);




static u8 listbuf[0x100];
static int listlen = 0;
static u8 charbuf[0x100];
static int charlen = 0;
static int status = 0;
static int theone = 0;




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

	if(status == 0)
	{
		drawtext(win, 0xffffff,
			cx-ww, cy-hh, cx+ww, cy+hh,
			listbuf, listlen
		);
		return;
	}

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
	int x, y, w, h, enq,deq;
	u32* p;
	u32* q;
	u8* buf;
	struct uartterm* term = act->idx;
	if((status == 0)&&(charlen == 0))
	{
		gentui_text(win, 7, 0, 0, listbuf, listlen);
		return;
	}

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

	if((status == 0)&&(charlen == 0))return;
/*
	p = old->buf;
	enq = old->enq;
	deq = old->deq;
	old->deq = enq;
	if(enq == deq)return;

	if(enq > deq)say("%.*s", enq-deq, p+deq);
	else say("%.*s%.*s", 0x100000-deq, p+deq, enq, p);
*/
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
	void* addr;
	int j;
	u64 tmp;
	u64 why = ev->why;

	if(ev->what == _kbd_)
	{
		if(status != 0)
		{
			j = 3;
			if(why == 0x1b){tmp = 0x1b;j=1;}
			else if(why == 0x25)tmp = 0x445b1b;
			else if(why == 0x26)tmp = 0x415b1b;
			else if(why == 0x27)tmp = 0x435b1b;
			else if(why == 0x28)tmp = 0x425b1b;
			else return;

			if(status == 1)
			{
				writeuart(theone, 0, (void*)&tmp, j);
			}
			else
			{
				writeshell(theone, 0, (void*)&tmp, j);
			}
		}
		return;
	}

	if(ev->what == _char_)
	{
		if(status != 0)
		{
			if(status == 1)
			{
				writeuart(theone, 0, (void*)ev, 1);
			}
			else
			{
				writeshell(theone, 0, (void*)ev, 1);
			}
			return;
		}
		if(why == 0x8)
		{
			if(charlen > 0)
			{
				say("\b \b");
				charlen--;
				charbuf[charlen] = 0;
			}
			return;
		}

		say("%c",why);
		if((why == 0xd)|(why == 0xa))
		{
			if(charlen == 0)
			{
				theone = startshell();
				status = 2;
			}
			else
			{
				//theone = startuart(charbuf, 115200);
				addr = systemcreate(_uart_, charbuf);
				relationcreate(act, 0, _act_, addr, 0, _fd_);
				status = 1;
			}
			return;
		}
		if(charlen < 31)
		{
			charbuf[charlen] = why;
			charlen++;
		}
	}
}
static void terminal_write_data(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	say("%.*s", len, buf);
	queue_copy(act->idx, buf, len);
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
	listlen = listuart(listbuf, 0x100);
	if(listlen != 0)say("%.*s", listlen, listbuf);
}
static void terminal_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
}
static void terminal_create(struct actor* act)
{
	void* tty;
	struct uartterm* term;
	if(0 == act)return;

	tty = arenacreate(_uart_, 0);
	relationcreate(act, 0, _act_, tty, 0, _win_);

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
