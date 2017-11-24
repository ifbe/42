#include "actor.h"




void login_read_cli(struct arena* win)
{
}
void login_read_tui(struct arena* win)
{
}
void login_read_html(struct arena* win)
{
}
void login_read_vbo(struct arena* win)
{
	carvesolid_rect(
		win, 0x444444,
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0
	);
}
void login_read_pixel(struct arena* win)
{
	int w = win->w;
	int h = win->h;

	drawsolid_rect(
		win, 0xffffff,
		w/4, h/4, w*3/4, h*3/4
	);
}
void login_read(struct arena* win)
{
	//cli
	if(win->fmt == hex32('c','l','i',0))login_read_cli(win);

	//text
	else if(win->fmt == hex32('t','u','i',0))login_read_tui(win);

	//html
	else if(win->fmt == hex32('h','t','m','l'))login_read_html(win);

	//vbo
	else if(win->fmt == hex32('v','b','o',0))login_read_vbo(win);

	//pixel
	else login_read_pixel(win);
}