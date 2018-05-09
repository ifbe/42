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




static void calculator_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 fg;
	int x,y;
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
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy);

	//display
	drawstring(win, 0xffffff, cx-ww, cy-hh, buffer, 0);
	drawstring(win, 0xffffff, cx-ww, cy-hh+16, infix, 0);
	drawstring(win, 0xffffff, cx-ww, cy-hh+32, postfix, 0);
	drawstring(win, 0xffffff, cx-ww, cy-hh+48, result, 0);

	//keypad
	for(y=0;y<4;y++)
	{
		for(x=0;x<8;x++)
		{
			fg = 0x444444;
			if(x<4)fg += (y<<4) + (x<<20);
			else fg += (x<<4) + (y<<20);

			drawsolid_rect(win, fg,
				cx+ww*(x-4)/4, cy+hh*(y+0)/4,
				cx+ww*(x-3)/4, cy+hh*(y+1)/4
			);
			drawascii(win, 0xffffff,
				cx+ww*(x-4)/4, cy+hh*y/4, table[y][x]
			);
		}
	}
}
static void calculator_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void calculator_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void calculator_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"calc\" style=\"width:50%%;height:100px;float:left;background-color:#8194ea;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void calculator_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void calculator_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("calc(%x,%x,%x)\n",win,act,sty);
	say("buffer:%s\n", infix);
	say("postfix:%s\n", postfix);
	say("result:%s\n", result);
}
static void calculator_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)calculator_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)calculator_read_tui(win, sty, act, pin);
	else if(fmt == _html_)calculator_read_html(win, sty, act, pin);
	else if(fmt == _json_)calculator_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)calculator_read_vbo(win, sty, act, pin);
	else calculator_read_pixel(win, sty, act, pin);
}
static void calculator_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	double final;
	int x,y,ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(_char_ == type)
	{
		if(0x8 == key)
		{
			if(count <= 0)return;

			count--;
			buffer[count] = 0x20;
		}
		else if(0xd == key)
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
static void calculator_stop(struct actor* act, struct pinid* pin)
{
}
static void calculator_start(struct actor* act, struct pinid* pin)
{
	int j;
	buffer[0] = '1';
	buffer[1] = '+';
	buffer[2] = '2';
	buffer[3] = 0;
	count = 3;
}
static void calculator_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void calculator_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(128);
}




void calculator_register(struct actor* p)
{
	p->type = _orig_;
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
