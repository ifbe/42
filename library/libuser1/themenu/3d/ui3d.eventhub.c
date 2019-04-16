#include "libuser.h"




void eventhub_debug(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	vec3 tc,tr,tf;
	tr[0] = 1.0 / 32;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0 / 32;
	tf[2] = 0.0;

	tc[0] = -1.0;
	tc[1] = 15.0 / 32;
	tc[2] = 0.0;
	carve2d_vec4(win, 0xffff00, tc, tr, tf, "vl", win->camera.vl);

	tc[0] = -1.0;
	tc[1] = 13.0 / 32;
	tc[2] = 0.0;
	carve2d_vec4(win, 0x0000ff, tc, tr, tf, "vr", win->camera.vr);

	tc[0] = -1.0;
	tc[1] = 11.0 / 32;
	tc[2] = 0.0;
	carve2d_vec4(win, 0xff00ff, tc, tr, tf, "vb", win->camera.vb);

	tc[0] = -1.0;
	tc[1] = 9.0 / 32;
	tc[2] = 0.0;
	carve2d_vec4(win, 0x00ff00, tc, tr, tf, "vu", win->camera.vu);

	tc[0] = -1.0;
	tc[1] = 7.0 / 32;
	tc[2] = 0.0;
	carve2d_vec4(win, 0x00ffff, tc, tr, tf, "vn", win->camera.vn);

	tc[0] = -1.0;
	tc[1] = 5.0 / 32;
	tc[2] = 0.0;
	carve2d_vec4(win, 0xff0000, tc, tr, tf, "vf", win->camera.vf);

	tc[0] = -1.0;
	tc[1] = 3.0 / 32;
	tc[2] = 0.0;
	carve2d_vec4(win, 0x808080, tc, tr, tf, "vq", win->camera.vq);

	tc[0] = -1.0;
	tc[1] = 1.0 / 32;
	tc[2] = 0.0;
	carve2d_vec4(win, 0xffffff, tc, tr, tf, "vc", win->camera.vc);
}
void eventhub_list(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int y;
	vec3 tc, tr, tf;
	struct relation* rel;
	struct arena* twig;
	struct actor* leaf;

	rel = win->orel0;
	while(1)
	{
		if(0 == rel)break;
		if(_win_ == rel->dsttype)
		{
			twig = (void*)(rel->dstchip);
			if(_sb3d_ == twig->fmt)goto found;
		}
		rel = samesrcnextdst(rel);
	}
	return;

found:
	tr[0] = 1.0 / 32;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0 / 32;
	tf[2] = 0.0;

	y = 0;
	rel = twig->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			tc[0] = 1.0 - 0.25;
			tc[1] = (15 - 2*y) / 32.0;
			tc[2] = 0.0;
			y += 1;

			leaf = (void*)(rel->dstchip);

			carve2d_string(win, 0xff00ff, tc, tr, tf, (void*)&leaf->fmt, 8);
		}

		rel = samesrcnextdst(rel);
	}
}
static int eventhub_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	vec3 tc,tr,tf;

	tr[0] = 0.125;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 0.5;
	tf[2] = 0.0;

	tc[0] = 0.125 - 1.0;
	tc[1] = 0.0;
	tc[2] = 0.0;
	carveopaque2d_rect(win, 0xffffff, tc, tr, tf);

	tc[0] = 1.0 - 0.125;
	tc[1] = 0.0;
	tc[2] = 0.0;
	carveopaque2d_rect(win, 0xffffff, tc, tr, tf);

	//
	eventhub_debug(act, pin, win, sty);

	//
	eventhub_list(act, pin, win, sty);
	return 0;
}
static int eventhub_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int x,y,z,w;
	short* t = (void*)ev;

	if(0x2b70 == ev->what){
		x = t[0] * 8 / (win->width);
		y = t[1] * 4 / (win->height);
		//say("x=%d,y=%d\n",x,y);

		if((y >= 1) && (y <= 2)){
			if(x <= 0)return 1;
			if(x >= 7)return 1;
		}
	}
	return 0;
}
static void eventhub_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void eventhub_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void eventhub_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void eventhub_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void eventhub_delete()
{
}
static void eventhub_create(void* addr)
{
}




void eventhub_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('e', 'v', 'e', 'n', 't', 'h', 'u', 'b');

	p->oncreate = (void*)eventhub_create;
	p->ondelete = (void*)eventhub_delete;
	p->onstart  = (void*)eventhub_start;
	p->onstop   = (void*)eventhub_stop;
	p->oncread  = (void*)eventhub_cread;
	p->oncwrite = (void*)eventhub_cwrite;
	p->onsread  = (void*)eventhub_sread;
	p->onswrite = (void*)eventhub_swrite;
}
