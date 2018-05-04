#include "actor.h"
int term_write(void*);
//vkbd
int vkbd_read(void*);
int actorinput_vkbd(struct arena* win, struct event* ev);
//menu
int actoroutput_menu(struct arena*);
int actorinput_menu(struct arena* win, struct event* ev);
//mode0
int actoroutput_overview(struct arena*);
int actorinput_overview(struct arena* win, struct event* ev);
//mode1
int actoroutput_detail(struct arena*);
//mode2
int actoroutput_posture(struct arena*);
int camera_event(struct arena* win, struct event* ev);
//mode3
int actoroutput_edit(struct arena*);
int playwith2d(struct arena* win, struct event* ev);
int playwith3d(struct arena* win, struct event* ev);
//mode4
int actoroutput_deliver(struct arena* win);
int actorinput_deliver(struct arena* win, struct event* ev);
//mode5
int actoroutput_oneonone(struct arena* win);
int actorinput_oneonone(struct arena* win, struct event* ev);




static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct compo* compo = 0;
void helpin_create(void* addr)
{
	arena = addr + 0x000000;
	actor = addr + 0x100000;
	style = addr + 0x000000;
	compo = addr + 0x100000;
}
void helpout_create(void* addr)
{
	arena = addr + 0x000000;
	actor = addr + 0x100000;
	style = addr + 0x200000;
	compo = addr + 0x300000;
}




int actorinput_special(struct arena* win, struct event* ev)
{
	int h16, l16, ret, val;

	val = 0;
	if(_kbd_ == ev->what)
	{
		if(ev->why == 0xfb)val = 'l';
		else if(ev->why == 0xfc)val = 'r';
	}
	else if(_joy_ == ev->what)
	{
		ret = ((ev->why)>>32)&0xffff;
		if(_ll_ == ret)val = 'l';
		else if(_rr_ == ret)val = 'r';
	}

	if('l' == val)
	{
		ret = win->menutype;
		h16 = ret >> 16;
		l16 = ret & 0xffff;
		if(0 != h16)
		{
			l16 = (l16+1)%8;
			win->menutype = (h16 << 16) | l16;
			return 1;
		}
	}
	else if('r' == val)
	{
		ret = win->menutype;
		h16 = ret >> 16;
		if(0 != h16)win->menutype &= 0xffff;
		else win->menutype |= 0xffff0000;
		return 1;
	}
	return 0;
}
void actorinput_touch(struct arena* win, struct event* ev)
{
	int x,y,z,btn;
	if('p' != (ev->what & 0xff))return;

	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	z = ((ev->why)>>32)&0xffff;
	btn = ((ev->why)>>48)&0xffff;

	if('l' == btn)btn = 10;
	else if('r' == btn)btn = 11;
	else if(10 < btn)return;

	if(hex32('p','@',0,0) == ev->what)
	{
		win->touchmove[btn].x = x;
		win->touchmove[btn].y = y;
	}
	else if(hex32('p','+',0,0) == ev->what)
	{
		win->touchdown[btn].x = win->touchmove[btn].x = x;
		win->touchdown[btn].y = win->touchmove[btn].y = y;
		win->touchdown[btn].z = 1;
	}
	else if(hex32('p','-',0,0) == ev->what)
	{
		win->touchdown[btn].z = 0;
	}
}
void actorinput(struct arena* win, struct event* ev)
{
	int h16,l16,ret;

	//check vkbd
	ret = actorinput_vkbd(win, ev);
	if(0 != ret)goto theend;

	//check menu
	ret = actorinput_special(win, ev);
	if(0 != ret)goto theend;

	//
	ret = win->menutype;
	h16 = ret >> 16;
	l16 = ret & 0xffff;
	if(0 != h16)
	{
		actorinput_menu(win, ev);
	}
	else if(0 == l16)
	{
		actorinput_overview(win, ev);
	}
	else if(1 == l16)
	{
	}
	else if(2 == l16)
	{
		if(_vbo_ == win->fmt)playwith3d(win, ev);
		else playwith2d(win, ev);
	}
	else if(3 == l16)
	{
		camera_event(win, ev);
	}
	else if(4 == l16)
	{
		actorinput_deliver(win, ev);
	}
	else if(5 == l16)
	{
		actorinput_oneonone(win, ev);
	}

theend:
	if('p' == (ev->what&0xff))actorinput_touch(win, ev);
	win->enq += 1;
}




void background_pixel(struct arena* win)
{
	int x;
	int w = win->width;
	int h = win->height;
	int s = win->stride;
	int len = s*h;
	u32* buf = (u32*)(win->buf);

	for(x=0;x<len;x++)buf[x] = 0xff000000;
/*
	if((win->theone) | (win->edit) | (0 == win->irel))
	{
		drawline(win, 0xffffff, 0, h/2, w, h/2);
		drawline(win, 0xffffff, w/2, 0, w/2, h);
	}
*/
}
void background_vbo(struct arena* win)
{
	int j;
	struct texandobj* mod = win->mod;
	for(j=0;j<8;j++)
	{
		mod[j].ilen = 0;
		mod[j].vlen = 0;
	}
	for(j=0x80;j<0x88;j++)
	{
		mod[j].ilen = 0;
		mod[j].vlen = 0;
	}
/*
	if((win->edit) | (0 <= win->theone) | (0 == win->irel))
	{
		carveline(win, 0xff0000, 0.0, 0.0, 0.0, 10000.0, 0.0, 0.0);
		carveline(win, 0x00ff00, 0.0, 0.0, 0.0, 0.0, 10000.0, 0.0);
		carveline(win, 0x0000ff, 0.0, 0.0, 0.0, 0.0, 0.0, 10000.0);
	}
*/
}
void background_json(struct arena* win)
{
	win->len = mysnprintf(win->buf, 0x100000, "{\n");
}
void background_html(struct arena* win)
{
	struct htmlpiece* hp = win->hp;

	hp[1].len = mysnprintf(
		hp[1].buf, 0x100000,
		"<html><head>\n"
		"<meta charset=\"UTF-8\">\n"
		"<style type=\"text/css\">\n"
		"*{margin:0;padding:0;}\n"
	);
	hp[2].len = mysnprintf(
		hp[2].buf, 0x100000,
		"<body style=\"width:100%%;height:100%%;\">\n"
	);
}
void background_tui(struct arena* win)
{
}
void background_cli(struct arena* win)
{
}
void background(struct arena* win)
{
	u64 fmt = win->fmt;
	if(_cli_ == fmt)background_cli(win);
	else if(_tui_ == fmt)background_tui(win);
	else if(_html_ == fmt)background_html(win);
	else if(_json_ == fmt)background_json(win);
	else if(_vbo_ == fmt)background_vbo(win);
	else background_pixel(win);
}




void foreground_pixel(struct arena* win)
{
	int j;
	for(j=0;j<11;j++)
	{
		if(0 == win->touchdown[j].z)continue;

		drawline(win, 0xff00ff,
			win->touchdown[j].x, win->touchdown[j].y,
			win->touchmove[j].x, win->touchmove[j].y
		);
	}
}
void foreground_vbo(struct arena* win)
{
	int j;
	float x0,y0,z0;
	float x1,y1,z1;

	for(j=0;j<11;j++)
	{
		if(0 == win->touchdown[j].z)continue;

		x0 = (float)(win->touchdown[j].x) / (float)(win->width);
		x0 = x0*2 - 1.0;
		y0 = (float)(win->touchdown[j].y) / (float)(win->height);
		y0 = 1.0 - y0*2;
		x1 = (float)(win->touchmove[j].x) / (float)(win->width);
		x1 = x1*2 - 1.0;
		y1 = (float)(win->touchmove[j].y) / (float)(win->height);
		y1 = 1.0 - y1*2;
		carveline2d(win, 0xff00ff, x0, y0, -0.99, x1, y1, -0.99);
	}

	if(1)
	{
		x0 = win->target.cx;
		y0 = win->target.cy;
		z0 = win->target.cz;
		carvepoint(win, 0xff0000, x0, y0, z0);

		j = (win->width + win->height) / 128;
		x0 = (float)j / (float)(win->width);
		y0 = (float)j / (float)(win->height);
		carveline2d(win, 0xffffff, -x0, 0.0, -0.99, x0, 0.0, -0.99);
		carveline2d(win, 0xffffff, 0.0, -y0, -0.99, 0.0, y0, -0.99);
	}
}
void foreground_json(struct arena* win)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(buf+len, 0x100000-len, "}\n");

	win->len = len;
}
void foreground_html(struct arena* win)
{
	int len;
	u8* buf;
	struct htmlpiece* hp = win->hp;

	len = hp[1].len;
	buf = hp[1].buf + len;
	hp[1].len += mysnprintf(buf, 0x100000-len, "</style></head>\n");

	len = hp[2].len;
	buf = hp[2].buf + len;
	hp[2].len += mysnprintf(buf, 0x100000-len, "</body></html>\n");
}
void foreground_tui(struct arena* win)
{
}
void foreground_cli(struct arena* win)
{
}
void foreground(struct arena* win)
{
	u64 fmt = win->fmt;
	if(_cli_ == fmt)foreground_cli(win);
	else if(_tui_ == fmt)foreground_tui(win);
	else if(_html_ == fmt)foreground_html(win);
	else if(_json_ == fmt)foreground_json(win);
	else if(_vbo_ == fmt)foreground_vbo(win);
	else foreground_pixel(win);
}




int actoroutput(struct arena* win)
{
/*
	//cli silent
	if(win->fmt == _cli_)
	{
		if(win->edit)return 0;
	}
*/
	int h16, l16, ret;

	//bg
	background(win);

	//context
	ret = win->menutype;
	l16 = ret&0xffff;
	h16 = (ret>>16)&0xffff;
	if(0 != h16)actoroutput_menu(win);
	else if(0 == l16)actoroutput_overview(win);
	else if(1 == l16)actoroutput_detail(win);
	else if(2 == l16)actoroutput_edit(win);
	else if(3 == l16)actoroutput_posture(win);
	else if(4 == l16)actoroutput_deliver(win);
	else if(5 == l16)actoroutput_oneonone(win);

	//vkbd
	vkbd_read(win);

	//fg
	foreground(win);
	return 0;
}