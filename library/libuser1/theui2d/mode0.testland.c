#include "libuser.h"




void actoroutput_void_pixel(struct arena* win, struct style* sty)
{
	int x,y,cx,cy,ww,hh;
	int w = win->width;
	int h = win->height;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

	//red point
	x = 32 * (ww+hh) / (w+h);
	drawsolid_rect(win, 0xff00ff, cx-16, cy-16, cx+16, cy+16);

	//heng shu
	x = win->input[10].xn;
	y = win->input[10].yn;
	drawline(win, 0xffffff, cx-ww, y, cx+ww, y);
	drawline(win, 0xffffff, x, cy-hh, x, cy+hh);
}
void actoroutput_void_vbo(struct arena* win, struct style* sty)
{
	float x;
	float y;
	float* vc;
	float* vr;
	float* vf;
	vec3 tc;
	vec3 tr;
	vec3 tf;
	struct style tmp;
	if(0 == sty)
	{
		sty = &tmp;
		sty->vc[0] = 0.0;
		sty->vc[1] = 0.0;
		sty->vc[2] = -0.5;
		sty->vr[0] = 1.0;
		sty->vr[1] = 0.0;
		sty->vr[2] = 0.0;
		sty->vf[0] = 0.0;
		sty->vf[1] = 1.0;
		sty->vf[2] = 0.0;
	}
	vc = sty->vc;
	vr = sty->vr;
	vf = sty->vf;

	//red point
	x = 16.0 / (win->width);
	y = 16.0 / (win->height);
	tc[0] = vc[0];
	tc[1] = vc[1];
	tc[2] = vc[2] - 0.2;
	tr[0] = vr[0]*x;
	tr[1] = vr[1];
	tr[2] = vr[2];
	tf[0] = vf[0];
	tf[1] = vf[1]*y;
	tf[2] = vf[2];
	carvesolid2d_rect(win, 0xff00ff, tc, tr, tf);

	//heng shu
	x = (float)(win->input[10].xn);
	x = 2*x/(win->width) - 1.0;
	y = (float)(win->input[10].yn);
	y = 1.0 - 2*y/(win->height);

	tc[0] = -1.0;
	tc[1] = y;
	tc[2] = -0.9;
	tr[0] = 1.0;
	tr[1] = y;
	tr[2] = -0.9;
	carveline2d(win, 0xffffff, tc, tr);

	tc[0] = x;
	tc[1] = 1.0;
	tc[2] = -0.9;
	tr[0] = x;
	tr[1] = -1.0;
	tr[2] = -0.9;
	carveline2d(win, 0xffffff, tc, tr);
}
void actoroutput_void_json(struct arena* win, struct style* sty)
{
}
void actoroutput_void_html(struct arena* win, struct style* sty)
{
}
void actoroutput_void_tui(struct arena* win, struct style* sty)
{
}
void actoroutput_void_cli(struct arena* win, struct style* sty)
{
}
void actoroutput_void(struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(_cli_ == fmt)actoroutput_void_cli(win, sty);
	else if(_tui_ == fmt)actoroutput_void_tui(win, sty);
	else if(_html_ == fmt)actoroutput_void_html(win, sty);
	else if(_json_ == fmt)actoroutput_void_json(win, sty);
	else if(_vbo_ == fmt)actoroutput_void_vbo(win, sty);
	else actoroutput_void_pixel(win, sty);
}




void actorinput_void(struct arena* win, struct style* sty, struct event* ev)
{
	int x,y;
	if('p' == (ev->what&0xff))
	{
		x = (ev->why)&0xffff;
		y = ((ev->why)>>16)&0xffff;

		if(	(0x2d70 == ev->what) &&
			(x > (win->width/2)-16) &&
			(x < (win->width/2)+16) &&
			(y > (win->height/2)-16) &&
			(y < (win->height/2)+16) )
		{
			eventwrite(0,0,0,0);
		}
	}
}