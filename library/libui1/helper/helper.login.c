#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151




void login_read_pixel(struct arena* win)
{
	int j;
	int w = win->w;
	int h = win->h;
	drawicon_1(win, w/4, h/4, w*3/4, h*3/4);

	j = 5 * 4 * 4;
	drawstring(win, "hello", 4, (w/2)-j, (h/2)-32, 0, 0);
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