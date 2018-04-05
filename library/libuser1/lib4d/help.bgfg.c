#include "actor.h"
void vkbd_read(void*);
void carveaxis(void*);
void login_read(void*);
void carvedrone(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz);




void background_cli(struct arena* win)
{
}
void background_tui(struct arena* win)
{
}
void background_html(struct arena* win)
{
	win->len = 0;
}
void background_vbo(struct arena* win)
{
	int j;
	struct texandobj* mod = win->buf;
	for(j=0;j<4;j++)
	{
		mod[j].ilen = 0;
		mod[j].vlen = 0;
	}
	for(j=0x81;j<=0x85;j++)
	{
		mod[j].ilen = 0;
		mod[j].vlen = 0;
	}
	carvedrone(win, 0xffffff,
		0.0, 0.0, 0.0,
		0.0, 256.0, 0.0,
		0.0, 0.0, 1.0
	);
}
void background_pixel(struct arena* win)
{
	int x;
	int len = (win->stride)*(win->height);
	u32* buf = (u32*)(win->buf);

	for(x=0;x<len;x++)buf[x] = 0xff000000;
}
void background(struct arena* win)
{
	u64 fmt = win->fmt;
	//if((_vbo_ != win->fmt) | (12 == win->flag0));

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
	if(win->edit)carveaxis(win);
}
void foreground_pixel(struct arena* win)
{
	int j;
	vkbd_read(win);

	for(j=0;j<11;j++)
	{
		if(0 == win->touchdown[j].z)continue;

		drawline(win, 0xff00ff,
			win->touchdown[j].x, win->touchdown[j].y,
			win->touchmove[j].x, win->touchmove[j].y
		);
	}
}
void foreground(struct arena* win)
{
	u64 fmt = win->fmt;
	if((win->theone) | (0 == win->irel))login_read(win);

	if(_cli_ == fmt)foreground_cli(win);
	else if(_tui_ == fmt)foreground_tui(win);
	else if(_html_ == fmt)foreground_html(win);
	else if(_vbo_ == fmt)foreground_vbo(win);
	else foreground_pixel(win);
}
