#include "libuser.h"
void* allocstyle();
void* allocpinid();
int actorstart(void*, void*, void*, void*);
int actorinput_touch(struct arena* win, struct event* ev);




static u64 want[] = {
	hex64('o','v','e','r','v','i','e','w'),
	hex64('t','a','b','b','a','r', 0, 0),
	hex32('v','k','b','d'),
	hex64('c','o','r','n','e','r', 0, 0)
};




void ui2d_read_cli(struct arena* win, struct style* sty)
{
}
void ui2d_read_tui(struct arena* win, struct style* sty)
{
}
void ui2d_read_html(struct arena* win, struct style* sty)
{
}
void ui2d_read_json(struct arena* win, struct style* sty)
{
}
void ui2d_read_vbo(struct arena* win, struct style* sty)
{
	int j;
	float x0,y0,x1,y1;
	vec3 vc;
	vec3 vr;
	vec3 vf;
	for(j=0;j<12;j++)
	{
		if(0 == win->input[j].z0)continue;

		x0 = (float)(win->input[j].x0) / (float)(win->width);
		x0 = x0*2 - 1.0;
		y0 = (float)(win->input[j].y0) / (float)(win->height);
		y0 = 1.0 - y0*2;
		x1 = (float)(win->input[j].xn) / (float)(win->width);
		x1 = x1*2 - 1.0;
		y1 = (float)(win->input[j].yn) / (float)(win->height);
		y1 = 1.0 - y1*2;

		vc[0] = x0;
		vc[1] = y0;
		vc[2] = -0.99;
		vr[0] = x1;
		vr[1] = y1;
		vr[2] = -0.99;
		carveline2d_arrow(win, 0xff00ff, vc, vr);
/*
		vc[0] = (x0+x1)/2;
		vc[1] = (y0+y1)/2;
		vc[2] = -0.99;
		vr[0] = (x1-x0)/2;
		vr[1] = 0.0;
		vr[2] = -0.99;
		vf[0] = 0.0;
		vf[1] = (y1-y0)/2;
		vf[2] = -0.99;
		carveline2d_rect(win, 0x00ff00, vc, vr, vf);
*/
	}
}
void ui2d_read_pixel(struct arena* win, struct style* sty)
{
	int j;
	int x0,y0,x1,y1;
	for(j=0;j<12;j++)
	{
		if(0 == win->input[j].z0)continue;

		x0 = win->input[j].x0;
		y0 = win->input[j].y0;
		x1 = win->input[j].xn;
		y1 = win->input[j].yn;
		drawline_arrow(win, 0xff00ff, x0, y0, x1, y1);
		drawline_rect(win, 0x00ff00, x0, y0, x1, y1);
	}
}
int ui2d_read(struct arena* cc, void* cf, struct arena* win, struct style* stack)
{
	u64 fmt;
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;
	win->vfmt = _ui2d_;

	rel = cc->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			act = (void*)(rel->dstchip);
			sty = (void*)(rel->srcfoot);
			pin = (void*)(rel->dstfoot);
			act->onread(win, sty, act, pin);
		}

		rel = samesrcnextdst(rel);
	}
	//actoroutput_overview(win, sty);
	//actoroutput_tabbar(win, sty);

	fmt = win->fmt;
	if(fmt == _cli_)ui2d_read_cli(win, sty);
	else if(fmt == _tui_)ui2d_read_tui(win, sty);
	else if(fmt == _html_)ui2d_read_html(win, sty);
	else if(fmt == _json_)ui2d_read_json(win, sty);
	else if(fmt == _vbo_)ui2d_read_vbo(win, sty);
	else ui2d_read_pixel(win, sty);
	return 0;
}
int ui2d_write(struct arena* cc, void* cf, struct arena* win, struct style* stack, struct event* ev)
{
	int ret;
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	ret = 0;
	rel = cc->oreln;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			act = (void*)(rel->dstchip);
			sty = (void*)(rel->srcfoot);
			pin = (void*)(rel->dstfoot);
			ret = act->onwrite(act, pin, win, 0, ev, 0);
			if(ret)break;
		}

		rel = samesrcprevdst(rel);
	}

	if('p' == (ev->what&0xff))actorinput_touch(win, ev);
	return ret;
}




int ui2d_start(struct arena* c, void* cf, struct arena* r, void* rf)
{
	struct relation* rel;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;

	rel = c->orel0;
	while(1)
	{
		if(0 == rel)break;

		sty = (void*)(rel->srcfoot);
		act = (void*)(rel->dstchip);
		pin = (void*)(rel->dstfoot);
		actorstart(r, sty, act, pin);

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int ui2d_create(struct arena* win, void* str)
{
	int j,k;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
	struct relation* rel;

	for(j=0;j<4;j++)
	{
		act = actorcreate(want[j], 0);
		if(0 == act)continue;

		relationcreate(act, 0, _act_, win, 0, _win_);

		rel = act->irel0;
		if(0 == rel)continue;

		sty = allocstyle();
		if(0 == sty)continue;

		pin = allocpinid();
		if(0 == pin)continue;

		rel->srcfoot = (u64)sty;
		rel->dstfoot = (u64)pin;

		sty->vc[0] = 0;
		sty->vc[1] = 0;
		sty->vc[2] = 0;

		sty->vr[0] = 1000*1000;
		sty->vr[1] = 0;
		sty->vr[2] = 0;

		sty->vf[0] = 0;
		sty->vf[1] = 1000*1000;
		sty->vf[2] = 0;

		sty->vu[0] = 0;
		sty->vu[1] = 0;
		sty->vu[2] = 1000*1000;
	}
	return 0;
}
