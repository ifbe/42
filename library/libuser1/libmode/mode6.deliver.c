#include "libuser.h"




int actoroutput_deliver_recursion(
	struct arena* uwin, struct style* usty,
	struct actor* uact, struct pinid* upin)
{
	int x,y,j;
	struct style st;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
	struct relation* orel;
	st.vr[0] = usty->vr[0]/2;
	st.vr[1] = usty->vr[1]/2;
	st.vr[2] = usty->vr[2]/2;
	st.vf[0] = usty->vf[0]/2;
	st.vf[1] = usty->vf[1]/2;
	st.vf[2] = usty->vf[2]/2;
	st.vu[0] = usty->vu[0]/2;
	st.vu[1] = usty->vu[1]/2;
	st.vu[2] = usty->vu[2]/2;

	j = 0;
	orel = uact->orel0;
	while(1)
	{
		if(0 == orel)break;

		if(_act_ == orel->dsttype)
		{
			x = j % 2;
			y = j / 2;
			act = (void*)(orel->dstchip);
			pin = (void*)(orel->dstfoot);
			sty = (void*)(orel->srcfoot);
			if(sty)
			{
			}
			else
			{
				st.vc[0] = usty->vc[0] + st.vr[0]*(2*x-1) + st.vf[0]*(2*y-1);
				st.vc[1] = usty->vc[1] + st.vr[1]*(2*x-1) + st.vf[1]*(2*y-1);
				st.vc[2] = usty->vc[2] + st.vr[2]*(2*x-1) + st.vf[2]*(2*y-1);

				sty = &st;
			}

			act->onread(uwin, sty, act, pin);
			if(act->orel0)actoroutput_deliver_recursion(uwin, sty, act, pin);
		}

		j++;
		orel = samesrcnextdst(orel);
	}
}
int actoroutput_deliver(struct arena* win, struct style* stack)
{
	struct relation* orel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	orel = win->orel0;
	while(1)
	{
		if(orel == 0)break;

		if(_act_ == orel->dsttype)
		{
			act = (void*)(orel->dstchip);
			pin = (void*)(orel->dstfoot);
			//say("%x,%x,%x,%x\n", win, act, sty, pin);
			//say("%x\n", rel);

			sty = (void*)(orel->srcfoot);
			act->onread(win, sty, act, pin);
			if(act->orel0)actoroutput_deliver_recursion(win, sty, act, pin);
		}

		orel = samesrcnextdst(orel);
	}
	return 0;
}




int actorinput_deliver(struct arena* win, struct event* ev)
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