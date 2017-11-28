#include "actor.h"
int uart_list();
int uart_choose(void*);
int uart_write(void*);




int status = 0;




static void terminal_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void terminal_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int w = (win->w) * (sty->wantw) / 0x20000;
	int h = (win->h) * (sty->wanth) / 0x20000;

	drawsolid_rect(
		win, 0xffffff,
		cx-w, cy-h, cx+w, cy+h
	);
}
static void terminal_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void terminal_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void terminal_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	if(status == 0)
	{
		say("terminal(%x,%x,%x)\n",win,act,sty);
		uart_list();
	}
}
static void terminal_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == hex32('c','l','i',0))terminal_read_cli(win, act, sty);
	else if(fmt == hex32('t','u','i',0))terminal_read_tui(win, act, sty);
	else if(fmt == hex32('h','t','m','l'))terminal_read_html(win, act, sty);
	else if(fmt == hex32('v','b','o',0))terminal_read_vbo(win, act, sty);
	else terminal_read_pixel(win, act, sty);
}
static void terminal_write(struct event* ev)
{
	if(ev->what == __char__)
	{
		if(status == 0)
		{
			uart_choose("COM7");
			status = 1;
		}
		else
		{
			uart_write((void*)ev);
		}
	}
}




static void terminal_list()
{
}
static void terminal_change()
{
}
static void terminal_start()
{
}
static void terminal_stop()
{
}
void terminal_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex32('t', 'e', 'r', 'm');

	p->start = (void*)terminal_start;
	p->stop = (void*)terminal_stop;
	p->list = (void*)terminal_list;
	p->choose = (void*)terminal_change;
	p->read = (void*)terminal_read;
	p->write = (void*)terminal_write;
}
void terminal_delete()
{
}
