#include "actor.h"




void actoroutput_menu_cli(struct arena* win)
{
}
void actoroutput_menu_tui(struct arena* win)
{
}
void actoroutput_menu_html(struct arena* win)
{
}
void actoroutput_menu_json(struct arena* win)
{
}
void actoroutput_menu_vbo(struct arena* win)
{
	int j,c;
	int k = win->menutype&3;
	carvesolid2d_rect(
		win, 0x404040,
		0.0, 0.0, -0.2,
		0.5, 0.0, 0.0,
		0.0, 0.5, 0.0
	);
	for(j=0;j<4;j++)
	{
		if(j == k)c = 0xff00ff;
		else c = 0x808080;
		carvesolid2d_rect(
			win, c,
			0.0, (3-j*2)/8.0, -0.4,
			0.4, 0.0, 0.0,
			0.0, 0.1, 0.0
		);
		carve2d_decimal(
			win, 0xffffff,
			0.0, (3-j*2)/8.0, -0.6,
			0.4, 0.0, 0.0,
			0.0, 0.1, 0.0,
			j
		);
	}
}
void actoroutput_menu_pixel(struct arena* win)
{
	int j,c;
	int k = win->menutype&3;
	drawsolid_rect(
		win, 0x404040,
		win->width/4,   win->height/4,
		win->width*3/4, win->height*3/4
	);
	for(j=0;j<4;j++)
	{
		if(j == k)c = 0xff00ff;
		else c = 0x808080;
		drawsolid_rect(
			win, c,
			win->width*5/16,  win->height*(17+j*8)/64,
			win->width*11/16, win->height*(23+j*8)/64
		);
		drawdec_fit(
			win, 0xffffff,
			win->width*5/16,  win->height*(17+j*8)/64,
			win->width*11/16, win->height*(23+j*8)/64,
			j
		);
	}
}
void actoroutput_menu(struct arena* win)
{
	u64 fmt = win->fmt;
	if(_cli_ == fmt)actoroutput_menu_cli(win);
	else if(_tui_ == fmt)actoroutput_menu_tui(win);
	else if(_html_ == fmt)actoroutput_menu_html(win);
	else if(_json_ == fmt)actoroutput_menu_json(win);
	else if(_vbo_ == fmt)actoroutput_menu_vbo(win);
	else actoroutput_menu_pixel(win);
}