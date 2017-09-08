#include "actor.h"
void sudoku_solve(void*, void*);




//
void drawdecimal(
	void*, int data, int size,
	int x, int y, u32 fg, u32 bg);
void drawrect_body(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
void drawrect(void*,
	int x1, int y1,
	int x2, int y2,
	u32 body, u32 frame);




//
static int px,py;
static char table[9][9];
//
static u8* buffer;




static void sudoku_read_text(struct arena* win)
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
			for(j=-1;j<=1;j++)
			{
				for(k=-2;k<=3;k++)
				{
					p[ret +(j*width*4) +(k*4) +3] = color;
				}
			}

			//data
			p[ret] = table[y][x] + 0x30;
		}
	}
}
static void sudoku_read_html(struct arena* win)
{
}
static void sudoku_read_pixel(struct arena* win)
{
	int x,y;
	int w = win->w;
	int h = win->h;

	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			drawrect(win,
				x*w/9,	 y*h/9,
				(x+1)*w/9, (y+1)*h/9,
				0x888888,	  0
			);

			if(table[y][x] != 0)
			{
				drawdecimal(
					win, table[y][x], 4,
					x*w/9, y*h/9, 0, 0
				);
			}
		}
	}
	drawrect_body(win,
		0, (h/3)-2,
		w, (h/3)+2,
		0
	);
	drawrect_body(win,
		0, (h*2/3)-2,
		w, (h*2/3)+2,
		0
	);
	drawrect_body(win,
		(w/3)-2, 0,
		(w/3)+2, h,
		0
	);
	drawrect_body(win,
		(w*2/3)-2, 0,
		(w*2/3)+2, h,
		0
	);
}
static void sudoku_read_cli()
{
	int x,y;
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			if(table[y][x] == 0)continue;
			say("%d	",table[y][x]);
		}
		say("\n");
	}
	say("\n");
}
static void sudoku_read(struct arena* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		sudoku_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		sudoku_read_html(win);
	}

	//pixel
	else
	{
		sudoku_read_pixel(win);
	}
}
static void sudoku_write(struct event* ev)
{
	u64 what = ev->what;
	u64 key = ev->why;
	if(what == 0x64626b)
	{
		if(key == 0x25)	//left
		{
			if(px<1)return;
			px--;
		}
		else if(key == 0x26)	//up
		{
			if(py<1)return;
			py--;
		}
		else if(key == 0x27)	//right
		{
			if(px<0)return;
			if(px>=8)return;
			px++;
		}
		else if(key == 0x28)	//down
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
