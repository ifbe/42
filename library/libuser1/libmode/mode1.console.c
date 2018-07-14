#include "libuser.h"




void actoroutput_console_pixel(struct arena* win, struct style* sty)
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
	drawsolid_rect(win, 0xffffff, cx-ww+16, cy-hh+16, cx+ww-16, cy+hh-16);
	drawsolid_rect(win, 0xe0e0e0, cx+ww-32, cy-hh+16, cx+ww-16, cy+hh-16);
}
void actoroutput_console_vbo(struct arena* win, struct style* sty)
{
	vec3 tc, tr, tf;
	struct style tmp;
	float* vc;
	float* vr;
	float* vf;
	if(0 == sty)
	{
		sty = &tmp;
		sty->vc[0] = 0.0;
		sty->vc[1] = 0.0;
		sty->vc[2] = -0.9;
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

	tc[0] = vc[0];
	tc[1] = vc[1];
	tc[2] = -0.7;
	tr[0] = vr[0]*0.99;
	tr[1] = vr[1]*0.99;
	tr[2] = vr[2]*0.99;
	tf[0] = vf[0]*0.99;
	tf[1] = vf[1]*0.99;
	tf[2] = vf[2]*0.99;
	carvesolid2d_rect(win, 0x404040, tc, tr, tf);
}
void actoroutput_console_json(struct arena* win, struct style* sty)
{
}
void actoroutput_console_html(struct arena* win, struct style* sty)
{
}
void actoroutput_console_tui(struct arena* win, struct style* sty)
{
}
void actoroutput_console_cli(struct arena* win, struct style* sty)
{
}
void actoroutput_console(struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(_cli_ == fmt)actoroutput_console_cli(win, sty);
	else if(_tui_ == fmt)actoroutput_console_tui(win, sty);
	else if(_html_ == fmt)actoroutput_console_html(win, sty);
	else if(_json_ == fmt)actoroutput_console_json(win, sty);
	else if(_vbo_ == fmt)actoroutput_console_vbo(win, sty);
	else actoroutput_console_pixel(win, sty);
}
void actorinput_console(struct arena* win, struct event* ev)
{
	if('p' == (ev->what&0xff))
	{
		win->modex = (ev->why)&0xffff;
		win->modey = ((ev->why)>>16)&0xffff;
	}
}