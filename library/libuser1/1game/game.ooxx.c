#include "actor.h"




static int turn;
static u8 data[9];




static void ooxx_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);

	//heng
	drawline(win, 0xffffff, cx-ww, cy-hh/3, cx+ww, cy-hh/3);
	drawline(win, 0xffffff, cx-ww, cy+hh/3, cx+ww, cy+hh/3);

	//shu
	drawline(win, 0xffffff, cx-ww/3, cy-hh, cx-ww/3, cy+hh);
	drawline(win, 0xffffff, cx+ww/3, cy-hh, cx+ww/3, cy+hh);

	//ox
	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			if(data[3*y + x] == 'o')
			{
				drawline_circle(win, 0xff,
					cx+(x-1)*ww*2/3, cy+(y-1)*hh/3, ww/6
				);
			}
			else if(data[3*y + x] == 'x')
			{
				drawline(win, 0xff0000,
					cx+(4*x-5)*ww/6, cy+(4*y-5)*hh/6,
					cx+(4*x-3)*ww/6, cy+(4*y-3)*hh/6
				);
				drawline(win, 0xff0000,
					cx+(4*x-5)*ww/6, cy+(4*y-3)*hh/6,
					cx+(4*x-3)*ww/6, cy+(4*y-5)*hh/6
				);
			}
		}//forx
	}//fory
}
static void ooxx_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
	carvesolid_sphere(
		win, 0xffffff,
		cx, cy, 0.0,
		ww, 0.0, 0.0,
		0.0, 0.0, hh
	);
}
static void ooxx_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void ooxx_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void ooxx_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	u8 ch;
	int x,y;
	say("ooxx(%x,%x,%x)\n",win,act,sty);

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			ch = data[y*3 + x];
			if((ch!='o') && (ch!='x'))say("_	");
			else say("%c	",ch);
		}
		say("\n");
	}
}
static void ooxx_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)ooxx_read_cli(win, act, sty);
	else if(fmt == _tui_)ooxx_read_tui(win, act, sty);
	else if(fmt == _vbo_)ooxx_read_vbo(win, act, sty);
	else if(fmt == _html_)ooxx_read_html(win, act, sty);
	else ooxx_read_pixel(win, act, sty);
}




void ooxx_write(struct event* ev)
{
	char val;
	int x,y;
/*
	int width = haha->width;
	int height = haha->height;
	int min = (width<height) ? width:height;
*/
	int min = 512;
	if(ev->what == 0x2d70)
	{
		x=(ev->why) & 0xffff;
		if(x>min)return;

		y=( (ev->why) >> 16 ) & 0xffff;
		if(y>min)return;

		x = x*3/min;
		y = y*3/min;
say("%d,%d\n",x,y);
		if(data[y*3 + x] != 0)return;

		if((turn&0x1) == 0x1)val='o';
		else val='x';

		data[y*3 + x] = val;
		turn++;
	}
}




static void ooxx_list()
{
}
static void ooxx_choose()
{
}
static void ooxx_start()
{
	int j;

	turn=0;
	for(j=0;j<9;j++)data[j]=0;
}
static void ooxx_stop()
{
}
void ooxx_delete()
{
}
void ooxx_create(void* base, struct actor* act)
{
	act->type = hex32('g', 'a', 'm', 'e');
	act->name = hex32('o', 'o', 'x', 'x');
	act->irel = 0;
	act->orel = 0;

	act->start = (void*)ooxx_start;
	act->stop = (void*)ooxx_stop;
	act->list = (void*)ooxx_list;
	act->choose = (void*)ooxx_choose;
	act->read = (void*)ooxx_read;
	act->write = (void*)ooxx_write;
}
