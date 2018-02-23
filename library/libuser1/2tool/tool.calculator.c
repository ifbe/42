#include "actor.h"
void postfix2binarytree(void* postfix, void* out);
void infix2postfix(void* infix, void* postfix);
double calculator(void* postfix, u64 x, u64 y);




static int count=0;
static u8 buffer[128];
static u8 infix[128];
static u8 postfix[128];
static u8 result[128];
//
static u8 table[4][8] = {
'0', '1', '2', '3', '+', '-', '*', '/',
'4', '5', '6', '7', '^', '%', '!', ' ',
'8', '9', 'a', 'b', '<', '>', '(', ')',
'c', 'd', 'e', 'f', '.', ' ', '`', '=',
};




static void calculator_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u32 fg;
	int x,y;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx / 4;
	int hh = sty->i_fy / 4;
	int dd = sty->i_uz / 4;

	//display
	drawsolid_rect(win, 0x222222, cx-ww*4, cy-hh*4, cx+ww*4, cy);
	drawstring(win, 0xffffff, cx-ww*4, cy-hh*4, buffer, 0);
	drawstring(win, 0xffffff, cx-ww*4, cy-hh*4+16, infix, 0);
	drawstring(win, 0xffffff, cx-ww*4, cy-hh*4+32, postfix, 0);
	drawstring(win, 0xffffff, cx-ww*4, cy-hh*4+48, result, 0);

	//keypad
	for(y=0;y<4;y++)
	{
		for(x=0;x<8;x++)
		{
			fg = 0x444444;
			if(x<4)fg += (y<<4) + (x<<20);
			else fg += (x<<4) + (y<<20);

			drawsolid_rect(win, fg,
				cx+ww*(x-4), cy+hh*(y+0),
				cx+ww*(x-3), cy+hh*(y+1)
			);
			drawascii(win, 0xffffff,
				cx+ww*(x-4), cy+hh*y, table[y][x]
			);
		}
	}
}
static void calculator_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void calculator_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void calculator_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void calculator_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("calc(%x,%x,%x)\n",win,act,sty);
	say("buffer:%s\n", infix);
	say("postfix:%s\n", postfix);
	say("result:%s\n", result);
}
static void calculator_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)calculator_read_cli(win, act, sty);
	else if(fmt == _tui_)calculator_read_tui(win, act, sty);
	else if(fmt == _html_)calculator_read_html(win, act, sty);
	else if(fmt == _vbo_)calculator_read_vbo(win, act, sty);
	else calculator_read_pixel(win, act, sty);
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

	if(type == _char_)
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
static void calculator_stop()
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
static void calculator_delete()
{
}
static void calculator_create()
{
}




void calculator_register(struct actor* p)
{
	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex32('c', 'a', 'l', 'c');
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)calculator_create;
	p->ondelete = (void*)calculator_delete;
	p->onstart  = (void*)calculator_start;
	p->onstop   = (void*)calculator_stop;
	p->onlist   = (void*)calculator_list;
	p->onchoose = (void*)calculator_change;
	p->onread   = (void*)calculator_read;
	p->onwrite  = (void*)calculator_write;
}