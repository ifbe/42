#include "actor.h"
void carveaxis(void*);




void background_read_cli(struct arena* win)
{
}
void background_read_tui(struct arena* win)
{
}
void background_read_html(struct arena* win)
{
}
void background_read_vbo(struct arena* win)
{
	carveaxis(win);
}
void background_read_pixel(struct arena* win)
{
	int x;
	int len = (win->w)*(win->h);
	u32* buf = (u32*)(win->buf);

	for(x=0; x<len; x++)buf[x] = 0xff000000;
}
void background(struct arena* win)
{
	//cli
	if(win->fmt == hex32('c','l','i',0))background_read_cli(win);

	//text
	else if(win->fmt == hex32('t','u','i',0))background_read_tui(win);

	//html
	else if(win->fmt == hex32('h','t','m','l'))background_read_html(win);

	//vbo
	else if(win->fmt == hex32('v','b','o',0))background_read_vbo(win);

	//pixel
	else background_read_pixel(win);
}
void foreground(struct arena* win)
{
}