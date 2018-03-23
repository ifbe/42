#include "actor.h"




//
static u8 buffer[8][8];




static void chess_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 color;
	int x,y;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			if(((x+y+32)%2) != 0)color = 0x111111;
			else color = 0xffffff;

			drawsolid_rect(win, color,
				(cx-ww)+(2*x+0)*ww/8, (cy-hh)+(2*y+0)*hh/8,
				(cx-ww)+(2*x+2)*ww/8, (cy-hh)+(2*y+2)*hh/8
			);
			if(buffer[y][x] == 0)continue;

			drawascii(win, 0xff00ff,
				(cx-ww-4)+(2*x+1)*ww/8, (cy-hh-8)+(2*y+1)*hh/8,
				buffer[y][x]
			);
		}
	}
}
static void chess_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 color;
	int x,y;
	float xxx, yyy;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			if(((x+y+32)%2) != 0)color = 0x111111;
			else color = 0xffffff;

			xxx = cx + (x+x-7)*ww/8;
			yyy = cy - (y+y-7)*hh/8;
			carvesolid_prism4(
				win, color,
				xxx, yyy, 0.0,
				ww/8, 0.0, 0.0,
				0.0, hh/8, 0.0,
				0.0, 0.0, 0.0
			);
			carvesolid_cone(
				win, 0xffff00,
				xxx, yyy, 0.0,
				ww/16, 0.0, 0.0,
				0.0, 0.0, ww/8
			);
		}
	}
}
static void chess_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void chess_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void chess_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u8 ch;
	int x,y;
	say("chess(%x,%x,%x)\n",win,act,sty);

	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			ch = buffer[y][x];
			if(ch <= 0x20)say("_	");
			else say("%c	", ch);
		}
		say("\n");
	}
}
static void chess_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)chess_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)chess_read_tui(win, sty, act, pin);
	else if(fmt == _vbo_)chess_read_vbo(win, sty, act, pin);
	else if(fmt == _html_)chess_read_html(win, sty, act, pin);
	else chess_read_pixel(win, sty, act, pin);
}
static void chess_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	//say("@chess:%x,%x\n", ev->why, ev->what);
}
static void chess_stop(struct actor* act, struct pinid* pin)
{
}
static void chess_start(struct actor* act, struct pinid* pin)
{
	int j,k;
	for(k=0;k<8;k++)
	{
		for(j=0;j<8;j++)
		{
			buffer[k][j] = 0;
		}
	}

	//
	for(j=0;j<8;j++)
	{
		buffer[1][j] = 'p';
		buffer[6][j] = 'P';
	}

	//
	buffer[0][0] = 'r';
	buffer[0][1] = 'n';
	buffer[0][2] = 'b';
	buffer[0][3] = 'k';
	buffer[0][4] = 'q';
	buffer[0][5] = 'b';
	buffer[0][6] = 'n';
	buffer[0][7] = 'r';

	//
	buffer[7][0] = 'R';
	buffer[7][1] = 'N';
	buffer[7][2] = 'B';
	buffer[7][3] = 'Q';
	buffer[7][4] = 'K';
	buffer[7][5] = 'B';
	buffer[7][6] = 'N';
	buffer[7][7] = 'R';
}
static void chess_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)stopmemory(act->buf);
}
static void chess_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = startmemory(64);
}
static void chess_list()
{
}
static void chess_choose()
{
}




void chess_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('c', 'h', 'e', 's', 's', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)chess_create;
	p->ondelete = (void*)chess_delete;
	p->onstart  = (void*)chess_start;
	p->onstop   = (void*)chess_stop;
	p->onlist   = (void*)chess_list;
	p->onchoose = (void*)chess_choose;
	p->onread   = (void*)chess_read;
	p->onwrite  = (void*)chess_write;
}
