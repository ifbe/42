#include "actor.h"
static char* name[8] = {
	"0.world overview",
	"1.element detail",
	"2.adjust posture",
	"3.edit existing ",
	"4.pass through  ",
	"5.              ",
	"6.              ",
	"7.              "
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
	int j,c;
	int k = win->menutype & 0xff;
	carvesolid2d_rect(
		win, 0x404040,
		0.0, 0.0, -0.2,
		0.5, 0.0, 0.0,
		0.0, 0.5, 0.0
	);
	for(j=0;j<8;j++)
	{
		if(j == k)c = 0xff00ff;
		else c = 0x808080;
		carvesolid2d_rect(
			win, c,
			0.0, (7-j*2)/16.0, -0.4,
			0.49, 0.0, 0.0,
			0.0, 0.049, 0.0
		);
		carvestring2d_center(
			win, 0xffffff,
			0.0, (7-j*2)/16.0, -0.6,
			1.0/16, 0.0, 0.0,
			0.0, 1.0/32, 0.0,
			(u8*)name[j], 0
		);
	}
}
void actoroutput_menu_pixel(struct arena* win)
{
	int x,y,j,c;
	int k = win->menutype & 0xff;
	drawsolid_rect(
		win, 0x404040,
		win->width/4,   win->height/4,
		win->width*3/4, win->height*3/4
	);

	x = (win->width/4)-2;
	y = (win->height/32)-2;
	for(j=0;j<8;j++)
	{
		if(j == k)c = 0xff00ff;
		else c = 0x808080;
		drawsolid_rect(
			win, c,
			(win->width/2) - x,
			(win->height*(9+j*2)/32) - y,
			(win->width/2) + x,
			(win->height*(9+j*2)/32) + y
		);
		drawstring_fit(
			win, 0xffffff,
			(win->width/2) - x,
			(win->height*(9+j*2)/32) - y,
			(win->width/2) + x,
			(win->height*(9+j*2)/32) + y,
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
		x = x*4/(win->width);
		y = y*16/(win->height);
		if((x >= 1)&&(x <= 2))
		{
			if((y >= 4)&&(y <= 11))
			{
				if(hex32('p','-',0,0) == ret)win->menutype = y-4;
				else win->menutype = (y-4) | 0xffff0000;
			}
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