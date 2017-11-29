#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void term_write(void*);




void login_read_pixel(struct arena* win)
{
	int j;
	int w = win->w;
	int h = win->h;
	drawicon_1(
		win, 0xff00ff,
		w/4, h*2/8, w*3/4, h*3/8,
		"hello", 0
	);
	drawicon_1(
		win, 0x444444,
		w/4, h*3/8, w*3/4, h*4/8,
		"hello", 0
	);
	drawicon_1(
		win, 0x444444,
		w/4, h*4/8, w*3/4, h*5/8,
		"hello", 0
	);
	drawicon_1(
		win, 0x444444,
		w/4, h*5/8, w*3/4, h*6/8,
		"hello", 0
	);
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
	//say("%x,%x,%x,%x\n", ev->why, ev->what, ev->where, ev->when);
	if(ev->what == hex32('c','h','a','r'))
	{
		term_write(ev);
	}
}