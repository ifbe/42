#include "libuser.h"




static int eventmap_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	vec3 tc,tr,tf;
	tr[0] = 0.025;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 0.025;
	tf[2] = 0.0;


	//target
	tc[0] = -1.0;
	tc[1] = 0.2 - 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0x0000ff, tc, tr, tf, (void*)"vr: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0x0000ff, tc, tr, tf, win->target.vr);

	tc[0] = -1.0;
	tc[1] = 0.15 - 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0x00ff00, tc, tr, tf, (void*)"vf: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0x00ff00, tc, tr, tf, win->target.vf);

	tc[0] = -1.0;
	tc[1] = 0.1 - 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xff0000, tc, tr, tf, (void*)"vu: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xff0000, tc, tr, tf, win->target.vu);

	tc[0] = -1.0;
	tc[1] = 0.05 - 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xffffff, tc, tr, tf, (void*)"vc: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xffffff, tc, tr, tf, win->target.vc);


	//camera
	tc[0] = -1.0;
	tc[1] = -0.05 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0x0000ff, tc, tr, tf, (void*)"vl: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0x0000ff, tc, tr, tf, win->camera.vl);

	tc[0] = -1.0;
	tc[1] = -0.1 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0x0000ff, tc, tr, tf, (void*)"vr: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0x0000ff, tc, tr, tf, win->camera.vr);

	tc[0] = -1.0;
	tc[1] = -0.15 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0x00ff00, tc, tr, tf, (void*)"vb: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0x00ff00, tc, tr, tf, win->camera.vb);

	tc[0] = -1.0;
	tc[1] = -0.2 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0x00ff00, tc, tr, tf, (void*)"vu: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0x00ff00, tc, tr, tf, win->camera.vu);

	tc[0] = -1.0;
	tc[1] = -0.25 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xff0000, tc, tr, tf, (void*)"vn: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xff0000, tc, tr, tf, win->camera.vn);

	tc[0] = -1.0;
	tc[1] = -0.3 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xff0000, tc, tr, tf, (void*)"vf: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xff0000, tc, tr, tf, win->camera.vf);

	tc[0] = -1.0;
	tc[1] = -0.35 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xffffff, tc, tr, tf, (void*)"vq: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xffffff, tc, tr, tf, win->camera.vq);

	tc[0] = -1.0;
	tc[1] = -0.4 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xffffff, tc, tr, tf, (void*)"vc: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xffffff, tc, tr, tf, win->camera.vc);

	return 0;
}
static int eventmap_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	if(joy_event == (ev->what & 0xff))return 1;
	say("%llx,%llx\n", ev->what, ev->why);
	return 0;
}
static void eventmap_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void eventmap_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void eventmap_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void eventmap_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void eventmap_delete()
{
}
static void eventmap_create(void* addr)
{
}




void eventmap_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('e', 'v', 'e', 'n', 't', 'm', 'a', 'p');

	p->oncreate = (void*)eventmap_create;
	p->ondelete = (void*)eventmap_delete;
	p->onstart  = (void*)eventmap_start;
	p->onstop   = (void*)eventmap_stop;
	p->oncread  = (void*)eventmap_cread;
	p->oncwrite = (void*)eventmap_cwrite;
	p->onsread  = (void*)eventmap_sread;
	p->onswrite = (void*)eventmap_swrite;
}
