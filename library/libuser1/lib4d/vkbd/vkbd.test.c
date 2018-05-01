#include "actor.h"
void carvearrorkey2d(
	void*, u32,
	float, float, float,
	float, float, float,
	float, float, float,
	u8*, int
);
void drawarrorkey2d(
	struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1,
	u8* buf, int t
);
static u16 joyl[8]={_dl_, _dr_, _dn_, _df_, _lt_, _lb_, _ls_, _ll_};
static u16 joyr[8]={_kx_, _kb_, _ka_, _ky_, _rt_, _rb_, _rs_, _rr_};




void vkbd_read_pixel(struct arena* win)
{
	u8 ch[8];
	int x,y,c,l,rgb;
	int w = win->width;
	int h = win->height;
	if(win->vkbdtype < 0)goto haha;

	//bg
	drawsolid_rect(win, 0x202020, 0, h*3/4, w, h);

	c = ((win->vkbdtype)>>16)&0xff;
	if('k' == c)
	{
		for(y=0;y<8;y++)
		{
			for(x=0;x<16;x++)
			{
				l = 2;
				c = x+(y<<4);
				if(c == (win->vkbdtype&0xffff))rgb = 0xffff00ff;
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
		ch[0] = 'l';
		ch[1] = 'r';
		ch[2] = 'n';
		ch[3] = 'f';
		ch[4] = 't';
		ch[5] = 'b';
		ch[6] = 's';
		ch[7] = '-';

		y = (win->vkbdtype)&0xffff;
		for(x=0;x<8;x++){if(joyl[x] == y)ch[x] |= 0x80;}
		drawarrorkey2d(win, 0xff00ff, 0, h*13/16, h*3/16, h, ch, 1);

		ch[0] = 'x';
		ch[1] = 'b';
		ch[2] = 'a';
		ch[3] = 'y';
		ch[4] = 't';
		ch[5] = 'b';
		ch[6] = 's';
		ch[7] = '+';

		y = (win->vkbdtype)&0xffff;
		for(x=0;x<8;x++){if(joyr[x] == y)ch[x] |= 0x80;}
		drawarrorkey2d(win, 0xff00ff, w-h*3/16, h*13/16, w, h, ch, -1);
	}
	else
	{
		drawstring_fit(win, 0xffffff, w*1/4, h*13/16, w*3/4, h*15/16, (void*)"helloworld", 10);
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
	u8 ch[8];
	float j,k;
	int x,y,c,rgb;
	int w = win->width;
	int h = win->height;
	if(win->vkbdtype < 0)goto haha;

	carvesolid2d_rect(
		win, 0x202020,
		0.0, -0.75, 0.0,
		1.0, 0.0, 0.0,
		0.0, 0.25, 0.0
	);

	c = ((win->vkbdtype)>>16)&0xff;
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
				if(c == (win->vkbdtype&0xffff))rgb = 0xff00ff;
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
		ch[4] = 't';
		ch[5] = 'b';
		ch[6] = 's';
		ch[7] = '-';

		y = (win->vkbdtype)&0xffff;
		for(x=0;x<8;x++){if(joyl[x] == y)ch[x] |= 0x80;}
		carvearrorkey2d(
			win, 0xff00ff,
			j-1.0, k-1.0, 0.0,
			j, 0.0, 0.0,
			0.0, k, 0.0,
			ch, 1
		);

		ch[0] = 'x';
		ch[1] = 'b';
		ch[2] = 'a';
		ch[3] = 'y';
		ch[4] = 't';
		ch[5] = 'b';
		ch[6] = 's';
		ch[7] = '+';

		y = (win->vkbdtype)&0xffff;
		for(x=0;x<8;x++){if(joyr[x] == y)ch[x] |= 0x80;}
		carvearrorkey2d(
			win, 0xff00ff,
			1.0-j, k-1.0, 0.0,
			j, 0.0, 0.0,
			0.0, k, 0.0,
			ch, -1
		);
	}
	else
	{
		carvestring2d_center(
			win, 0xffffff,
			0.0, -0.75, -0.02,
			0.2, 0.0, 0.0,
			0.0, 0.1, 0.0,
			(void*)"helloworld", 10
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
	short tmp[4];
	int x,y,w,h,ret;
	if(win->vkbdtype < 0)return 0;

	w = win->width;
	h = win->height;
	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	if(y < h*3/4)return 0;

	if(hex32('p','-',0,0) == ev->what)
	{
		ret = ((win->vkbdtype)>>16)&0xff;
		if('k' == ret)
		{
			x = x*16/w;
			y = 31 - (y*32/h);
			ret = x + (y*16);

			win->vkbdtype = ((int)'k'<<16) + ret;
			eventwrite(ret, _char_, (u64)win, 0);
		}
		else if('j' == ret)
		{
			y = (h-y)*16/h;
			if(x*2 < w)
			{
				x = x*16/h;
				if((0==x)&&(1==y))ret = _dl_;
				else if((2==x)&&(1==y))ret = _dr_;
				else if((1==x)&&(0==y))ret = _dn_;
				else if((1==x)&&(2==y))ret = _df_;
				else if((0==x)&&(3==y))ret = _lt_;
				else if((2==x)&&(3==y))ret = _lb_;
				else if((1==x)&&(1==y))ret = _ls_;
				else if((3==x)&&(2==y))ret = _ll_;
				else ret = 0;
			}
			else
			{
				x = w-x;
				x = x*16/h;
				if((0==x)&&(1==y))ret = _kb_;
				else if((2==x)&&(y==1))ret = _kx_;
				else if((1==x)&&(y==0))ret = _ka_;
				else if((1==x)&&(y==2))ret = _ky_;
				else if((2==x)&&(y==3))ret = _rt_;
				else if((0==x)&&(y==3))ret = _rb_;
				else if((1==x)&&(y==1))ret = _rs_;
				else if((3==x)&&(y==2))ret = _rr_;
				else ret = 0;
			}
			win->vkbdtype = ((int)'j'<<16) + ret;

			if(0 != ret)
			{
				tmp[0] = 0;
				tmp[1] = 0;
				tmp[2] = ret;
				tmp[3] = 0;
				eventwrite(*(u64*)tmp, _joy_, (u64)win, 0);
			}
		}
	}

byebye:
	return 1;
}
int actorinput_vkbd(struct arena* win, struct event* ev)
{
	int x,y,ret;
	int why = ev->why;
	int what = ev->what;
	if('p' == (what&0xff))
	{
		if(0x2d70 == what)
		{
			x = win->width;
			y = win->height;
			if(x<y)ret = x>>4;
			else ret = y>>4;

			//open or close vkbd
			x = why&0xffff;
			y = (why>>16)&0xffff;
			if(y+ret > win->height)
			{
				if(x+ret > win->width)
				{
					if(win->vkbdtype < 0)win->vkbdtype = (int)'j'<<16;
					else win->vkbdtype = -1;
					return 1;
				}
				else if(x < ret)
				{
					if(win->vkbdtype < 0)win->vkbdtype = (int)'k'<<16;
					else win->vkbdtype = -1;
					return 1;
				}
			}
		}

		//call vkbd
		if(win->vkbdtype >= 0)
		{
			ret = vkbd_write(win, ev);
			if(0 != ret)return 1;
		}
	}
	return 0;
}