#include "actor.h"
void sudoku_solve(void*, void*);




//
static int px,py;
static char table[9][9];
//
static u8* buffer;




static void sudoku_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void sudoku_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	int t1, t2, t3, t4;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx *2/9;
	int hh = sty->i_fy *2/9;
	int dd = sty->i_uz *2/9;

	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			t1 = cx+(2*x-9)*ww/2;
			t2 = cy+(2*y-9)*hh/2;
			t3 = cx+(2*x-7)*ww/2;
			t4 = cy+(2*y-7)*hh/2;
			drawsolid_rect(win, 0xcccccc, t1, t2, t3, t4);
			drawline_rect(win, 0x222222, t1, t2, t3, t4);

			if(table[y][x] != 0)
			{
				drawdecimal(win, 0,
					cx+(2*x-9)*ww/2,
					cy+(2*y-9)*hh/2,
					table[y][x]
				);
			}
		}
	}
}
static void sudoku_read_vbo(struct arena* win, struct actor* act, struct style* sty)
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
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			if((x>2)&&(x<6)&&(y>2)&&(y<6))color = 0xcccccc;
			else if((x<3)&&(y<3))color = 0x444444;
			else if((x<3)&&(y>5))color = 0x444444;
			else if((x>5)&&(y<3))color = 0x444444;
			else if((x>5)&&(y>5))color = 0x444444;
			else color = 0x888888;

			xxx = cx + (x+x-8)*ww/18;
			yyy = cy - (y+y-8)*hh/18;
			carvesolid_rect(
				win, color,
				xxx, yyy, 0.0,
				ww/18, 0.0, 0.0,
				0.0, hh/18, 0.0
			);
		}
	}
}
static void sudoku_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y,j,k,ret,color;
	int width = win->w;
	int height = win->h;
	char* p = (char*)(win->buf);

	for(x=0;x<width*height*4;x++)p[x] = 0;
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			if(table[y][x] == 0)continue;

			//position
			ret = (3*y+1)*width + 6*x + 2;
			ret <<= 2;

			//color
			if( (px == x)&&(py == y) )color = 1;
			else if( ((x>2)&&(x<6)) && ((y<3)|(y>5)) )color = 2;
			else if( ((y>2)&&(y<6)) && ((x<3)|(x>5)) )color = 2;
			else color = 4;
			gentui_rect(win, color, x*6, y*3, x*6+5, y*3+2);

			//data
			p[ret] = table[y][x] + 0x30;
		}
	}
}
static void sudoku_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	say("sudoku(%x,%x,%x)\n",win,act,sty);

	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			if(table[y][x] == 0)continue;
			say("%d	",table[y][x]);
		}
		say("\n");
	}
}
static void sudoku_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)sudoku_read_cli(win, act, sty);
	else if(fmt == _tui_)sudoku_read_tui(win, act, sty);
	else if(fmt == _html_)sudoku_read_html(win, act, sty);
	else if(fmt == _vbo_)sudoku_read_vbo(win, act, sty);
	else sudoku_read_pixel(win, act, sty);
}
static void sudoku_write(struct event* ev)
{
	u64 what = ev->what;
	u64 key = ev->why;
	if(what == _kbd_)
	{
		if(key == 0x48)	//up
		{
			if(py<1)return;
			py--;
		}
		else if(key == 0x4b)	//left
		{
			if(px<1)return;
			px--;
		}
		else if(key == 0x4d)	//right
		{
			if(px<0)return;
			if(px>=8)return;
			px++;
		}
		else if(key == 0x50)	//down
		{
			if(py<0)return;
			if(py>=8)return;
			py++;
		}
	}
}








static void sudoku_list()
{
}
static void sudoku_choose()
{
}
static void sudoku_start()
{
	int x,y;
	px = py = 0;

	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			table[y][x] = 0;
		}
	}
	sudoku_solve(table, buffer);
}
static void sudoku_stop()
{
}
void sudoku_create(void* base,void* addr)
{
	struct actor* p = addr;
	buffer = base+0x300000;

	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('s', 'u', 'd', 'o', 'k', 'u', 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->start = (void*)sudoku_start;
	p->stop = (void*)sudoku_stop;
	p->list = (void*)sudoku_list;
	p->choose = (void*)sudoku_choose;
	p->read = (void*)sudoku_read;
	p->write = (void*)sudoku_write;
}
void sudoku_delete()
{
}
