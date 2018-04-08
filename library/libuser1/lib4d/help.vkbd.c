#include "actor.h"




void vkbd_read_pixel(struct arena* win)
{
	int x,y,c,l,rgb;
	int w = win->width;
	int h = win->height;
	if(0 == win->vkbd)goto haha;

	for(y=0;y<16;y++)
	{
		for(x=0;x<16;x++)
		{
			l = 2;
			c = x+(y<<4);
			if(c == win->vkbd)rgb = 0xffff00ff;
			else rgb = 0x40808080;

			if(0x0 == c)c = hex32('\\','0',0,0);
			else if(0x7 == c)c = hex32('\\','a',0,0);
			else if(0x8 == c)c = hex32('\\','b',0,0);
			else if(0x9 == c)c = hex32('\\','t',0,0);
			else if(0xa == c)c = hex32('\\','n',0,0);
			else if(0xd == c)c = hex32('\\','r',0,0);
			else if(0xf0 <= c)
			{
				if(0xfa <= c)
				{
					l = 3;
					c = ((c-0xfa)<<16) + hex32('f','1','0',0);
				}
				else
				{
					l = 2;
					c = ((c-0xf0)<<8) + hex32('f','0',0,0);
				}
			}
			else if(0x80 <= c)
			{
				l = 1;
				c = ' ';
			}
			else l = 1;

			drawicon_1(
				win, rgb,
				(x*w/16)+1, (h/2)+(y*h/32)+1,
				((x+1)*w/16)-1, (h/2)+((y+1)*h/32)-1,
				(u8*)&c, l
			);
		}
	}

haha:
	if(w<h)x = w>>5;
	else x = h>>5;

	drawsolid_circle(
		win, 0xabcdef,
		w-x, h-x, x
	);
	drawsolid_circle(
		win, 0xc08040,
		w-x, h-x, x/2
	);

}
void vkbd_read_vbo(struct arena* win)
{
	float j,k;
	int x,y,c,rgb;
	int w = win->width;
	int h = win->height;
	if(0 == win->vkbd)goto haha;

	for(y=0;y<16;y++)
	{
		for(x=0;x<16;x++)
		{
			c = x+(y<<4);
			if(c == win->vkbd)rgb = 0xff00ff;
			else rgb = 0x808080;

			carvesolid2d_rect(
				win, rgb,
				(x-7.5)/8.0, -(y+0.5)/16.0,
				1.0/17, 0.0, 0.0, 0.5/17
			);
		}
	}

haha:
	if(w<h)x = w>>4;
	else x = h>>4;
	j = (float)x / (float)w;
	k = (float)x / (float)h;

	carvesolid2d_circle(
		win, 0xabcdef,
		1.0-j, k-1.0,
		j, 0.0,
		0.0, k
	);
}
void vkbd_read_html(struct arena* win)
{
}
void vkbd_read_tui(struct arena* win)
{
}
void vkbd_read_cli(struct arena* win)
{
}
void vkbd_read(struct arena* win)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)vkbd_read_cli(win);
	else if(fmt == _tui_)vkbd_read_tui(win);
	else if(fmt == _html_)vkbd_read_html(win);
	else if(fmt == _vbo_)vkbd_read_vbo(win);
	else vkbd_read_pixel(win);
}




int vkbd_write(struct arena* win, struct event* ev)
{
	int x,y,ret;
	int w = win->width;
	int h = win->height;

	if(hex32('p','@',0,0) == ev->what)
	{
		x = (ev->why)&0xffff;
		y = ((ev->why)>>16)&0xffff;
		if((win->vkbd) && (y*2 > h))
		{
			x = x*16/w;
			y = y*32/h;
			if(y < 16)return 0;

			win->vkbd = x + (y*16) - 256;
		}
	}
	if(hex32('p','-',0,0) == ev->what)
	{
		x = (ev->why)&0xffff;
		y = ((ev->why)>>16)&0xffff;
		if(x<y)ret = x>>4;
		else ret = y>>4;
say("w=%x,h=%x,x=%x,y=%x,ret=%x\n",w,h,x,y,ret);
		if((y+ret > h) && (x+ret > w))
		{
			if(win->vkbd)win->vkbd = 0;
			else win->vkbd = 1;
			return 0;
		}
		else if((win->vkbd) && (y*2 > h))
		{
			x = x*16/w;
			y = y*32/h;
			if(y < 16)return 0;

			ret = x + (y*16) - 256;
			if(ret < 0x80)eventwrite(ret, _char_, (u64)win, 0);
			else eventwrite(ret, _kbd_, (u64)win, 0);
			return 1;
		}
	}
	return 0;
}