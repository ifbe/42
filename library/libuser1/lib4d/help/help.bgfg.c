#include "actor.h"
void carveaxis(void*);




void background_cli(struct arena* win)
{
}
void background_tui(struct arena* win)
{
}
void background_html(struct arena* win)
{
}
void background_vbo(struct arena* win)
{
	carveaxis(win);
}
void background_pixel(struct arena* win)
{
	int x;
	int len = (win->w)*(win->h);
	u32* buf = (u32*)(win->buf);

	for(x=0; x<len; x++)buf[x] = 0xff000000;
}
void background(struct arena* win)
{
	if(win->fmt == hex32('c','l','i',0))background_cli(win);
	else if(win->fmt == hex32('t','u','i',0))background_tui(win);
	else if(win->fmt == hex32('h','t','m','l'))background_html(win);
	else if(win->fmt == hex32('v','b','o',0))background_vbo(win);
	else background_pixel(win);
}




void foreground_cli(struct arena* win)
{
}
void foreground_tui(struct arena* win)
{
}
void foreground_html(struct arena* win)
{
}
void foreground_vbo(struct arena* win)
{
}
void foreground_pixel(struct arena* win)
{
}
void foreground(struct arena* win)
{
	if(win->fmt == hex32('c','l','i',0))foreground_cli(win);
	else if(win->fmt == hex32('t','u','i',0))foreground_tui(win);
	else if(win->fmt == hex32('h','t','m','l'))foreground_html(win);
	else if(win->fmt == hex32('v','b','o',0))foreground_vbo(win);
	else foreground_pixel(win);
}
