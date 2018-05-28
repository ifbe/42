#include "actor.h"




void actoroutput_term_pixel(struct arena* win)
{
	drawsolid_rect(win, 0x404040, 16, 16, win->width-16, win->height-16);
}
void actoroutput_term_vbo(struct arena* win)
{
	vec3 vc;
	vec3 vr;
	vec3 vf;

	vc[0] = 0.0;
	vc[1] = 0.0;
	vc[2] = -0.5;
	vr[0] = 0.95;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = 0.95;
	vf[2] = 0.0;
	carvesolid2d_rect(win, 0x404040, vc, vr, vf);
}
void actoroutput_term_json(struct arena* win)
{
}
void actoroutput_term_html(struct arena* win)
{
}
void actoroutput_term_tui(struct arena* win)
{
}
void actoroutput_term_cli(struct arena* win)
{
}
void actoroutput_term(struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(_cli_ == fmt)actoroutput_term_cli(win);
	else if(_tui_ == fmt)actoroutput_term_tui(win);
	else if(_html_ == fmt)actoroutput_term_html(win);
	else if(_json_ == fmt)actoroutput_term_json(win);
	else if(_vbo_ == fmt)actoroutput_term_vbo(win);
	else actoroutput_term_pixel(win);
}
void actorinput_term(struct arena* win, struct event* ev)
{
	if('p' == (ev->what&0xff))
	{
		win->modex = (ev->why)&0xffff;
		win->modey = ((ev->why)>>16)&0xffff;
	}
}