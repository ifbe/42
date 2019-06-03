#include "libuser.h"




static int rtscam_event(
	struct actor* act, struct style* pin,
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




static void rtscam_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static int rtscam_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	return rtscam_event(act, pin, win, sty, ev, 0);
}
static void rtscam_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void rtscam_start(struct halfrel* self, struct halfrel* peer)
{
    say("@rtscam_start\n");
}




static void rtscam_search(struct actor* act)
{
}
static void rtscam_modify(struct actor* act)
{
}
static void rtscam_delete(struct actor* act)
{
}
static void rtscam_create(struct actor* act, void* addr)
{
    say("@rtscam_create\n");
}




void rtscam_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('r', 't', 's', 'c', 'a', 'm', 0, 0);

	p->oncreate = (void*)rtscam_create;
	p->ondelete = (void*)rtscam_delete;
	p->onsearch = (void*)rtscam_search;
	p->onmodify = (void*)rtscam_modify;

	p->onstart = (void*)rtscam_start;
	p->onstop  = (void*)rtscam_stop;
	p->onread  = (void*)rtscam_read;
	p->onwrite = (void*)rtscam_write;
}
