#include "actor.h"
void carvearrorkey2d(
	void*, u32,
	float, float, float,
	float, float, float,
	float, float, float,
	int, int, int, int, int, int);
static u8 button[16][3] = {
	{3, 1, '.'},
	{3, 5, '.'},
	{1, 3, '.'},
	{5, 3, '.'},
	{11, 3, '.'},
	{13, 1, '.'},
	{13, 5, '.'},
	{15, 3, '.'},
	{3, 7, 'n'},
	{3, 11, 'f'},
	{1, 9, 'l'},
	{5, 9, 'r'},
	{11, 9, 'x'},
	{13, 7, 'a'},
	{13, 11, 'y'},
	{15, 9, 'b'}
};




void vkbd_read_pixel(struct arena* win)
{
	int x,y,c,l,rgb;
	int w = win->width;
	int h = win->height;
	if(win->vkbd < 0)goto haha;

	c = ((win->vkbd)>>8)&0xff;
	if('k' == c)
	{
		drawsolid_rect(win, 0x202020, 0, h*3/4, w, h);
		for(y=0;y<8;y++)
		{
			for(x=0;x<16;x++)
			{
				l = 2;
				c = x+(y<<4);
				if(c == (win->vkbd&0xff))rgb = 0xffff00ff;
				else rgb = 0x20808080;

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
					(x*w/16)+1, h+1-((y+1)*h/32)+1,
					((x+1)*w/16)-1, h-1-(y*h/32),
					(u8*)&c, l
				);
			}
		}
	}
	else if('j' == c)
	{
		c = (win->vkbd)&0xff;
		x = ((c&0xf)-1)/2;
		y = (((c>>4)&0xf)-1)/2;
		if((1 == x)&&(0 == y))y = 0;	//up
		else if((1 == x)&&(2 == y))y = 1;	//down
		else if((0 == x)&&(1 == y))y = 2;	//left
		else if((2 == x)&&(1 == y))y = 3;	//right
		else if((4 == x)&&(1 == y))y = 4;
		else if((5 == x)&&(0 == y))y = 5;
		else if((5 == x)&&(2 == y))y = 6;
		else if((6 == x)&&(1 == y))y = 7;
		else y = 16;

		drawsolid_rect(win, 0x202020, 0, h*3/4, w, h);
		drawsolid_circle(win, 0x404040, w*1/4, h*7/8, h/8);
		drawsolid_circle(win, 0x404040, w*3/4, h*7/8, h/8);

		for(x=0;x<16;x++)
		{
			if(x == y)c = 0xff00ff;
			else c = 0xffffff;
			drawsolid_circle(
				win, c,
				(button[x][0])*w/16,
				(32-button[x][1])*h/32,
				(w+h)/64
			);
			drawascii(
				win, 0,
				(button[x][0])*w/16-4,
				(32-button[x][1])*h/32-8,
				button[x][2]
			);
		}
	}

haha:
	if(w<h)x = w>>5;
	else x = h>>5;

	drawsolid_circle(
		win, 0x0000ff,
		x, h-x, x/2
	);
	drawsolid_circle(
		win, 0xff0000,
		w-x, h-x, x/2
	);
}
void vkbd_read_vbo(struct arena* win)
{
	u8 ch[12];
	float j,k;
	int x,y,c,rgb;
	int w = win->width;
	int h = win->height;
	if(win->vkbd < 0)goto haha;

	carvesolid2d_rect(
		win, 0x202020,
		0.0, -0.75, 0.0,
		1.0, 0.0, 0.0,
		0.0, 0.25, 0.0
	);

	c = ((win->vkbd)>>8)&0xff;
	if('k' == c)
	{
		if(w<h)x = w/17;
		else x = h/17;
		j = (float)x / (float)w;
		k = (float)x / (float)h;

		for(y=0;y<8;y++)
		{
			for(x=0;x<16;x++)
			{
				c = x+(y<<4);
				if(c == (win->vkbd&0xff))rgb = 0xff00ff;
				else rgb = 0x808080;

				carvesolid2d_rect(
					win, rgb,
					(x-7.5)/8.0, (y-15.5)/16.0, -0.001,
					1.0/17, 0.0, 0.0,
					0.0, 0.5/17, 0.0
				);

				if((0!=c)&&(7!=c)&&(8!=c)&&(9!=c)&&(0xa!=c)&&(0xd!=c))
				{
					carve2d_ascii(
						win, 0xffffff,
						(x-7.5)/8.0, (y-15.5)/16.0, -0.002,
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
						(x-7.5)/8.0, (y-15.5)/16.0, -0.002,
						j, 0.0, 0.0,
						0.0, k/2, 0.0,
						'\\'
					);
					carve2d_ascii(
						win, 0xffffff,
						(x-7.5)/8.0+j, (y-15.5)/16.0, -0.002,
						j, 0.0, 0.0,
						0.0, k/2, 0.0,
						c
					);
				}
			}
		}
	}
	else if('j' == c)
	{
		y = h*3/16;
		j = (float)y / (float)w;
		k = (float)y / (float)h;

		ch[0] = 'l';
		ch[1] = 'r';
		ch[2] = 'n';
		ch[3] = 'f';
		ch[4] = '-';
		ch[5] = '+';
		ch[6] = 'x';
		ch[7] = 'b';
		ch[8] = 'a';
		ch[9] = 'y';
		ch[10] = '-';
		ch[11] = '+';
		for(x=0;x<8;x++)
		{
			if(ch[x] == (win->vkbd&0xff))ch[x] -= 0x20;
		}

		carvearrorkey2d(
			win, 0xff00ff,
			j-1.0, k-1.0, 0.0,
			j, 0.0, 0.0,
			0.0, k, 0.0,
			ch[0], ch[1], ch[2], ch[3], ch[4], ch[5]
		);
		carvearrorkey2d(
			win, 0xff00ff,
			1.0-j, k-1.0, 0.0,
			j, 0.0, 0.0,
			0.0, k, 0.0,
			ch[6], ch[7], ch[8], ch[9], ch[10], ch[11]
		);
/*
	else if('j' == c)
	{
		c = (win->vkbd)&0xff;
		x = ((c&0xf)-1)/2;
		y = (((c>>4)&0xf)-1)/2;
		for(c=0;c<16;c++)
		{
			if((x==button[c][0])&&(y==button[c][1]))break;
		}
		y = c;

		carvesolid2d_circle(
			win, 0x404040,
			-5.0/8, 9.0/16-1.0, -0.001,
			6.0/16, 0.0, 0.0,
			0.0, 3.0/16, 0.0
		);
		carvesolid2d_circle(
			win, 0x404040,
			5.0/8, 9.0/16-1.0, -0.001,
			6.0/16, 0.0, 0.0,
			0.0, 3.0/16, 0.0
		);
		carvesolid2d_circle(
			win, 0x404040,
			-5.0/8, 3.0/16-1.0, -0.001,
			6.0/16, 0.0, 0.0,
			0.0, 3.0/16, 0.0
		);
		carvesolid2d_circle(
			win, 0x404040,
			5.0/8, 3.0/16-1.0, -0.001,
			6.0/16, 0.0, 0.0,
			0.0, 3.0/16, 0.0
		);
		for(x=0;x<16;x++)
		{
			if(x == y)c = 0xff00ff;
			else c = 0xffffff;
			m = (float)(button[x][0]) - 8.0;
			n = (float)(button[x][1]) - 16.0;
			carvesolid2d_circle(
				win, c,
				m/8.0, n/16.0, -0.9,
				2.0/17, 0.0, 0.0,
				0.0, 1.0/17, 0.0
			);
			carve2d_ascii(
				win, 0x0000ff,
				(m+0.5)/8.0, n/16.0, -0.9,
				j*2, 0.0, 0.0,
				0.0, k, 0.0,
				button[x][2]
			);
		}
	}
*/
	}
	else
	{
		carvestring2d_center(
			win, 0xffffff,
			0.0, -0.75, -0.02,
			0.2, 0.0, 0.0,
			0.0, 0.1, 0.0,
			(void*)"fuckyou", 7
		);
	}

haha:
	if(w<h)x = w>>4;
	else x = h>>4;
	j = (float)x / (float)w;
	k = (float)x / (float)h;

	carvesolid2d_circle(
		win, 0x0000ff,
		1.0-j, k-1.0, -0.9,
		j/2, 0.0, 0.0,
		0.0, k/2, 0.0
	);
	carvesolid2d_circle(
		win, 0xff0000,
		j-1.0, k-1.0, -0.9,
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
	int x,y,w,h,ret;
	if(win->vkbd < 0)return 0;

	w = win->width;
	h = win->height;
	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	if(y < h*3/4)return 0;

	if(hex32('p','-',0,0) == ev->what)
	{
		ret = ((win->vkbd)>>8)&0xff;
		if('k' == ret)
		{
			x = x*16/w;
			y = 31 - (y*32/h);
			ret = x + (y*16);

			win->vkbd = ((int)'k'<<8) + ret;
			eventwrite(ret, _char_, (u64)win, 0);
		}
		else if('j' == ret)
		{
			ret = h/16;
			y = (h-y)/ret;
			if(x*2 < w)
			{
				x = x/ret;
				if((0==x)&&(1==y))ret = 'l';
				else if((2==x)&&(y==1))ret = 'r';
				else if((1==x)&&(y==0))ret = 'n';
				else if((1==x)&&(y==2))ret = 'f';
				else ret = 0;
			}
			else
			{
				x = w-x;
				x = x/ret;
				if((0==x)&&(1==y))ret = 'b';
				else if((2==x)&&(y==1))ret = 'x';
				else if((1==x)&&(y==0))ret = 'a';
				else if((1==x)&&(y==2))ret = 'y';
				else ret = 0;
			}

			win->vkbd = ((int)'j'<<8) + ret;
			if(0 != ret)
			{
				if('l' == ret)ret = 0x4b;
				else if('r' == ret)ret = 0x4d;
				else if('f' == ret)ret = 0x48;
				else if('n' == ret)ret = 0x50;
				eventwrite(ret, _kbd_, (u64)win, 0);
			}
		}
	}

byebye:
	return 1;
}