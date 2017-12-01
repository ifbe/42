#include "actor.h"
int uart_list(void*);
int uart_choose(void*);
int uart_write(void*);




struct uartinfo
{
	u8* buf;
	int len;
	int enq;
	int deq;
};
static struct uartinfo* old;
static struct uartinfo new;
int listlen = 0;
char listbuf[0x100];
int charlen = 0;
char charbuf[0x100];
int status = 0;




static void queue_copy(char* buf, int len)
{
	int j,k;
	char* p = new.buf;
	if(p == 0)return;

	k = new.enq;
	for(j=0;j<len;j++)
	{
		if(buf[j] == 0)continue;
		if(buf[j] == 0x7)continue;
		if(buf[j] != 0x8)
		{
			p[k] = buf[j];
			k = (k+1)%0x100000;
		}
		else
		{
			k = (k+0xfffff)%0x100000;
			p[k] = 0;
			if(*(u32*)(buf+j) == 0x4b5b1b08)j+=3;
		}
	}
	new.enq = k;
}
static void terminal_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u8* p;
	int j,k;
	int enq,deq;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int w = (win->w) * (sty->wantw) / 0x20000;
	int h = (win->h) * (sty->wanth) / 0x20000;
	drawline_rect(win, 0xffffff,
		cx-w, cy-h, cx+w, cy+h
	);

	if(status == 0)
	{
		if(listlen == 0)
		{
			listlen = uart_list(listbuf);
			say("%.*s", listlen, listbuf);
		}

		drawvt100(win, 0xffffff,
			cx-w, cy-h, cx+w, cy+h,
			listbuf, listlen
		);
		return;
	}

	if(old == 0)return;
	p = old->buf;
	enq = old->enq;
	deq = old->deq;
	old->deq = enq;
	if(enq != deq)
	{
		if(enq > deq)
		{
			queue_copy(p+deq, enq-deq);
		}
		else
		{
			queue_copy(p+deq, 0x100000-deq);
			queue_copy(p, enq);
		}
	}

	enq = new.enq;
	p = new.buf;
	if(p == 0)return;

	k = 0;
	for(j=enq-1;j>0;j--)
	{
		if(p[j] == '\n')k++;
		if(k+1 > h/8)
		{
			j++;
			break;
		}
	}

	drawvt100(win, 0xffffff,
		cx-w, cy-h, cx+w, cy+h,
		p+j, 0x100000-j
	);
}
static void terminal_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void terminal_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void terminal_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void terminal_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	char* p;
	int enq, deq;
	if((status == 0)&&(charlen == 0))
	{
		if(listlen == 0)
		{
			say("terminal(%x,%x,%x)\n",win,act,sty);
			listlen = uart_list(listbuf);
		}
		say("%.*s", listlen, listbuf);
		return;
	}

	if(old == 0)return;
	p = old->buf;
	enq = old->enq;
	deq = old->deq;
	old->deq = enq;
	if(enq == deq)return;

	if(enq > deq)
	{
		//printmemory(p+deq, enq-deq);
		say("%.*s", enq-deq, p+deq);
	}
	else
	{
		//printmemory(p+deq, 0x200-deq);
		//printmemory(p, enq);
		say("%.*s%.*s", 0x100000-deq, p+deq, enq, p);
	}
}
static void terminal_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == __cli__)terminal_read_cli(win, act, sty);
	else if(fmt == __tui__)terminal_read_tui(win, act, sty);
	else if(fmt == __vbo__)terminal_read_vbo(win, act, sty);
	else if(fmt == __html__)terminal_read_html(win, act, sty);
	else terminal_read_pixel(win, act, sty);
}




static void terminal_write(struct event* ev)
{
	int j;
	if(ev->what == __uart__)
	{
		old = (void*)(ev->why);
		return;
	}
	if(ev->what == __char__)
	{
		j = (ev->why)&0xff;
		if(status != 0)
		{
			uart_write((void*)ev);
			return;
		}
		if(j == 0x8)
		{
			if(charlen > 0)
			{
				say("\b \b");
				charlen--;
				charbuf[charlen] = 0;
			}
			return;
		}

		say("%c",j);
		if((j == 0xd)|(j == 0xa))
		{
			if(charlen == 0)return;

			uart_choose(charbuf);
			status = 1;
			return;
		}
		if(charlen < 31)
		{
			charbuf[charlen] = j;
			charlen++;
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
	new.enq = 0;
	new.deq = 0;
	new.len = 0x100000;
	new.buf = startmemory(new.len);
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
