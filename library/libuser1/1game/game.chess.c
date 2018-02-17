#include "actor.h"




//
static u8 buffer[8][8];




static void chess_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u32 color;
	int x,y;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int w = (win->w) * (sty->wantw) / 0x10000 / 8;
	int h = (win->h) * (sty->wanth) / 0x10000 / 8;
	if(w >= h)w=h;
	else h=w;

	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			if(((x+y+32)%2) != 0)color = 0x111111;
			else color = 0xffffff;

			drawsolid_rect(win, color,
				cx+(x-4)*w, cy+(y-4)*h,
				cx+(x-3)*w, cy+(y-3)*h
			);
			if(buffer[y][x] == 0)continue;

			drawascii(win, 0xff00ff,
				cx+(x-4)*w, cy+(y-4)*h,
				buffer[y][x]
			);
		}
	}
}
static void chess_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	u32 color;
	int x,y;
	float xxx, yyy;

	float cx = (float)(sty->cx) / 65536.0 - 0.5;
	float cy = (float)(sty->cy) / 65536.0 - 0.5;
	float w = (float)(sty->wantw) / 65536.0;
	float h = (float)(sty->wanth) / 65536.0;

	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			if(((x+y+32)%2) != 0)color = 0x111111;
			else color = 0xffffff;

			xxx = cx + (x+x-7)*w/16;
			yyy = cy - (y+y-7)*h/16;
			carvesolid_prism4(
				win, color,
				xxx, yyy, 0.0,
				w/16, 0.0, 0.0,
				0.0, h/16, 0.0,
				0.0, 0.0, 0.0
			);
		}
	}
}
static void chess_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void chess_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void chess_read_cli(struct arena* win, struct actor* act, struct style* sty)
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
static void chess_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)chess_read_cli(win, act, sty);
	else if(fmt == _tui_)chess_read_tui(win, act, sty);
	else if(fmt == _vbo_)chess_read_vbo(win, act, sty);
	else if(fmt == _html_)chess_read_html(win, act, sty);
	else chess_read_pixel(win, act, sty);
}
static void chess_write(struct event* ev)
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
void chess_delete()
{
}
void chess_create(void* base, struct actor* act)
{
	act->type = hex32('g', 'a', 'm', 'e');
	act->name = hex64('c', 'h', 'e', 's', 's', 0, 0, 0);
	act->irel = 0;
	act->orel = 0;

	act->start = (void*)chess_start;
	act->stop = (void*)chess_stop;
	act->list = (void*)chess_list;
	act->choose = (void*)chess_choose;
	act->read = (void*)chess_read;
	act->write = (void*)chess_write;
}