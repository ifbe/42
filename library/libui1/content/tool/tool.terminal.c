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
static u8 listbuf[0x100];
static int listlen = 0;
static u8 charbuf[0x100];
static int charlen = 0;
static int status = 0;




static int buffer_goto(u8* buf, int* ret, int x, int y)
{
	int j,k;

	j = *ret;
	for(;j>0;j--)
	{
		if(buf[j] == '\n')
		{
			if(y >= 25)break;
			else y++;
		}
	}

	for(k=0;k<x;k++)
	{
		if(buf[j+k] == '\n')break;
	}
	if(k>0)k--;

	say("%x->%x\n", *ret, j+k);
	*ret = j+k;
	return 0;
}
static int drawvt100_position(u8* p, int* xx, int* yy)
{
	int j, k, x=0, y=0;
	for(k=0;k<4;k++)
	{
		if(p[k] == ';')break;
	}
	if(k>=4)return 0;

	for(j=0;j<k;j++)
	{
		y = (y*10) + p[j] - 0x30;
	}

	for(;k<8;k++)
	{
		if((p[k] == 'H')|(p[k] == 'f'))break;
	}
	if(k>=8)return 0;

	j++;
	for(;j<k;j++)
	{
		x = (x*10) + p[j] - 0x30;
	}

	*xx = x;
	*yy = y;
	return 1;
}
static void queue_copy(u8* buf, int len)
{
	int i,j,k;
	int x,y,z;
	u8* p = new.buf;
	if(p == 0)return;

	k = new.enq;
	for(j=0;j<len;j++)
	{
		if(buf[j] == 0)continue;
		if(buf[j] == 0x7)continue;
		if(buf[j] == 0x8)
		{
			k = (k+0xfffff)%0x100000;
			p[k] = 0x20;
			continue;
		}
		if((buf[j] == 0xd)&&(buf[j+1] != 0xa))
		{
			for(x=k;x>0;x--)
			{
				y = (x+0xfffff)%0x100000;
				if(p[y] == '\n')
				{
					k = x;
					break;
				}
			}
			continue;
		}

		if((buf[j] == 0x1b)&&(buf[j+1] == 0x5b))
		{
			//Clear line from cursor right
			if(buf[j+2] == 'K')
			{
				for(x=0;x<256;x++)
				{
					if(k+x > 0xfffff)break;
					if(p[k+x] == 0)break;
					if(p[k+x] == '\n')break;

					p[k+x] = 0x20;
				}

				printmemory(buf+j, 3);
				j += 2;
				continue;
			}

			//Clear screen from cursor down
			if(buf[j+2] == 'J')
			{
				y = 0;
				for(x=0;x<128*25;x++)
				{
					if(k+x > 0xfffff)break;
					if(p[k+x] == 0)break;
					if(p[k+x] == '\n')
					{
						y++;
						if(y >= 25)break;
						else continue;
					}

					p[k+x] = 0x20;
				}

				printmemory(buf+j, 3);
				j += 2;
				continue;
			}

			//(0,0)
			if((buf[j+2] == 'H')|(buf[j+2] == 'f'))
			{
				buffer_goto(p, &k, 0, 0);

				printmemory(buf+j, 3);
				j+=2;
				continue;
			}

			if(buf[j+3] == 'A')
			{
				printmemory(buf+j, 4);
				j+=3;
				continue;
			}

			if(buf[j+3] == 'C')
			{
				x = buf[j+2]-0x30;
				k = (k+x)%0x100000;

				printmemory(buf+j, 4);
				j+=3;
				continue;
			}

			if(buf[j+4] == 'C')
			{
				x = (buf[j+2]-0x30)*10 + (buf[j+3]-0x30);
				k = (k+x)%0x100000;

				printmemory(buf+j, 5);
				j+=4;
				continue;
			}

			//position
			for(z=3;z<8;z++)
			{
				if((buf[j+z] == 'H')|(buf[j+z] == 'f'))
				{
					i = drawvt100_position(buf+2, &x, &y);
					if(i != 0)
					{
						buffer_goto(p, &k, x, y);

						printmemory(buf+j, z+1);
						j += z;
						continue;
					}
				}
			}
		}

		p[k] = buf[j];
		k = (k+1)%0x100000;
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
	int ww = (win->w) * (sty->wantw) / 0x20000;
	int hh = (win->h) * (sty->wanth) / 0x20000;

	drawline_rect(win, 0xffffff,
		cx-ww, cy-hh, cx+ww, cy+hh
	);

	if(status == 0)
	{
		if(listlen == 0)
		{
			listlen = uart_list(listbuf);
			if(listlen != 0)say("%.*s", listlen, listbuf);
		}

		drawtext(win, 0xffffff,
			cx-ww, cy-hh, cx+ww, cy+hh,
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
		if(k+1 > hh/8)
		{
			j++;
			break;
		}
	}

	drawvt100(win, 0xffffff,
		cx-ww, cy-hh, cx+ww, cy+hh,
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
	u8* p;
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
	u64 temp;
	if(ev->what == __uart__)
	{
		old = (void*)(ev->why);
		return;
	}

	if(ev->what == __kbd__)
	{
		j = ev->why;
		if(status != 0)
		{
			if(j == 0x25)temp = 0x445b1b;
			else if(j == 0x26)temp = 0x415b1b;
			else if(j == 0x27)temp = 0x435b1b;
			else if(j == 0x28)temp = 0x425b1b;
			else return;

			uart_write((void*)&temp);
		}
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
