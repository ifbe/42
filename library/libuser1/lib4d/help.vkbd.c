#include "actor.h"
static u8 button[8][3] = {
	{4, 6, 'f'},
	{4, 2, 'n'},
	{2, 4, 'l'},
	{6, 4, 'r'},
	{10, 4, 'x'},
	{12, 6, 'y'},
	{12, 2, 'a'},
	{14, 4, 'b'}
};




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

			//joystick area
			if((y>8)&&(y<15))
			{
				if((x>0)&&(x<7))continue;
				if((x>8)&&(x<15))continue;
			}

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

	c = win->vkbd;
	x = ((c&0xf)-1)/2;
	y = (((c>>4)&0xf)-1)/2;
	if((1 == x)&&(6 == y))y = 0;	//down
	else if((1 == x)&&(4 == y))y = 1;	//up
	else if((0 == x)&&(5 == y))y = 2;	//left
	else if((2 == x)&&(5 == y))y = 3;	//right
	else if((4 == x)&&(5 == y))y = 4;
	else if((5 == x)&&(6 == y))y = 5;
	else if((5 == x)&&(4 == y))y = 6;
	else if((6 == x)&&(5 == y))y = 7;
	else y = 10;

	for(x=0;x<8;x++)
	{
		if(x == y)c = 0xff00ff;
		else c = 0xffffff;
		drawsolid_circle(
			win, c,
			(button[x][0])*w/16,
			(button[x][1]+24)*h/32,
			(w+h)/64
		);
		drawascii(
			win, 0,
			(button[x][0])*w/16-4,
			(button[x][1]+24)*h/32-8,
			button[x][2]
		);
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
	float j,k,m,n;
	int x,y,c,rgb;
	int w = win->width;
	int h = win->height;
	if(0 == win->vkbd)goto haha;

	carvesolid2d_rect(
		win, 0,
		0.0, -0.5, 0.0,
		1.0, 0.0, 0.0,
		0.0, 0.5, 0.0
	);

	if(w<h)x = w/17;
	else x = h/17;
	j = (float)x / (float)w;
	k = (float)x / (float)h;
	for(y=0;y<16;y++)
	{
		for(x=0;x<16;x++)
		{
			c = x+(y<<4);
			if(c == win->vkbd)rgb = 0xff00ff;
			else rgb = 0x808080;

			//joystick area
			if((y>8)&&(y<15))
			{
				if((x>0)&&(x<7))continue;
				if((x>8)&&(x<15))continue;
			}

			carvesolid2d_rect(
				win, rgb,
				(x-7.5)/8.0, -(y+0.5)/16.0, -0.01,
				1.0/17, 0.0, 0.0,
				0.0, 0.5/17, 0.0
			);
			if(y >= 8)continue;

			if((0!=c)&&(7!=c)&&(8!=c)&&(9!=c)&&(0xa!=c)&&(0xd!=c))
			{
				carve2d_ascii(
					win, 0xffffff,
					(x-7.5)/8.0, -(y+0.5)/16.0, -0.02,
					j, 0.0, 0.0,
					0.0, k/2, 0.0,
					c
				);
			}
			else
			{
				if(0x0 == c)c = '0';
				else if(0x7 == c)c = 'a';
				else if(0x8 == c)c = 'b';
				else if(0x9 == c)c = 't';
				else if(0xa == c)c = 'n';
				else if(0xd == c)c = 'r';
				carve2d_ascii(
					win, 0xffffff,
					(x-7.5)/8.0, -(y+0.5)/16.0, -0.02,
					j, 0.0, 0.0,
					0.0, k/2, 0.0,
					'\\'
				);
				carve2d_ascii(
					win, 0xffffff,
					(x-7.5)/8.0+j, -(y+0.5)/16.0, -0.02,
					j, 0.0, 0.0,
					0.0, k/2, 0.0,
					c
				);
			}
		}
	}

	c = win->vkbd;
	x = ((c&0xf)-1)/2;
	y = (((c>>4)&0xf)-1)/2;
	if((1 == x)&&(4 == y))y = 0;	//up
	else if((1 == x)&&(6 == y))y = 1;	//down
	else if((0 == x)&&(5 == y))y = 2;	//left
	else if((2 == x)&&(5 == y))y = 3;	//right
	else if((4 == x)&&(5 == y))y = 4;
	else if((5 == x)&&(4 == y))y = 5;
	else if((5 == x)&&(6 == y))y = 6;
	else if((6 == x)&&(5 == y))y = 7;
	else y = 10;

	for(x=0;x<8;x++)
	{
		if(x == y)c = 0xff00ff;
		else c = 0xffffff;
		m = (float)(button[x][0]) - 8.0;
		n = (float)(button[x][1]) - 16.0;
		carvesolid2d_circle(
			win, c,
			m/8.0, n/16.0, -0.01,
			2.0/17, 0.0, 0.0,
			0.0, 1.0/17, 0.0
		);
		carve2d_ascii(
			win, 0x0000ff,
			(m+0.5)/8.0, n/16.0, -0.02,
			j*2, 0.0, 0.0,
			0.0, k, 0.0,
			button[x][2]
		);
	}

haha:
	if(w<h)x = w>>4;
	else x = h>>4;
	j = (float)x / (float)w;
	k = (float)x / (float)h;

	carvesolid2d_circle(
		win, 0xabcdef,
		1.0-j, k-1.0, -0.01,
		j, 0.0, 0.0,
		0.0, k, 0.0
	);
	carvesolid2d_circle(
		win, 0xc08040,
		1.0-j, k-1.0, -0.02,
		j/2, 0.0, 0.0,
		0.0, k/2, 0.0
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
	u64 why,what;
	int x,y,ret;
	int w = win->width;
	int h = win->height;

	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	if(y*2 < h)return 0;

	if(hex32('p','@',0,0) == ev->what)
	{
		if(0 == win->vkbd)return 0;
		x = x*16/w;
		y = y*32/h;
		win->vkbd = x + (y*16) - 256;
	}
	if(hex32('p','-',0,0) == ev->what)
	{
		if(x<y)ret = x>>4;
		else ret = y>>4;

		if((y+ret > h) && (x+ret > w))
		{
			if(win->vkbd)win->vkbd = 0;
			else win->vkbd = 1;
			return 1;
		}

		x = x*16/w;
		y = y*32/h;
		ret = x + (y*16) - 256;

		if(ret < 0x80)
		{
			why = ret;
			what = _char_;
		}
		else
		{
			what = _kbd_;
			if(ret >= 0xf0)why = ret;
			else
			{
				x = ((ret&0xf)-1)/2;
				y = (((ret>>4)&0xf)-1)/2;

				if((1 == x)&&(4 == y))why = 0x48;	//up
				if((0 == x)&&(5 == y))why = 0x4b;	//left
				if((2 == x)&&(5 == y))why = 0x4d;	//right
				if((1 == x)&&(6 == y))why = 0x50;	//down
			}
		}

		eventwrite(why, what, (u64)win, 0);
		return 1;
	}
	return 1;
}