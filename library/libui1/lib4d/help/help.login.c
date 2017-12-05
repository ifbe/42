#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void term_write(void*);
void actorchoose(void*);



static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static int chosen = 0;
static void login_this()
{
	if(chosen < 0)return;
	if(chosen > 31)return;
	actorchoose((void*)&actor[chosen].name);
}




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
void login_read_html(struct arena* win)
{
}
void login_read_tui(struct arena* win)
{
	int j,k,x,y;
	int ww = ((win->w)/2)&0xfffc;
	int hh = (win->h)/2;

	gentui_rect(win, 4, ww/2, hh/2, ww*3/2, hh*3/2);

	for(j=0;j<32;j++)
	{
		x = j%4;
		x = ww + (x-2)*ww/4;
		y = j/4;
		y = hh + (y-4);
		if(j==chosen)k=1;
		else k=2;

		gentui_rect(win, k, x, y, x+7, y);
		gentui_str(win, 0, x, y, (u8*)&actor[j].name, 8);
	}
}
void login_read_cli(struct arena* win)
{
}
void login_read(struct arena* win)
{
	if(win->fmt == __cli__)login_read_cli(win);
	else if(win->fmt == __tui__)login_read_tui(win);
	else if(win->fmt == __vbo__)login_read_vbo(win);
	else if(win->fmt == __html__)login_read_html(win);
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
		else if(x == '-')login_this();
	}
	else if(ev->what == __char__)
	{
		if(ev->why == '=')login_this();
		else if(ev->why == '+')chosen = (chosen+1)%32;
		else if(ev->why == '-')chosen = (chosen+31)%32;
		else term_write(ev);
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