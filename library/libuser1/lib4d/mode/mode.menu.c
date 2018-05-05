#include "actor.h"
static char* target[2] = {
	"all",
	"one"
};
static char* name[4] = {
	"0.overview",
	"1.adjust  ",
	"2.interact",
	"3.????????"
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
	int j,k,m,n;
	m = win->menutype & 7;
	n = win->menutype & 1;

	if(0 == n)k = 0xffff00;
	else k = 0x404040;
	carvesolid2d_rect(
		win, k,
		-7.0/16, 0.0, -0.4,
		1.0/16, 0.0, 0.0,
		0.0, 0.5, 0.0
	);

	if(1 == n)k = 0x00ffff;
	else k = 0x404040;
	carvesolid2d_rect(
		win, k,
		7.0/16, 0.0, -0.4,
		1.0/16, 0.0, 0.0,
		0.0, 0.5, 0.0
	);

	for(j=0;j<8;j++)
	{
		if(j == m)
		{
			if(0 == (j&1))k = 0xffff00;
			else k = 0x00ffff;
		}
		else k = 0x404040;

		carvesolid2d_rect(
			win, k,
			((j&1)*2-1)/24.0, (7-j*2)/16.0, -0.4,
			1.0/3, 0.0, 0.0,
			0.0, 1.0/17, 0.0
		);
		carvestring2d_center(
			win, ~k,
			0.0, (7-j*2)/16.0, -0.6,
			1.0/17, 0.0, 0.0,
			0.0, 1.0/33, 0.0,
			(u8*)name[j/2], 0
		);
	}
}
void actoroutput_menu_pixel(struct arena* win)
{
	int j,k,m,n,x,y;
	m = win->menutype & 7;
	n = win->menutype & 1;

	if(0 == n)k = 0xffff00;
	else k = 0x404040;
	drawsolid_rect(
		win, k,
		win->width*3/16, win->height*4/16,
		win->width*4/16, win->height*11/16
	);

	if(1 == n)k = 0x00ffff;
	else k = 0x404040;
	drawsolid_rect(
		win, k,
		win->width*12/16, win->height*5/16,
		win->width*13/16, win->height*12/16
	);

	x = (win->width)/4;
	y = (win->height)/32-2;
	for(j=0;j<8;j++)
	{
		if(j == m)
		{
			if(0 == (j&1))k = 0xffff00;
			else k = 0x00ffff;
		}
		else k = 0x404040;
		drawsolid_rect(
			win, k,
			(win->width)*(8+(j&1))/16 - x,
			(win->height*(9+j*2)/32) - y,
			(win->width)*(7+(j&1))/16 + x,
			(win->height*(9+j*2)/32) + y
		);
		drawstring_fit(
			win, ~k,
			(win->width/2) - x/2,
			(win->height*(9+j*2)/32) - y,
			(win->width/2) + x/2,
			(win->height*(9+j*2)/32) + y,
			(u8*)name[j/2], 0
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
		if((x >= 1)&&(x < 3)&&(y >= 4)&&(y < 12))
		{
			if(hex32('p','-',0,0) == ret)win->menutype = (y-4);
			else win->menutype = 0xffff0000 | (y-4);
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