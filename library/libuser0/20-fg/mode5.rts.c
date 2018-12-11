#include "libuser.h"
void drawborder2d(struct arena* win, struct style* sty, void* name);
void carveborder2d(struct arena* win, struct style* sty, void* name);




void actoroutput_rts_vbo(struct arena* win, struct style* sty)
{
    vec3 vc,vr,vf;

    vc[0] = 0.0;
    vc[1] = -5.0/6;
    vc[2] = -0.5;
    vr[0] = 0.5;
    vr[1] = 0.0;
    vr[2] = 0.0;
    vf[0] = 0.0;
    vf[1] = 1.0/6;
    vf[2] = 0.0;
    carveline2d_rect(win, 0x804000, vc, vr, vf);

    vc[0] = -0.75;
    vc[1] = -3.0/4;
    vc[2] = -0.5;
    vr[0] = 0.25;
    vr[1] = 0.0;
    vr[2] = 0.0;
    vf[0] = 0.0;
    vf[1] = 1.0/4;
    vf[2] = 0.0;
    carveline2d_rect(win, 0x004080, vc, vr, vf);

    vc[0] = 0.75;
    carveline2d_rect(win, 0x004080, vc, vr, vf);
}
void actoroutput_rts_pixel(struct arena* win, struct style* sty)
{
    int w = win->width;
    int h = win->height;
    drawline_rect(win, 0x004080, w*0/4, h*3/4, w/4, h);
    drawline_rect(win, 0x804000, w*1/4, h*5/6, w*3/4, h);
    drawline_rect(win, 0x004080, w*3/4, h*3/4, w, h);
}
int actoroutput_2d(struct arena* win, struct style* stack)
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

			if(_vbo_ == win->fmt)carveborder2d(win, sty, &act->name);
			else drawborder2d(win, sty, &act->name);
		}

		orel = samesrcnextdst(orel);
	}

	if(_vbo_ == win->fmt)actoroutput_rts_vbo(win, stack);
	else actoroutput_rts_pixel(win, stack);
	return 0;
}




int actorinput_2d(struct arena* win, struct style* sty, struct event* ev)
{
	struct actor* act;
	struct pinid* pin;
	struct relation* orel;

	orel = win->oreln;
	if(0 == orel)return 0;

	act = (void*)(orel->dstchip);
	pin = (void*)(orel->dstfoot);
	return act->onwrite(act, pin, 0, 0, ev, 0);
}