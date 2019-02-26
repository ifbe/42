#include "libuser.h"
int vkbd_joystick_read(struct arena* cc,void* cf,struct arena* win,void* sty);
int vkbd_joystick_write(struct arena* win,struct event* ev);
int vkbd_keyboard_read(struct arena* cc,void* cf,struct arena* win,void* sty);
int vkbd_keyboard_write(struct arena* win,struct event* ev);



void vkbd_read_pixel(struct arena* win, struct style* sty)
{
	int x,y,m,n,c;
	int w = win->width;
	int h = win->height;
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

	if(n+c < h)return;
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
void vkbd_read_vbo(struct arena* win, struct style* sty)
{
	int c;
	vec3 vc;
	vec3 vr;
	vec3 vf;
	float j,k,x,y;
	float w = win->width;
	float h = win->height;
	if(w<h)c = w / 32;
	else c = h / 32;

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

	if(k+c < h)return;
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




static int vkbd_cread(struct arena* cc, struct style* cf, struct arena* win, struct style* sty)
{
	return 0;
}
static int vkbd_cwrite(struct arena* win, struct style* stack, struct event* ev)
{
	return 0;
}
static int vkbd_sread(struct arena* win, struct style* sty, struct arena* cc, struct style* cf)
{
	switch(win->vkbdw)
	{
		case 'j':vkbd_joystick_read(0, 0, win, sty);break;
		case 'k':vkbd_keyboard_read(0, 0, win, sty);break;
	}
	switch(win->fmt)
	{
		case _vbo_:vkbd_read_vbo(win, sty);break;
		default:vkbd_read_pixel(win, sty);
	}
	return 0;
}
static int vkbd_swrite(struct arena* win, struct style* stack, struct event* ev)
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
					if(win->vkbdw > 0)win->vkbdw = 0;
					else win->vkbdw = 'j';
					return 1;
				}
				else if(x < ret)
				{
					if(win->vkbdw > 0)win->vkbdw = 0;
					else win->vkbdw = 'k';
					return 1;
				}
			}
			if(k+ret > h)
			{
				if(j+ret > w)
				{
					if(x*2 < w)
					{
						j = win->forew;
						win->forew = (j & 0xf) | 0x10;
					}
					return 1;
				}
				else if(j < ret)
				{
					if(x*2 > w)
					{
						j = win->forew;
						win->forew = (j & 0xf) | 0x20;
					}
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
		if(0 == win->vkbdw)return 0;
		switch(win->vkbdw)
		{
			case 'j':ret = vkbd_joystick_write(win, ev);break;
			case 'k':ret = vkbd_keyboard_write(win, ev);break;
		}
		if(0 != ret)return 1;
	}
	return 0;
}
static int vkbd_stop(struct arena* c, void* cf, struct arena* r, void* rf)
{
	return 0;
}
static int vkbd_start(struct arena* c, void* cf, struct arena* r, void* rf)
{
	return 0;
}
static int vkbd_delete(struct arena* win)
{
	return 0;
}
static int vkbd_create(struct arena* win, u8* str)
{
	return 0;
}




void vkbd_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex32('v', 'k', 'b', 'd');

	p->oncreate = (void*)vkbd_create;
	p->ondelete = (void*)vkbd_delete;
	p->onstart  = (void*)vkbd_start;
	p->onstop   = (void*)vkbd_stop;
	p->onget    = (void*)vkbd_cread;
	p->onpost   = (void*)vkbd_cwrite;
	p->onread   = (void*)vkbd_sread;
	p->onwrite  = (void*)vkbd_swrite;
}