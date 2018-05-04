#include "actor.h"
static char* target[2] = {
	"all",
	"one"
};
static char* name[4] = {
	"0.overview",
	"1.adjust  ",
	"2.interact",
	"3.        "
};




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
	int j,k,c;
	carvesolid2d_rect(
		win, 0x404040,
		0.0, 0.0, -0.2,
		1.0, 0.0, 0.0,
		0.0, 0.5, 0.0
	);

	k = win->menutype & 1;
	for(j=0;j<2;j++)
	{
		if(j == k)c = 0xff00ff;
		else c = 0x808080;
		carvestring2d_center(
			win, c,
			(j*6-3)*0.25, 0.0, -0.6,
			0.125, 0.0, 0.0,
			0.0, 1.0/16, 0.0,
			(u8*)target[j], 0
		);
	}

	k = (win->menutype>>1)&0xff;
	for(j=0;j<4;j++)
	{
		if(j == k)c = 0xff00ff;
		else c = 0x808080;
		carvesolid2d_rect(
			win, c,
			0.0, (3-j*2)/8.0, -0.4,
			0.49, 0.0, 0.0,
			0.0, 0.1, 0.0
		);
		carvestring2d_center(
			win, 0xffffff,
			0.0, (3-j*2)/8.0, -0.6,
			1.0/16, 0.0, 0.0,
			0.0, 1.0/16, 0.0,
			(u8*)name[j], 0
		);
	}
}
void actoroutput_menu_pixel(struct arena* win)
{
	int x,y,j,k,c;
	drawsolid_rect(
		win, 0x404040,
		0,   win->height/4,
		win->width, win->height*3/4
	);

	k = win->menutype & 1;
	x = (win->width/8);
	y = (win->height/32);
	for(j=0;j<2;j++)
	{
		if(j == k)c = 0xff00ff;
		else c = 0x808080;
		drawstring_fit(
			win, c,
			(win->width)*(j*6+1)/8 - x,
			(win->height/2) - y,
			(win->width)*(j*6+1)/8 + x,
			(win->height/2) + y,
			(u8*)target[j], 0
		);
	}

	k = (win->menutype>>1)&0xff;
	x = (win->width/4)-2;
	y = (win->height/16)-2;
	for(j=0;j<4;j++)
	{
		if(j == k)c = 0xff00ff;
		else c = 0x808080;
		drawsolid_rect(
			win, c,
			(win->width/2) - x,
			(win->height*(5+j*2)/16) - y,
			(win->width/2) + x,
			(win->height*(5+j*2)/16) + y
		);
		drawstring_fit(
			win, 0xffffff,
			(win->width/2) - x,
			(win->height*(5+j*2)/16) - y,
			(win->width/2) + x,
			(win->height*(5+j*2)/16) + y,
			(u8*)name[j], 0
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
int actorinput_menu(struct arena* win, struct event* ev)
{
	int x, y, ret;
	ret = ev->what;

	if('p' == (ret&0xff))
	{
		x = (ev->why)&0xffff;
		y = ((ev->why)>>16)&0xffff;
		x = x*2/(win->width);
		y = y*8/(win->height);
		if((y >= 2)&&(y <= 5))
		{
			if(hex32('p','-',0,0) == ret)win->menutype = ((y-2)<<1)+x;
			else win->menutype = 0xffff0000 | ((y-2)<<1) + x;
		}
		return 0;
	}
	else if(_joy_ == ret)x = ((ev->why)>>32)&0xffff;
	else if(_kbd_ == ret)x = ev->why;
	else return 0;

	if((_df_ == x) | (0x48 == x))
	{
		y = win->menutype & 7;
		y = (y+7)%8;
		win->menutype = y | 0xffff0000;
	}
	else if((_dn_ == x) | (0x50 == x))
	{
		y = win->menutype & 7;
		y = (y+1)%8;
		win->menutype = y | 0xffff0000;
	}
	return 0;
}