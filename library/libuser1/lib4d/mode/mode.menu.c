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
	vec4 va;
	vec4 vb;
	int j,k,x,y,c;

	carveline2d(win, 0xffffff, 0.0, -1.0, -0.2, 0.0, 1.0, -0.2);
	for(j=-3;j<4;j+=2)
	{
		va[0] = -0.5;
		va[1] = 0.25*j;
		vb[0] = 0.5;
		vb[1] = 0.25*j;
		carveline2d(win, 0xffffff, va[0], va[1], -0.2, vb[0], vb[1], -0.2);
	}

	k = win->menutype & 7;
	for(j=0;j<8;j++)
	{
		x = j%2;
		y = j/2;
		if(j == k)c = 0xff00ff;
		else c = 0x404040;
		carvesolid2d_rect(
			win, c,
			x-0.5, (3-2*y)*0.25, -0.4,
			0.4, 0.0, 0.0,
			0.0, 0.2, 0.0
		);
		carvestring2d_center(
			win, ~c,
			x-0.5, (13-8*y)/16.0, -0.6,
			1.0/8, 0.0, 0.0,
			0.0, 1.0/16, 0.0,
			(u8*)name[y], 0
		);
		carvestring2d_center(
			win, ~c,
			x-0.5, (11-8*y)/16.0, -0.6,
			1.0/8, 0.0, 0.0,
			0.0, 1.0/16, 0.0,
			(u8*)target[x], 0
		);
	}
}
void actoroutput_menu_pixel(struct arena* win)
{
	int va[2];
	int vb[2];
	int j,k,x,y,c;

	drawline(win, 0xffffff, win->width/2, 0, win->width/2, win->height);
	for(j=1;j<8;j+=2)
	{
		va[0] = win->width/4;
		va[1] = win->height*j/8;
		vb[0] = win->width*3/4;
		vb[1] = win->height*j/8;
		drawline(win, 0xffffff, va[0], va[1], vb[0], vb[1]);
	}

	k = win->menutype & 7;
	for(j=0;j<8;j++)
	{
		x = j%2;
		y = j/2;
		va[0] = (win->width)*(8*x+1)/16;
		va[1] = (win->height)*(8*y+1)/32;
		vb[0] = (win->width)*(8*x+7)/16;
		vb[1] = (win->height)*(8*y+7)/32;

		if(j == k)c = 0xff00ff;
		else c = 0x404040;
		drawsolid_rect(win, c, va[0], va[1], vb[0], vb[1]);

		drawstring_fit(win, ~c, va[0], va[1], vb[0], (va[1]+vb[1])/2, (u8*)name[y], 0);
		drawstring_fit(win, ~c, va[0], (va[1]+vb[1])/2, vb[0], vb[1], (u8*)target[x], 0);
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
	int x, y, ret, tmp;
	ret = ev->what;

	if('p' == (ret&0xff))
	{
		x = (ev->why)&0xffff;
		y = ((ev->why)>>16)&0xffff;
		x = x*2/(win->width);
		y = y*4/(win->height);

		tmp = (y<<1) | x;
		if(hex32('p','-',0,0) != ret)tmp |= 0xffff0000;

		win->menutype = tmp;
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