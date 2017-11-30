#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void term_write(void*);
void actorchoose(void*);



static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static int chosen;




void login_read_pixel(struct arena* win)
{
	u32 color;
	int x,y,j;
	int w = (win->w)/8;
	int h = (win->h)/16;

	for(j=0;j<32;j++)
	{
		if(j == chosen)color = 0xff00ff;
		else color = 0x0000ff;

		x = j%4;
		y = j/4;
		drawicon_1(
			win, color,
			(x+2)*w, (y+4)*h, (x+3)*w, (y+5)*h,
			(u8*)&actor[j].name, 8
		);
	}
}
void login_read_vbo(struct arena* win)
{
	carveline_circle(
		win, 0x00bfff,
		0.0, 0.0, 0.0,
		0.5, 0.0, 0.0,
		0.0, 0.0, 1.0
	);

#define sqrt3div4 0.43301270189222
	carveline_triangle(
		win, 0x00bfff,
		0.0, 0.5, 0.0,
		sqrt3div4, -0.25, 0.0,
		-sqrt3div4, -0.25, 0.0
	);
	carveline_triangle(
		win, 0x00bfff,
		0.0, -0.5, 0.0,
		sqrt3div4, 0.25, 0.0,
		-sqrt3div4, 0.25, 0.0
	);
/*
	carveline_bezier(
		win, 0x00bfff,
		0.0, 0.5, 0.0,
		0.0, -0.5, 0.0,
		0.0, 0.0, 0.5
	);
	carveline_bezier(
		win, 0x00bfff,
		sqrt3div4, -0.25, 0.0,
		-sqrt3div4, 0.25, 0.0,
		0.0, 0.0, 1.0
	);
	carveline_bezier(
		win, 0x00bfff,
		sqrt3div4, 0.25, 0.0,
		-sqrt3div4, -0.25, 0.0,
		0.0, 0.0, 1.5
	);
*/
	carveline_special(
		win, 0x00bfff,
		0.0, 0.0, 0.0,
		0.5, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, PI
	);
	carveline_special(
		win, 0x00bfff,
		0.0, 0.0, 0.0,
		0.5, 0.0, 0.0,
		0.0, 0.0, 1.0,
		PI*2/3, PI
	);
	carveline_special(
		win, 0x00bfff,
		0.0, 0.0, 0.0,
		0.5, 0.0, 0.0,
		0.0, 0.0, 1.0,
		PI*4/3, PI
	);
}
void login_read_tui(struct arena* win)
{
}
void login_read_html(struct arena* win)
{
}
void login_read_cli(struct arena* win)
{
}
void login_read(struct arena* win)
{
	if(win->fmt == hex32('c','l','i',0))login_read_cli(win);
	else if(win->fmt == hex32('t','u','i',0))login_read_tui(win);
	else if(win->fmt == hex32('h','t','m','l'))login_read_html(win);
	else if(win->fmt == hex32('v','b','o',0))login_read_vbo(win);
	else login_read_pixel(win);
}
void login_write(struct arena* win, struct event* ev)
{
	int x,y;
	y = (ev->what)&0xff;
	x = ((ev->what)>>8)&0xff;
	if(y == 'p')
	{
		if(x == '@')
		{
			x = (ev->why)&0xffff;
			x = (x*8)>>16;
			y = ((ev->why)>>16)&0xffff;
			y = (y*16)>>16;
			chosen = (y-4)*4 + (x-2);
		}
		else if(x == '-')
		{
			if((chosen >= 0)&&(chosen<32))
			{
				actorchoose((void*)&actor[chosen].name);
			}
		}
	}
	else if(ev->what == hex32('c','h','a','r'))
	{
		term_write(ev);
	}
}
void login_create(void* addr)
{
	arena = addr + 0;
	actor = addr + 0x100000;
	style = addr + 0x200000;
}
void login_delete()
{
}