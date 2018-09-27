#include "libuser.h"
void* allocstyle();
void* allocpinid();
u64 want[2] = {
	hex64('l','i','g','h','t',0,0,0),//hex64('s','k','y','s','p','h','.','.'),
	hex64('t','e','r','r','i','a','n', 0)
};




int back_read_init(struct arena* win)
{
	int j;
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	rel = (void*)(win->backdata);
	if(0 == rel)return 0;

	for(j=0;j<1;j++)
	{
		act = (void*)(rel[j].dstchip);
		if(0 != act)continue;

		//actor
		act = actorcreate(want[j], 0);
		if(0 == act)return 0;
		rel[j].dstchip = (u64)act;

		//style
		sty = allocstyle();
		sty->vr[0] = 1000000;
		sty->vr[1] = 0.0;
		sty->vr[2] = 0.0;
		sty->vf[0] = 0.0;
		sty->vf[1] = 1000000;
		sty->vf[2] = 0.0;
		sty->vu[0] = 0.0;
		sty->vu[1] = 0.0;
		sty->vu[2] = 1000000;
		rel[j].srcfoot = (u64)sty;

		//pinid
		pin = allocpinid();
		rel[j].dstfoot = (u64)pin;

		//start
		act->onstart(win, sty, act, pin);
	}
	return 0;
}
int back_read_draw(struct arena* win)
{
	int j;
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	rel = (void*)(win->backdata);
	if(0 == rel)return 0;

	for(j=0;j<1;j++)
	{
		act = (void*)(rel[j].dstchip);
		if(0 == act)continue;

		sty = (void*)(rel[j].srcfoot);
		pin = (void*)(rel[j].dstfoot);

		sty->vc[0] = win->target.vc[0];
		sty->vc[1] = win->target.vc[1];
		sty->vc[2] = win->target.vc[2];

		act->onread(win, sty, act, pin);
	}
	return 0;
}
int back_read(struct arena* win, struct style* sty)
{
	int j;
	u8* buf;
	if(_vbo_ != win->fmt)return 0;

	if(0 == win->backdata)
	{
		buf = memorycreate(0x10000);
		for(j=0;j<0x10000;j++)buf[j] = 0;
		win->backdata = (u64)buf;

		back_read_init(win);
	}

	back_read_draw(win);
	return 0;
}
int back_write(struct arena* win, struct event* ev)
{
	return 0;
}
