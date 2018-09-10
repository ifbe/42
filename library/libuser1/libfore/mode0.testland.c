#include "libuser.h"




void actoroutput_void_pixel(struct arena* win, struct style* sty)
{
	int cx,cy,ww,hh;
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

	drawline(win, 0xffffff, cx-ww, win->forey, cx+ww, win->forey);
	drawline(win, 0xffffff, win->forex, cy-hh, win->forex, cy+hh);

	cx = win->width/2;
	cy = win->height/2;
	drawsolid_rect(win, 0xff00ff, cx-16, cy-16, cx+16, cy+16);
}
void actoroutput_void_vbo(struct arena* win, struct style* sty)
{
	vec3 va;
	vec3 vb;
	vec3 vc;
	float x = (float)(win->forex);
	float y = (float)(win->forey);
	x = 2*x/(win->width) - 1.0;
	y = 1.0 - 2*y/(win->height);

	va[0] = -1.0;
	va[1] = y;
	va[2] = -0.5;
	vb[0] = 1.0;
	vb[1] = y;
	vb[2] = -0.5;
	carveline2d(win, 0xffffff, va, vb);

	va[0] = x;
	va[1] = 1.0;
	va[2] = -0.5;
	vb[0] = x;
	vb[1] = -1.0;
	vb[2] = -0.5;
	carveline2d(win, 0xffffff, va, vb);

	x = 16.0 / (win->width);
	y = 16.0 / (win->height);
	vc[0] = 0.0;
	vc[1] = 0.0;
	vc[2] = -0.8;
	va[0] = x;
	va[1] = 0.0;
	va[2] = 0.0;
	vb[0] = 0.0;
	vb[1] = y;
	vb[2] = 0.0;
	carvesolid2d_rect(win, 0xff00ff, vc, va, vb);
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
void actorinput_void(struct arena* win, struct event* ev)
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
		else
		{
			win->forex = x;
			win->forey = y;
		}
	}
}