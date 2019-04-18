#include "libuser.h"




static int rtsgame_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	return 0;
}
static int rtsgame_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	//say("%llx,%llx\n", ev->why, ev->what);
	int id;
	int x0,y0,x1,y1;
	short* t;
	if(_kbd_ == ev->what)
	{
		if(0x4b == ev->why)win->camera.vc[0] -= 10;
		else if(0x4d == ev->why)win->camera.vc[0] += 10;
		else if(0x50 == ev->why)win->camera.vc[1] -= 10;
		else if(0x48 == ev->why)win->camera.vc[1] += 10;
	}
	else if(0x4070 == ev->what)
	{
		id = (ev->why)>>48;
		if('l' == id)id = 10;
		else if('r' == id)id = 11;
		else if(id > 10)return 0;
		//if(0 == win->input[id].z0)return;

		//x0 = win->input[id].xn;
		//y0 = win->input[id].yn;
		x1 = (ev->why)&0xffff;
		y1 = ((ev->why)>>16)&0xffff;
		//win->camera.vc[0] += x1-x0;
		//win->camera.vc[1] -= y1-y0;

		if(x1 < 16)win->camera.vc[0] -= 10;
		if(x1 > win->width-16)win->camera.vc[0] += 10;
		if(y1 < 16)win->camera.vc[1] += 10;
		if(y1 > win->width-16)win->camera.vc[1] -= 10;
	}

/*
	float w = win->width;
	float h = win->height;
	win->nearn = 1;
	win->nearl =-w/h;
	win->nearr = w/h;
	win->nearb =-1;
	win->neart = 1;
*/
	return 1;
}
static void rtsgame_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void rtsgame_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void rtsgame_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void rtsgame_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
    say("@rtsgame_start\n");
}
static void rtsgame_delete()
{
}
static void rtsgame_create(void* addr)
{
    say("@rtsgame_create\n");
}




void rtsgame_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('r', 't', 's', 'c', 'a', 'm', 0, 0);

	p->oncreate = (void*)rtsgame_create;
	p->ondelete = (void*)rtsgame_delete;
	p->onstart  = (void*)rtsgame_start;
	p->onstop   = (void*)rtsgame_stop;
	p->oncread  = (void*)rtsgame_cread;
	p->oncwrite = (void*)rtsgame_cwrite;
	p->onsread  = (void*)rtsgame_sread;
	p->onswrite = (void*)rtsgame_swrite;
}
