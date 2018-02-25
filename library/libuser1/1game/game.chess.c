#include "actor.h"




//
static u8 buffer[8][8];




static void chess_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u32 color;
	int x,y;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx/4;
	int hh = sty->i_fy/4;
	int dd = sty->i_uz;
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			if(((x+y+32)%2) != 0)color = 0x111111;
			else color = 0xffffff;

			drawsolid_rect(win, color,
				cx+(x-4)*ww, cy+(y-4)*hh,
				cx+(x-3)*ww, cy+(y-3)*hh
			);
			if(buffer[y][x] == 0)continue;

			drawascii(win, 0xff00ff,
				cx+(x-4)*ww, cy+(y-4)*hh,
				buffer[y][x]
			);
		}
	}
}
static void chess_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u32 color;
	int x,y;
	float xxx, yyy;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			if(((x+y+32)%2) != 0)color = 0x111111;
			else color = 0xffffff;

			xxx = cx + (x+x-7)*ww/16;
			yyy = cy - (y+y-7)*hh/16;
			carvesolid_prism4(
				win, color,
				xxx, yyy, 0.0,
				ww/16, 0.0, 0.0,
				0.0, hh/16, 0.0,
				0.0, 0.0, 0.0
			);
		}
	}
}
static void chess_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void chess_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void chess_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
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
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)chess_read_cli(win, sty, act, com);
	else if(fmt == _tui_)chess_read_tui(win, sty, act, com);
	else if(fmt == _vbo_)chess_read_vbo(win, sty, act, com);
	else if(fmt == _html_)chess_read_html(win, sty, act, com);
	else chess_read_pixel(win, sty, act, com);
}
static void chess_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
	//say("@chess:%x,%x\n", ev->why, ev->what);
}




static void chess_list()
{
}
static void chess_choose()
{
}
static void chess_start()
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
static void chess_stop()
{
}
static void chess_delete()
{
}
static void chess_create()
{
}




void chess_register(struct actor* p)
{
	p->type = hex32('g', 'a', 'm', 'e');
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