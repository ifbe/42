#include "libuser.h"
void drawarrorkey2d(void*, u32, int x0, int y0, int x1, int y1, u8*, int);
void carvearrorkey2d(void*, u32, vec3 vc, vec3 vr, vec3 vf, u8*, int);




void vkbd_joystick_read_pixel(struct arena* win, struct style* sty)
{
	u8 ch[8];
	int c,l,rgb;
	int x,y,m,n;
	int w = win->width;
	int h = win->height;
	if(win->vkbdw < 0)goto haha;

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

haha:
	if(w<h)c = w>>6;
	else c = h>>6;

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
		x = win->input[0].xn;
		y = win->input[0].yn;
	}
	else if(win->input[10].z0)
	{
		m = win->input[10].x0;
		n = win->input[10].y0;
		x = win->input[10].xn;
		y = win->input[10].yn;
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
void vkbd_joystick_read_vbo(struct arena* win, struct style* sty)
{
	u8 ch[8];
	float j,k;
	vec3 vc;
	vec3 vr;
	vec3 vf;
	int x,y,c,rgb;
	int w = win->width;
	int h = win->height;
	if(win->vkbdw < 0)goto haha;

	c = win->vkbdw;
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

haha:
	if(w<h)c = w>>5;
	else c = h>>5;
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
		x = win->input[0].xn;
		y = win->input[0].yn;
	}
	else if(win->input[10].z0)
	{
		j = win->input[10].x0;
		k = win->input[10].y0;
		x = win->input[10].xn;
		y = win->input[10].yn;
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
void vkbd_joystick_read_html(struct arena* win, struct style* sty)
{
}
void vkbd_joystick_read_tui(struct arena* win, struct style* sty)
{
}
void vkbd_joystick_read_cli(struct arena* win, struct style* sty)
{
}
void vkbd_joystick_read(struct arena* cc, void* cf, struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)vkbd_joystick_read_cli(win, sty);
	else if(fmt == _tui_)vkbd_joystick_read_tui(win, sty);
	else if(fmt == _html_)vkbd_joystick_read_html(win, sty);
	else if(fmt == _vbo_)vkbd_joystick_read_vbo(win, sty);
	else vkbd_joystick_read_pixel(win, sty);
}




int vkbd_joystick_write(struct arena* win, struct event* ev)
{
	short tmp[4];
	int x,y,w,h,ret;
    say("vkbd_joystick_write\n");
	if(win->vkbdw <= 0)return 0;

	w = win->width;
	h = win->height;
	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	if(y < h*3/4)return 0;

	if(hex32('p','-',0,0) == ev->what)
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

		win->vkbdz = ret;
		win->vkbdw = 'j';
	}

byebye:
	return 1;
}
