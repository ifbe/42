#include "libuser.h"
void drawarrorkey2d(void*, u32, int x0, int y0, int x1, int y1, u8*, int);
void carvearrorkey2d(void*, u32, vec3 vc, vec3 vr, vec3 vf, u8*, int);




void vkbd_read_pixel(struct arena* win, struct style* sty)
{
	u8 ch[8];
	int c,l,rgb;
	int x,y,m,n;
	int w = win->width;
	int h = win->height;
	if(win->vkbdtype < 0)goto haha;

	c = ((win->vkbdtype)>>16)&0xff;
	if('k' == c)
	{
		//bg
		drawsolid_rect(win, 0x202020, 0, h*3/4, w, h);

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
		//bg
		drawsolid_rect(win, 0x202020, 0, h*3/4, w, h);

		ch[0] = 'l';
		ch[1] = 'r';
		ch[2] = 'n';
		ch[3] = 'f';
		ch[4] = 't';
		ch[5] = 'b';
		ch[6] = 's';
		ch[7] = '-';
		drawarrorkey2d(win, 0xff00ff, 0, h*13/16, h*3/16, h, ch, 1);

		ch[0] = 'x';
		ch[1] = 'b';
		ch[2] = 'a';
		ch[3] = 'y';
		ch[4] = 't';
		ch[5] = 'b';
		ch[6] = 's';
		ch[7] = '+';
		drawarrorkey2d(win, 0xff00ff, w-h*3/16, h*13/16, w, h, ch, -1);
	}
/*
	else
	{
		drawstring_fit(win, 0xffffff, w*1/4, h*13/16, w*3/4, h*15/16, (void*)"helloworld", 10);
	}
*/
haha:
	if(w<h)c = w>>5;
	else c = h>>5;

	drawsolid_circle(
		win, 0x0000ff,
		c, h-c, c
	);
	drawsolid_circle(
		win, 0xff0000,
		w-c, h-c, c
	);

	c *= 2;
	if(win->input[0].z0)
	{
		m = win->input[0].x0;
		n = win->input[0].y0;
		x = win->input[0].x1;
		y = win->input[0].y1;
	}
	else if(win->input[10].z0)
	{
		m = win->input[10].x0;
		n = win->input[10].y0;
		x = win->input[10].x1;
		y = win->input[10].y1;
	}
	else return;

	if(n+c > h)
	{
		if(m+c > w)
		{
			m = ((h-y)*(h-y)/(x-w) + (w+x)) / 2;
			n = ((x-w)*(x-w)/(y-h) + (y+h)) / 2;
			drawsolid_triangle(win, 0x808080, m, h, w, n, x, y);
			drawsolid_triangle(win, 0x000000, m, h, w, n, w, h);
		}
		else if(m < c)
		{
			drawsolid_rect(win, 0x404040, 0, 0, x, h);
			drawsolid_rect(win, 0x404040, 0, y, w, h);
		}
	}
}
void vkbd_read_vbo(struct arena* win, struct style* sty)
{
	u8 ch[8];
	float j,k;
	vec3 vc;
	vec3 vr;
	vec3 vf;
	int x,y,c,rgb;
	int w = win->width;
	int h = win->height;
	if(win->vkbdtype < 0)goto haha;

	c = ((win->vkbdtype)>>16)&0xff;
	if(('j' == c)|('k' == c))
	{
		vc[0] = 0.0;
		vc[1] = -0.75;
		vc[2] = -0.5;
		vr[0] = 1.0;
		vr[1] = 0.0;
		vr[2] = 0.0;
		vf[0] = 0.0;
		vf[1] = 0.25;
		vf[2] = 0.0;
		carvesolid2d_rect(win, 0x202020, vc, vr, vf);
	}

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

				vc[0] = (x-7.5)/8.0;
				vc[1] = (y-15.5)/16.0;
				vc[2] = -0.9;
				vr[0] = 1.0/17;
				vr[1] = 0.0;
				vr[2] = 0.0;
				vf[0] = 0.0;
				vf[1] = 0.5/17;
				vf[2] = 0.0;
				carvesolid2d_rect(win, rgb, vc, vr, vf);

				vc[2] = -0.91;
				vr[0] = j;
				vf[1] = k/2;
				if((0==c)|(7==c)|(8==c)|(9==c)|(0xa==c)|(0xd==c))
				{
					if(0x0 == c)c = '0';
					else if(0x7 == c)c = 'a';
					else if(0x8 == c)c = 'b';
					else if(0x9 == c)c = 't';
					else if(0xa == c)c = 'n';
					else if(0xd == c)c = 'r';
					vc[0] = (x-7.5)/8.0+j;
					vc[1] = (y-15.5)/16.0;
					carve2d_ascii(win, 0xffffff, vc, vr, vf, c);
					c = '\\';
				}

				vc[0] = (x-7.5)/8.0;
				vc[1] = (y-15.5)/16.0;
				carve2d_ascii(win, 0xffffff, vc, vr, vf, c);
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

		vc[0] = j-1.0;
		vc[1] = k-1.0;
		vc[2] = -0.7;
		vr[0] = j;
		vr[1] = 0.0;
		vr[2] = 0.0;
		vf[0] = 0.0;
		vf[1] = k;
		vf[2] = 0.0;
		carvearrorkey2d(win, 0xff00ff, vc, vr, vf, ch, 1);

		ch[0] = 'x';
		ch[1] = 'b';
		ch[2] = 'a';
		ch[3] = 'y';
		ch[4] = 't';
		ch[5] = 'b';
		ch[6] = 's';
		ch[7] = '+';

		vc[0] = 1.0-j;
		vc[1] = k-1.0;
		vc[2] = -0.7;
		vr[0] = j;
		vr[1] = 0.0;
		vr[2] = 0.0;
		vf[0] = 0.0;
		vf[1] = k;
		vf[2] = 0.0;
		carvearrorkey2d(win, 0xff00ff, vc, vr, vf, ch, -1);
	}
/*
	else
	{
		carvestring2d_center(
			win, 0xffffff,
			0.0, -0.75, -0.5,
			0.2, 0.0, 0.0,
			0.0, 0.1, 0.0,
			(void*)"helloworld", 10
		);
	}
*/
haha:
	if(w<h)c = w>>4;
	else c = h>>4;
	j = (float)c / (float)w;
	k = (float)c / (float)h;
	vr[0] = j;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = k;
	vf[2] = 0.0;

	vc[0] = 1.0-j;
	vc[1] = k-1.0;
	vc[2] = -0.99;
	carvesolid2d_circle(win, 0x0000ff, vc, vr, vf);
	vc[0] = j-1.0;
	carvesolid2d_circle(win, 0xff0000, vc, vr, vf);

	c *= 2;
	if(win->input[0].z0)
	{
		j = win->input[0].x0;
		k = win->input[0].y0;
		x = win->input[0].x1;
		y = win->input[0].y1;
	}
	else if(win->input[10].z0)
	{
		j = win->input[10].x0;
		k = win->input[10].y0;
		x = win->input[10].x1;
		y = win->input[10].y1;
	}
	else return;

	if(k+c > h)
	{
		if(j+c > w)
		{
			j = ((h-y)*(h-y)/(x-w) + (w+x)) / 2.0;
			j = 2*j/w - 1.0;
			k = ((x-w)*(x-w)/(y-h) + (y+h)) / 2.0;
			k = 1.0 - 2*k/h;
			vr[0] = j;
			vr[1] = -1.0;
			vr[2] = -0.9;
			vf[0] = 1.0;
			vf[1] = k;
			vf[2] = -0.9;
			vc[0] = 2.0*x/w - 1.0;
			vc[1] = 1.0 - 2.0*y/h;
			vc[2] = -0.9;
			carvesolid2d_triangle(win, 0x808080, vc, vr, vf);
			vc[0] = 1.0;
			vc[1] = -1.0;
			carvesolid2d_triangle(win, 0x000000, vc, vr, vf);
		}
		else if(j < c)
		{
			j = (float)x;
			k = (float)y;
			vc[0] = j/w - 1.0;
			vc[1] = 0.0;
			vc[2] = -0.9;
			vr[0] = 1.0+vc[0];
			vr[1] = 0.0;
			vr[2] = 0.0;
			vf[0] = 0.0;
			vf[1] = 1.0;
			vf[2] = 0.0;
			carvesolid2d_rect(win, 0x404040, vc, vr, vf);
			vc[0] = 0.0;
			vc[1] = 1.0-(h+k)/h;
			vr[0] = 1.0;
			vr[1] = 0.0;
			vf[0] = 0.0;
			vf[1] = 1.0+vc[1];
			carvesolid2d_rect(win, 0x404040, vc, vr, vf);
		}
	}
}
void vkbd_read_html(struct arena* win, struct style* sty)
{
}
void vkbd_read_tui(struct arena* win, struct style* sty)
{
}
void vkbd_read_cli(struct arena* win, struct style* sty)
{
}
void vkbd_read(struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)vkbd_read_cli(win, sty);
	else if(fmt == _tui_)vkbd_read_tui(win, sty);
	else if(fmt == _html_)vkbd_read_html(win, sty);
	else if(fmt == _vbo_)vkbd_read_vbo(win, sty);
	else vkbd_read_pixel(win, sty);
}




int vkbd_event(struct arena* win, struct event* ev)
{
	short tmp[4];
	int x,y,w,h,ret;
	if(win->vkbdtype <= 0)return 0;

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
				if((0==x)&&(1==y))ret = joyl_left;
				else if((2==x)&&(1==y))ret = joyl_right;
				else if((1==x)&&(0==y))ret = joyl_down;
				else if((1==x)&&(2==y))ret = joyl_up;
				else if((0==x)&&(3==y))ret = joyl_trigger;
				else if((2==x)&&(3==y))ret = joyl_bumper;
				else if((1==x)&&(1==y))ret = joyl_stick;
				else if((3==x)&&(2==y))ret = joyl_select;
				else ret = 0;

				if(ret)
				{
					tmp[0] = tmp[1] = tmp[2] = 0;
					tmp[3] = ret;
					eventwrite(*(u64*)tmp, joy_left, 0, 0);
				}
			}
			else
			{
				x = w-x;
				x = x*16/h;
				if((2==x)&&(y==1))ret = joyr_left;
				else if((0==x)&&(1==y))ret = joyr_right;
				else if((1==x)&&(y==0))ret = joyr_down;
				else if((1==x)&&(y==2))ret = joyr_up;
				else if((2==x)&&(y==3))ret = joyr_trigger;
				else if((0==x)&&(y==3))ret = joyr_bumper;
				else if((1==x)&&(y==1))ret = joyr_stick;
				else if((3==x)&&(y==2))ret = joyr_start;
				else ret = 0;

				if(ret)
				{
					tmp[0] = tmp[1] = tmp[2] = 0;
					tmp[3] = ret;
					eventwrite(*(u64*)tmp, joy_right, 0, 0);
				}
			}
			win->vkbdtype = ((int)'j'<<16) + ret;
		}
	}

byebye:
	return 1;
}
int vkbd_write(struct arena* win, struct event* ev)
{
	int j,k,x,y;
	int w,h,id,ret;
	u64 why = ev->why;
	u64 what = ev->what;
	if('p' == (what&0xff))
	{
		w = win->width;
		h = win->height;
		if(w<h)ret = w>>4;
		else ret = h>>4;

		id = (why>>48)&0xffff;
		if('l' == id)id = 10;
		if((0 != id)&&(10 != id))return 0;
		j = win->input[id].x0;
		k = win->input[id].y0;

		if(0x2d70 == what)
		{
			//open or close vkbd
			x = why&0xffff;
			y = (why>>16)&0xffff;
			if(y+ret > h)
			{
				if(x+ret > w)
				{
					if(win->vkbdtype > 0)win->vkbdtype = 0;
					else win->vkbdtype = (int)'j'<<16;
					return 1;
				}
				else if(x < ret)
				{
					if(win->vkbdtype > 0)win->vkbdtype = 0;
					else win->vkbdtype = (int)'k'<<16;
					return 1;
				}
			}
			if(k+ret > h)
			{
				if(j+ret > w)
				{
					if(x*2 < w)win->menutype = 1;
					return 1;
				}
				else if(j < ret)
				{
					if(x*2 > w)win->menutype = 1;
					return 1;
				}
			}
		}
		else if(0x4070 == what)
		{
			if(k+ret > h)
			{
				if(j+ret > w)return 1;
				if(j-ret < 0)return 1;
			}
		}

		//call vkbd
		if(win->vkbdtype >= 0)
		{
			ret = vkbd_event(win, ev);
			if(0 != ret)return 1;
		}
	}
	return 0;
}