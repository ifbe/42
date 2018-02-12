#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void term_write(void*);
void carvestarry_random(void*);
void act_at(void*, void*);
void draw8bit_rect(struct arena* win, u8 rgb, int x0, int y0, int x1, int y1);
void carveascii_area(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz);




static struct arena* arena = 0;
static struct actor* actor = 0;
static int chosen = 0;




void login_read_pixel(struct arena* win)
{
	u32 color;
	int x,y,j;
	int w = (win->w)/8;
	int h = (win->h)/16;

	for(j=0;j<32;j++)
	{
		if(j == chosen)color = 0x80ff00ff;
		else color = 0x800000ff;

		x = j%4;
		y = j/4;
		drawicon_1(
			win, color,
			(x+2)*w, (y+4)*h, (x+3)*w, (y+5)*h,
			(u8*)&actor[j].name, 8
		);
	}
}
void login_read_8bit(struct arena* win)
{
	int x,y;
	int j,c;
	for(j=0;j<32;j++)
	{
		if(j == chosen)c = 0x80;
		else c = 0x42;

		x = j%4;
		y = j/4;
		draw8bit_rect(
			win, c,
			 81+x*40, 61+y*10,
			118+x*40, 68+y*10
		);
	}
}
void login_read_vbo(struct arena* win)
{
	u32 color;
	int j,k;
	float x,y;

	carveline(
		win, 0xffffff,
		0.0, 0.0, 0.0,
		0.0, 0.0, 1.0
	);
	carveascii_area(
		win, 0xffffff,
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0
	);

	for(j=0;j<32;j++)
	{
		if(j == chosen)
		{
			k = 4.0;
			color = 0x00ff00;
		}
		else
		{
			k = 1.0;
			color = 0xffffff;
		}

		x = cosine(j/PI)/8.0;
		y = sine(j/PI)/8.0;
		carvestring(
			win, color,
			x, y, (j+1)/64.0,
			x/8, y/8, 0.0,
			-y/8, x/8, 0.0,
			(u8*)&actor[j].name, 8
		);
	}
}
/*
void login_read_vbo(struct arena* win)
{
	u32 color;
	int j,k,x,y;

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

	for(j=0;j<32;j++)
	{
		if(j == chosen)
		{
			k = 4.0;
			color = 0xff00ff;
		}
		else
		{
			k = 1.0;
			color = 0x0000ff;
		}

		x = j%4;
		y = j/4;
		carvestring(
			win, color,
			0.25*(x-1.5), 0.125*(y-3.5), 0.0,
			1.0/64.0, 0.0, 0.0,
			0.0, k/64.0, k/64.0,
			(u8*)&actor[j].name, 8
		);
	}
}
*/
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
	else if(win->fmt == __8bit__)login_read_8bit(win);
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
			act_at(win, &actor[chosen]);
		}
	}
	else if(ev->what == __kbd__)
	{
		if(ev->why == 0x4b)
		{
			chosen = (chosen+31)%32;
		}
		else if(ev->why == 0x4d)
		{
			chosen = (chosen+1)%32;
		}
	}
	else if(ev->what == __char__)
	{
		if((ev->why == 0xd)|(ev->why == 0xa))
		{
			act_at(win, &actor[chosen]);
		}
		else if(ev->why == 0x435b1b)
		{
			chosen = (chosen+1)%32;
		}
		else if(ev->why == 0x445b1b)
		{
			chosen = (chosen+31)%32;
		}
		else
		{
			term_write(ev);
		}
	}
}
void login_create(void* addr)
{
	arena = addr + 0;
	actor = addr + 0x100000;
}
void login_delete()
{
}
