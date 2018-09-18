#include "libuser.h"
void* allocstyle();
void* allocpinid();




int back_read(struct arena* win, struct style* sty)
{
	struct datapair* mod;
	struct actor* act;
	struct style* tmp;
	struct pinid* pin;
	if(_vbo_ != win->fmt)return 0;

	mod = win->mod;
	act = (void*)(win->backdata);
	tmp = (void*)(win->backstyle);
	pin = (void*)(win->backpinid);
	if(0 == act)
	{
		//actor
		act = actorcreate(hex64('s','k','y','s','p','h','.','.'), 0);
		if(0 == act)return 0;
		win->backdata = (u64)act;

		//style
		tmp = allocstyle();
		win->backstyle = (u64)tmp;

		tmp->vr[0] = 1000000;
		tmp->vr[1] = 0.0;
		tmp->vr[2] = 0.0;
		tmp->vf[0] = 0.0;
		tmp->vf[1] = 1000000;
		tmp->vf[2] = 0.0;
		tmp->vu[0] = 0.0;
		tmp->vu[1] = 0.0;
		tmp->vu[2] = 1000000;

		//pinid
		pin = allocpinid();
		win->backpinid = (u64)pin;

		//start
		act->onstart(win, tmp, act, pin);
	}

	tmp->vc[0] = win->camera.vc[0];
	tmp->vc[1] = win->camera.vc[1];
	tmp->vc[2] = win->camera.vc[2];
	act->onread(win, tmp, act, pin);
	return 0;
}
int back_write(struct arena* win, struct event* ev)
{
	return 0;
}
