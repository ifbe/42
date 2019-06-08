#include "libuser.h"




void camman_debug(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{/*
	vec3 tc,tr,tf;
	tr[0] = 1.0 / 64;
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
	carve2d_vec4(win, 0x00ff00, tc, tr, tf, "vu", win->camera.vt);

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
	carve2d_vec4(win, 0xffffff, tc, tr, tf, "vc", win->camera.vc);*/
}




void camman_listcamera(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int y, rgb;
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
			if(_cam3d_ == twig->fmt)goto found;
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

			if(y == act->y0)rgb = 0xff00ff;
			else rgb = 0xffffff;
			y += 1;

			leaf = (void*)(rel->dstchip);
			carve2d_string(win, rgb, tc, tr, tf, (void*)&leaf->fmt, 8);
		}

		rel = samesrcnextdst(rel);
	}
}




void camman_listlight(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int y, rgb;
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
			if(_lit3d_ == twig->fmt)goto found;
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
			tc[1] = (-1 - 2*y) / 32.0;
			tc[2] = 0.0;

			if(y == act->y0)rgb = 0xff00ff;
			else rgb = 0xffffff;
			y += 1;

			leaf = (void*)(rel->dstchip);
			carve2d_string(win, rgb, tc, tr, tf, (void*)&leaf->fmt, 8);
		}

		rel = samesrcnextdst(rel);
	}
}




static int camman_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	vec3 tc,tr,tf;

	tr[0] = 0.125;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 0.5;
	tf[2] = 0.0;
/*
	tc[0] = 0.125 - 1.0;
	tc[1] = 0.0;
	tc[2] = 0.0;
	carveopaque2d_rect(win, 0x3fffffff, tc, tr, tf);
*/
	tc[0] = 1.0 - 0.125;
	tc[1] = 0.0;
	tc[2] = 0.0;
	carveopaque2d_rect(win, 0x3fffffff, tc, tr, tf);

	//
	camman_debug(act, pin, win, sty);

	//
	camman_listcamera(act, pin, win, sty);

	//
	camman_listlight(act, pin, win, sty);

	return 0;
}
static int camman_event(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int x,y,z,w;
	short* t = (void*)ev;

	if(0x2b70 == ev->what){
		x = t[0] * 8 / (win->width);
		y = t[1] * 32 / (win->height);
		say("x=%d,y=%d\n",x,y);

		if((y >= 8) && (y <= 23)){
			if(x <= 0)return 1;
			if(x >= 7){
				act->y0 = y-8;
				return 1;
			}
		}
	}

	return 1;
}




static void camman_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	camman_draw(act, pin, win, sty);
}
static int camman_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	return camman_event(act, pin, win, sty, ev, 0);
}
static void camman_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void camman_start(struct halfrel* self, struct halfrel* peer)
{
}




static void camman_search(struct actor* act)
{
}
static void camman_modify(struct actor* act)
{
}
static void camman_delete(struct actor* act)
{
}
static void camman_create(struct actor* act)
{
}




void camman_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('c', 'a', 'm', 'm', 'a', 'n', 0, 0);

	p->oncreate = (void*)camman_create;
	p->ondelete = (void*)camman_delete;
	p->onsearch = (void*)camman_search;
	p->onmodify = (void*)camman_modify;

	p->onstart = (void*)camman_start;
	p->onstop  = (void*)camman_stop;
	p->onread  = (void*)camman_read;
	p->onwrite = (void*)camman_write;
}
