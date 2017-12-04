#include "actor.h"
struct uartinfo
{
	u8* buf;
	int len;
	int enq;
	int deq;
};
struct uartterm
{
	u8* buf;
	u32 len;
	u32 fg;
	u32 bg;
	int w;
	int h;
	int x;
	int y;
	int left;
	int right;
	int top;
	int bottom;
};
int uart_list(void*);
int uart_choose(void*);
int uart_write(void*);
void drawterm(struct arena* win, struct uartterm* term, int x0, int y0, int x1, int y1);




static struct uartinfo* old;
static struct uartterm term;
static u8 listbuf[0x100];
static int listlen = 0;
static u8 charbuf[0x100];
static int charlen = 0;
static int status = 0;



/*
static int buffer_goto(u8* buf, int* ret, int x, int y)
{
	int j = new.deq;
	int k = *ret;
	x--;
	y--;

	if(y != 0)
	{
		if(y > 24)y = 24;
		for(;j<k;j++)
		{
			if(buf[j] == '\n')
			{
				y--;
				if(y == 0)
				{
					j++;
					break;
				}
			}
		}
	}

	for(k=0;k<x;k++)
	{
		if(buf[j+k] == '\n')break;
	}

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
	printmemory(buf,len);

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
		if((buf[j] == 0x1b)&&(buf[j+1] == '='))
		{
			j++;
			printf("e=\n");
			continue;
		}
		if((buf[j] == 0x1b)&&(buf[j+1] == '>'))
		{
			j++;
			printf("e>\n");
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

			//(1,1)
			if((buf[j+2] == 'H')|(buf[j+2] == 'f'))
			{
				buffer_goto(p, &k, 1, 1);

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

				printf("i am fucking here\n");
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
*/
int queue_1b_color(u8* p)
{
	//reset
	if(p == 0)
	{
		term.fg = 7;
		term.bg = 0;
		return 0;
	}

	//reset
	if(p[0] == '0')
	{
		term.fg = 7;
		term.bg = 0;
		return 0;
	}

	//heavy
	else if(p[0] == '1')
	{
		term.fg = 1;
		return 1;
	}

	//foreground
	if(p[0] == '3')
	{
		if((p[1] >= '0')&&(p[1] <= '7'))term.fg = p[1]-0x30;
		return 3;
	}

	//background
	else if(p[0] == '4')
	{
		if((p[1] >= '0')&&(p[1] <= '7'))term.bg = p[1]-0x30;
		return 4;
	}
	return -1;
}
static int queue_1b_position(u8* p)
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

	say("position:%d,%d\n",x-1,y-1);
	term.x = x-1;
	term.y = (term.top)+y-1;
	return 1;
}
static int queue_1b(u8* p)
{
	int j;
	int x,y,w;
	u8* buf = term.buf;
	if(p[0] != 0x1b)return 0;
	if(p[1] != 0x5b)return 1;
	//printmemory(p, 16);

	//1b 5b 41: cursor up
	if(p[2] == 0x41)
	{
		//printf("a");
		if(term.y > 0)term.y--;
		return 3;
	}

	//1b 5b 42: cursor down
	if(p[2] == 0x42)
	{
		//printf("b");
		term.y++;
		return 3;
	}

	//1b 5b 43: cursor forward
	if(p[2] == 0x43)
	{
		//printf("c");
		if(term.x < term.w-1)term.x++;
		return 3;
	}

	//1b 5b 44: cursor backward
	if(p[2] == 0x44)
	{
		//printf("d");
		if(term.x > 0)term.x--;
		return 3;
	}

	//1b 5b H/f
	if((p[2] == 'H')|(p[2] == 'f'))
	{
		term.x = 0;
		term.y = term.top;
		return 3;
	}

	//1b 5b J: Clear screen from cursor down
	if(p[2] == 'J')
	{
		x = term.x;
		y = term.y;
		w = term.w;
		buf = (term.buf) + y*w*4;
		for(j=x*4;j<w*25*4;j+=4)
		{
			buf[j] = 0x20;
			buf[j+2] = term.fg;
			buf[j+3] = term.bg;
		}
		return 3;
	}

	//1b 5b K: Clear line from cursor right
	if(p[2] == 'K')
	{
		x = term.x;
		y = term.y;
		w = term.w;
		buf = (term.buf) + y*w*4;
		for(j=x*4;j<w*4;j+=4)
		{
			buf[j] = 0x20;
			buf[j+2] = term.fg;
			buf[j+3] = term.bg;
		}
		return 3;
	}

	//1b 5b m
	if(p[2] == 'm')
	{
		queue_1b_color(0);
		return 3;
	}

	//1b 5b ? m
	if(p[3] == 'm')
	{
		queue_1b_color(p+2);
		return 4;
	}

	//1b 5b ? n
	if(p[3] == 'n')
	{
		return 4;
	}

	if(p[3] == 'A')
	{
		return 4;
	}

	if(p[3] == 'C')
	{
		x = p[2]-0x30;
		term.x += x;
		return 4;
	}

	if(p[4] == 'C')
	{
		x = (p[2]-0x30)*10 + (p[3]-0x30);
		term.x += x;
		return 5;
	}

	//1b 5b ? ? h
	if(p[4] == 'h')
	{
		return 5;
	}

	//1b 5b ? ? l
	if(p[4] == 'l')
	{
		return 5;
	}

	//1b 5b ? ? m
	if(p[4] == 'm')
	{
		queue_1b_color(p+2);
		return 5;
	}

	if(p[3] == ';')
	{
		//1b 5b ? ; ? m
		if(p[5] == 'm')
		{
			queue_1b_color(p+2);
			queue_1b_color(p+4);
			return 6;
		}

		//1b 5b ? ; ? ? m
		else if(p[6] == 'm')
		{
			queue_1b_color(p+2);
			queue_1b_color(p+4);
			return 7;
		}
	}

	//1b 5b ? ? ; ? ? m
	if( (p[4] == ';') && (p[7] == 'm') )
	{
		queue_1b_color(p+2);
		queue_1b_color(p+5);
		return 8;
	}

	for(j=2;j<10;j++)
	{
		if(p[j] == 'r')return j+1;
		if( (p[j] == 'H') | (p[j] == 'f') )
		{
			queue_1b_position(p+2);
			return j+1;
		}
	}

	printmemory(p, 16);
	return 2;
}
static void queue_copy(u8* buf, int len)
{
	int j,k;
	int x,y;
	int w = term.w;
	int h = term.h;
	u8* dst = term.buf;
	//printmemory(buf,len);

	for(j=0;j<len;j++)
	{
		if(buf[j] == 0)continue;
		if(buf[j] == 0x7)continue;

		if(buf[j] == 0x8)
		{
			x = term.x;
			if(x > 0)term.x = x-1;
		}
		else if(buf[j] == '\r')
		{
			term.x = 0;
		}
		else if(buf[j] == '\n')
		{
			term.y++;
			if(w*(term.y) >= 0xf0000)
			{
				term.y = 0;
				term.top = 0;
			}
			else if(term.top < term.y - 24)
			{
				term.top = term.y - 24;
			}
		}
		else if(buf[j] == 0x1b)
		{
			if(buf[j+1] == '=')j++;
			else if(buf[j+1] == '>')j++;
			else
			{
				j += queue_1b(buf+j) - 1;
			}
		}
		else if(buf[j] >= 0x80)
		{
			if(buf[j] < 0xe0)k = 2;
			else if(buf[j] < 0xf0)k = 3;
			else if(buf[j] < 0xf8)k = 4;
			else if(buf[j] < 0xfc)k = 5;
			else if(buf[j] < 0xfe)k = 6;
			else k = 1;

			y = 4*(w*(term.y) + term.x);
			dst[y + 7] = term.bg;
			dst[y + 6] = term.fg;
			for(x=0;x<k;x++)dst[y+x] = buf[j+x];

			term.x += 2;
			j += k-1;
		}
		else
		{
			y = 4*(w*(term.y) + term.x);
			dst[y + 3] = term.bg;
			dst[y + 2] = term.fg;
			dst[y + 0] = buf[j];

			term.x++;
		}
	}
}
static void terminal_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u8* p;
	int enq,deq;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x20000;
	int hh = (win->h) * (sty->wanth) / 0x20000;
	drawhyaline_rect(win, 0x111111, cx-ww, cy-hh, cx+ww, cy+hh);

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
		if(enq > deq)queue_copy(p+deq, enq-deq);
		else
		{
			queue_copy(p+deq, 0x100000-deq);
			queue_copy(p, enq);
		}
	}
	drawterm(win, &term, cx-ww, cy-hh, cx+ww, cy+hh);
}
static void terminal_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void terminal_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void terminal_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
	int x, y, w, h, enq,deq;
	u32* p;
	u32* q;
	u8* buf;
	if((status == 0)&&(charlen == 0))
	{
		if(listlen == 0)
		{
			listlen = uart_list(listbuf);
		}
		gentui_text(win, 7, 0, 0, listbuf, listlen);
		return;
	}

	if(old == 0)return;
	buf = old->buf;
	enq = old->enq;
	deq = old->deq;
	old->deq = enq;

	if(enq != deq)
	{
		if(enq > deq)queue_copy(buf+deq, enq-deq);
		else
		{
			queue_copy(buf+deq, 0x100000-deq);
			queue_copy(buf, enq);
		}
	}

	w = win->w;
	if(w > term.w)w = term.w;
	h = win->h;
	if(h > term.h)h = term.h;

	p = (void*)(win->buf);
	q = (void*)(term.buf);
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			p[(win->w)*y + x] = q[(term.w)*(term.top+y) + x];
		}
	}
}
static void terminal_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	u8* p;
	int enq, deq;
	if((status == 0)&&(charlen == 0))
	{
		if(listlen == 0)
		{
			//say("terminal(%x,%x,%x)\n",win,act,sty);
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

	if(enq > deq)say("%.*s", enq-deq, p+deq);
	else say("%.*s%.*s", 0x100000-deq, p+deq, enq, p);
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
	u64 tmp;
	u64 why = ev->why;
	if(ev->what == __uart__)
	{
		old = (void*)(ev->why);
		return;
	}

	if(ev->what == __kbd__)
	{
		if(status != 0)
		{
			if(why == 0x1b)tmp = 0x1b;
			else if(why == 0x25)tmp = 0x445b1b;
			else if(why == 0x26)tmp = 0x415b1b;
			else if(why == 0x27)tmp = 0x435b1b;
			else if(why == 0x28)tmp = 0x425b1b;
			else return;

			uart_write((void*)&tmp);
		}
		return;
	}

	if(ev->what == __char__)
	{
		if(status != 0)
		{
			uart_write((void*)ev);
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
				*(u32*)charbuf = hex32('C','O','M','7');
				charlen = 4;
			}

			uart_choose(charbuf);
			status = 1;
			return;
		}
		if(charlen < 31)
		{
			charbuf[charlen] = why;
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
	//new.enq = 0;
	//new.deq = 0;
	//new.len = 0x100000;
	//new.buf = startmemory(new.len);

	term.left = 0;
	term.right = 0;
	term.top = 0;
	term.bottom = 0;
	term.x = 0;
	term.y = 0;
	term.w = 128;
	term.h = 25;
	term.bg = 0;
	term.fg = 7;
	term.len = 0x100000;
	term.buf = startmemory(term.len);
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
