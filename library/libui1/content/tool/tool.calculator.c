#include "actor.h"
//
void drawstring(
	void*, void* str, int size,
	int x, int y, u32 fg, u32 bg);
void drawascii(
	void*, u8 ch, int size,
	int x, int y, u32 fg, u32 bg);
void drawrect(void*,
	int x1, int y1,
	int x2, int y2,
	u32 bc, u32 fc);
//
double calculator(char* postfix, u64 x, u64 y);
void postfix2binarytree(char* postfix,void* out);
void infix2postfix(char* infix,char* postfix);




static char infix[128];
static char postfix[128];
static char result[128];
//
static char buffer[128];
static int count=0;
//
static char table[4][8] = {
'0', '1', '2', '3', '+', '-', '*', '/',
'4', '5', '6', '7', '^', '%', '!', ' ',
'8', '9', 'a', 'b', '<', '>', '(', ')',
'c', 'd', 'e', 'f', '.', ' ', '`', '=',
};




static void calculator_read_pixel(struct arena* win, struct actor* act, struct relation* rel)
{
	u32 fg;
	int x,y;
	int w8 = (win->w)/8;
	int h8 = (win->h)/8;

	drawrect(win,
		0, 0,
		w8*8-1, h8*4-1,
		0, 0xff00
	);
	for(y=0;y<4;y++)
	{
		for(x=0;x<8;x++)
		{
			if(x<4)fg = y*0x10 + x*0x100000;
			else fg = x*0x10 + y*0x100000;

			drawrect(win,
				w8*x, h8*(y+4),
				w8*(x+1), h8*(y+5),
				fg, 0xffffffff
			);
			drawascii(
				win, table[y][x], 4,
				w8*x, h8*(y+4), 0xffffffff, 0
			);
		}
	}
	drawstring(
		win, buffer, 2,
		16, 16, 0xffffffff, 0xff000000
	);
	drawstring(
		win, infix, 2,
		16, 16+32, 0xffffffff, 0xff000000
	);
	drawstring(
		win, postfix, 2,
		16, 16+64, 0xffffffff, 0xff000000
	);
	drawstring(
		win, result, 2,
		16, 16+96, 0xffffffff, 0xff000000
	);
}
static void calculator_read_html(struct arena* win, struct actor* act, struct relation* rel)
{
}
static void calculator_read_text(struct arena* win, struct actor* act, struct relation* rel)
{
}
static void calculator_read_cli(struct arena* win, struct actor* act, struct relation* rel)
{
	say("buffer:%s\n", infix);
	say("postfix:%s\n", postfix);
	say("result:%s\n", result);
}
static void calculator_read(struct arena* win, struct actor* act, struct relation* rel)
{
	u64 fmt = win->fmt;

	//cli
	//if(rel->dim == 1)calculator_read_cli(win, act, rel);

	//text
	if(fmt == 0x74786574)calculator_read_text(win, act, rel);

	//html
	else if(fmt == 0x6c6d7468)calculator_read_html(win, act, rel);

	//pixel
	else calculator_read_pixel(win, act, rel);
}




static void calculator_write(struct event* ev)
{
	double final;
	int x,y,ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == 0x2d70)
	{
		x = key&0xffff;
		y = (key>>16)&0xffff;

		x = x*8 / 512;
		y = y*8 / 512;
		if(y < 4)return;

		type = 0x72616863;
		key = table[y-4][x];

		if(key == '`')key = 0x8;
		if(key == '=')key = 0xd;
	}

	if(type == 0x72616863)	       //'char'
	{
		if(key==0x8)		    //backspace
		{
			if(count <= 0)return;

			count--;
			buffer[count] = 0x20;
		}
		else if(key==0xd)	       //enter
		{
			//清空输入区
			for(ret=0;ret<count;ret++)
			{
				infix[ret] = buffer[ret];
				buffer[ret] = 0x20;
			}
			infix[count] = 0;
			count=0;
			say("buffer:%s\n", infix);

			infix2postfix(infix, postfix);
			say("postfix:%s\n", postfix);

			final = calculator(postfix, 0, 0);
			double2decstr(final, result);
			say("result:%s\n", result);
		}
		else
		{
			if(count<128)
			{
				buffer[count] = key;
				count++;
			}
		}
	}
}




static void calculator_list()
{
}
static void calculator_change()
{
}
static void calculator_start()
{
	int j;
	buffer[0] = '1';
	buffer[1] = '+';
	buffer[2] = '2';
	buffer[3] = 0;
	count = 3;
}
static void calculator_stop()
{
}
void calculator_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex32('c', 'a', 'l', 'c');

	p->start = (void*)calculator_start;
	p->stop = (void*)calculator_stop;
	p->list = (void*)calculator_list;
	p->choose = (void*)calculator_change;
	p->read = (void*)calculator_read;
	p->write = (void*)calculator_write;
}
void calculator_delete()
{
}
