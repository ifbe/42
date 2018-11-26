#include "libuser.h"
int relation_choose(void*, void*);
int rect_point(vec3 crf[], vec3 xyz, vec3 out[]);
void drawborder2d(struct arena* win, struct style* sty, void* name);
void carveborder2d(struct arena* win, struct style* sty, void* name);




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

		if(_vbo_ == win->fmt)carveborder2d(win, sty, &act->name);
		else drawborder2d(win, sty, &act->name);
		orel = samesrcnextdst(orel);
	}

	return 0;
}




int playwith2d_pick(struct arena* win, int x, int y)
{
	int ret;
	vec3 out;
	vec3 xyz;
	vec3 crf[3];
	struct style* sty;
	struct relation* rel;

	xyz[0] = (float)x;
	xyz[1] = (float)y;

	rel = win->oreln;
	while(1)
	{
		if(0 == rel)break;

		sty = (void*)(rel->srcfoot);
		crf[0][0] = sty->vc[0];
		crf[0][1] = sty->vc[1];
		crf[0][2] = sty->vc[2];
		crf[1][0] = sty->vr[0];
		crf[1][1] = sty->vr[1];
		crf[1][2] = sty->vr[2];
		crf[2][0] = sty->vf[0];
		crf[2][1] = sty->vf[1];
		crf[2][2] = sty->vf[2];
		ret = rect_point(crf, xyz, &out);
		if(ret > 0)break;

		rel = samesrcprevdst(rel);
	}
	if(rel)relation_choose(win, rel);
	return 0;
}
int actorinput_win(struct arena* win, struct style* stack, struct event* ev)
{
	struct relation* orel;
	struct style* sty;
	int ax, ay, aaa, bbb;
	int x = (ev->why)&0xffff;
	int y = ((ev->why)>>16)&0xffff;
	int z = ((ev->why)>>32)&0xffff;
	int id = ((ev->why)>>48)&0xffff;

	orel = win->oreln;
	if(0 == orel)return 1;

	sty = (void*)(orel->srcfoot);
	if(0 == sty)return 1;

	if('f' == id)
	{
		sty->vr[0] = (sty->vr[0])*17/16;
		sty->vr[1] = (sty->vr[1])*17/16;
		sty->vr[2] = (sty->vr[2])*17/16;

		sty->vf[0] = (sty->vf[0])*17/16;
		sty->vf[1] = (sty->vf[1])*17/16;
		sty->vf[2] = (sty->vf[2])*17/16;

		sty->vu[0] = (sty->vu[0])*17/16;
		sty->vu[1] = (sty->vu[1])*17/16;
		sty->vu[2] = (sty->vu[2])*17/16;
		return 0;
	}
	if('b' == id)
	{
		sty->vr[0] = (sty->vr[0])*15/16;
		sty->vr[1] = (sty->vr[1])*15/16;
		sty->vr[2] = (sty->vr[2])*15/16;

		sty->vf[0] = (sty->vf[0])*15/16;
		sty->vf[1] = (sty->vf[1])*15/16;
		sty->vf[2] = (sty->vf[2])*15/16;

		sty->vu[0] = (sty->vu[0])*15/16;
		sty->vu[1] = (sty->vu[1])*15/16;
		sty->vu[2] = (sty->vu[2])*15/16;
		return 0;
	}
	if(hex32('p','+',0,0) == ev->what)
	{
		playwith2d_pick(win, x, y);
		return 0;
	}
	if(hex32('p','@',0,0) == ev->what)
	{
		if('l' == id)id = 10;
		else if('r' == id)id = 11;
		else if(id > 10)return 0;
		if(0 == win->input[id].z0)return 0;

		//two finger
		if(	(0 != win->input[0].z0)&&
			(0 != win->input[1].z0) )
		{
			if(0 == id)
			{
				x -= (win->input[1].x1);
				y -= (win->input[1].y1);
			}
			if(1 == id)
			{
				x -= (win->input[0].x1);
				y -= (win->input[0].y1);
			}

			ax = (win->input[0].x1) - (win->input[1].x1);
			ay = (win->input[0].y1) - (win->input[1].y1);

			aaa = x*x+y*y;
			bbb = ax*ax + ay*ay;

			sty->vr[0] = (sty->vr[0]) * aaa / bbb;
			sty->vr[1] = (sty->vr[1]) * aaa / bbb;
			sty->vr[2] = (sty->vr[2]) * aaa / bbb;

			sty->vf[0] = (sty->vf[0]) * aaa / bbb;
			sty->vf[1] = (sty->vf[1]) * aaa / bbb;
			sty->vf[2] = (sty->vf[2]) * aaa / bbb;

			sty->vu[0] = (sty->vu[0]) * aaa / bbb;
			sty->vu[1] = (sty->vu[1]) * aaa / bbb;
			sty->vu[2] = (sty->vu[2]) * aaa / bbb;
		}
		else if((0 == id)|(10 == id))
		{
			sty->vc[0] += x - (win->input[id].x1);
			sty->vc[1] += y - (win->input[id].y1);
			//say("%x,%x\n", sty->vc[0], sty->vc[1]);
		}
	}
	return 0;
}
