#include "libuser.h"




void border_pixel(struct arena* win, struct style* sty, void* name)
{
	int cx = sty->vc[0];
	int cy = sty->vc[1];
	int ww = sty->vr[0];
	int hh = sty->vf[1];
	drawline_rect(win, 0x400040, cx-ww, cy-hh, cx+ww, cy+hh);
	drawsolid_rect(win, 0xff00ff, cx-ww, cy-hh-16, cx+ww, cy-hh);
	drawstring_fit(win, 0, cx-ww, cy-hh-16, cx+ww, cy-hh, name, 8);
}
void border_vbo(struct arena* win, struct style* sty, void* name)
{
	vec3 tc,tr,tf;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	int w = win->width;
	int h = win->height;

	tc[0] = vc[0] / w;
	tc[1] = vc[1] / h;
	tc[2] = -0.5;
	tr[0] = vr[0] / w;
	tr[1] = vr[1] / h;
	tr[2] = 0.0;
	tf[0] = vf[0] / w;
	tf[1] = vf[1] / h;
	tf[2] = 0.0;
	carveline2d_rect(win, 0xff00ff, tc, tr, tf);

	tc[0] = vc[0] / w;
	tc[1] = (vc[1]+vf[1]+16.0) / h;
	tc[2] = -0.5;
	tf[0] = 0.0;
	tf[1] = 16.0 / h;
	tf[2] = 0.0;
	carvesolid2d_rect(win, 0xff00ff, tc, tr, tf);

	tc[2] = -0.6;
	tr[0] = 64.0 / w;
	carvestring2d_center(win, 0xffffff, tc, tr, tf, name, 0);
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
