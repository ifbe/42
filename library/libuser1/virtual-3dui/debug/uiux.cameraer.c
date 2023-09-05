#include "libuser.h"




void camman_debug(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
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
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int y, rgb;
	vec3 tc, tr, tf;
	struct relation* rel;
	_obj* twig;
	_obj* leaf;

	rel = win->orel0;
	while(1)
	{
		if(0 == rel)break;
		if(_sup_ == rel->dstnodetype)
		{
			twig = (void*)(rel->dstchip);
			//if(_cam3d_ == twig->hfmt)goto found;
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

		if(_ent_ == rel->dstnodetype)
		{
			tc[0] = 1.0 - 0.25;
			tc[1] = (15 - 2*y) / 32.0;
			tc[2] = 0.0;

			if(y == act->whdf.iy0)rgb = 0xff00ff;
			else rgb = 0xffffff;
			y += 1;

			leaf = (void*)(rel->dstchip);
			gl41string(win, rgb, tc, tr, tf, (void*)&leaf->hfmt, 8);
		}

		rel = samesrcnextdst(rel);
	}
}




void camman_listlight(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int y, rgb;
	vec3 tc, tr, tf;
	struct relation* rel;
	_obj* twig;
	_obj* leaf;

	rel = win->orel0;
	while(1)
	{
		if(0 == rel)break;
		if(_sup_ == rel->dstnodetype)
		{
			twig = (void*)(rel->dstchip);
			//if(_lit3d_ == twig->hfmt)goto found;
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

		if(_ent_ == rel->dstnodetype)
		{
			tc[0] = 1.0 - 0.25;
			tc[1] = (-1 - 2*y) / 32.0;
			tc[2] = 0.0;

			if(y == act->whdf.iy0)rgb = 0xff00ff;
			else rgb = 0xffffff;
			y += 1;

			leaf = (void*)(rel->dstchip);
			gl41string(win, rgb, tc, tr, tf, (void*)&leaf->hfmt, 8);
		}

		rel = samesrcnextdst(rel);
	}
}




static int camman_draw(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
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
	gl41opaque_rect(win, 0x3fffffff, tc, tr, tf);
*/
	tc[0] = 1.0 - 0.125;
	tc[1] = 0.0;
	tc[2] = 0.0;
	gl41opaque_rect(win, 0x3fffffff, tc, tr, tf);

	//
	camman_debug(act, pin, win, sty);

	//
	camman_listcamera(act, pin, win, sty);

	//
	camman_listlight(act, pin, win, sty);

	return 0;
}
static int camman_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
	int x,y,z,w;
	short* t = (void*)ev;

	if(0x2b70 == ev->what){
		x = t[0] * 8 / (win->whdf.width);
		y = t[1] * 32 / (win->whdf.height);
		say("x=%d,y=%d\n",x,y);

		if((y >= 8) && (y <= 23)){
			if(x <= 0)return 1;
			if(x >= 7){
				act->whdf.iy0 = y-8;
				return 1;
			}
		}
	}

	return 1;
}




static int camman_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	return 0;
}
static int camman_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	return 0;
}
static int camman_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int camman_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




static void camman_search(_obj* act)
{
}
static void camman_modify(_obj* act)
{
}
static void camman_delete(_obj* act)
{
}
static void camman_create(_obj* act)
{
}




void camman_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('c', 'a', 'm', 'm', 'a', 'n', 0, 0);

	p->oncreate = (void*)camman_create;
	p->ondelete = (void*)camman_delete;
	p->onreader = (void*)camman_search;
	p->onwriter = (void*)camman_modify;

	p->onattach = (void*)camman_attach;
	p->ondetach = (void*)camman_detach;
	p->ontaking = (void*)camman_taking;
	p->ongiving = (void*)camman_giving;
}
