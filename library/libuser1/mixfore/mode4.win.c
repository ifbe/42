#include "libuser.h"




void border_pixel(struct arena* win, struct style* sty, void* name)
{
	int cx = sty->vc[0];
	int cy = sty->vc[1];
	int ww = sty->vr[0];
	int hh = sty->vf[1];
	drawsolid_rect(win, 0xff00ff, cx-ww, cy-hh-16, cx+ww, cy-hh);
	drawstring_fit(win, 0, cx-ww, cy-hh-16, cx+ww, cy-hh, name, 8);
}
void border_vbo(struct arena* win, struct style* sty, void* name)
{
}
int actoroutput_win(struct arena* win, struct style* stack)
{
	struct relation* orel;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;

	//world
	orel = win->orel0;
	while(1)
	{
		if(orel == 0)break;

		if(_act_ == orel->dsttype)
		{
			act = (void*)(orel->dstchip);
			pin = (void*)(orel->dstfoot);
			sty = (void*)(orel->srcfoot);
			act->onread(win, sty, act, pin);
		}

		if(_vbo_ == win->fmt)border_vbo(win, sty, &act->name);
		else border_pixel(win, sty, &act->name);
		orel = samesrcnextdst(orel);
	}

	return 0;
}
int actorinput_win(struct arena* win, struct style* sty, struct event* ev)
{
	return 0;
}
