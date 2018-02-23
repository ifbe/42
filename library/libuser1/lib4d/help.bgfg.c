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
	u64 fmt = win->fmt;
	if(_cli_ == fmt)background_cli(win);
	else if(_tui_ == fmt)background_tui(win);
	else if(_html_ == fmt)background_html(win);
	else if(_vbo_ == fmt)background_vbo(win);
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
	int w = win->w;
	int h = win->h;
	int t;
	if(w < h)t = h;
	else t = w;

	t >>= 5;
	drawsolid_circle(
		win, 0xabcdef,
		w-t, h-t, t/2
	);
	drawsolid_circle(
		win, 0xc08040,
		w-t, h-t, t/4
	);
}
void foreground(struct arena* win)
{
	u64 fmt = win->fmt;
	if(_cli_ == fmt)foreground_cli(win);
	else if(_tui_ == fmt)foreground_tui(win);
	else if(_html_ == fmt)foreground_html(win);
	else if(_vbo_ == fmt)foreground_vbo(win);
	else foreground_pixel(win);
}
