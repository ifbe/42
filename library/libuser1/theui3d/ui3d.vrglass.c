#include "libuser.h"




static int vrglass_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float y;
	vec3 tc,tr,tf,tu;
	tr[0] = win->width/2;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tu[0] = 0.0;
	tu[1] = 0.0;
	tu[2] = win->height/2;

	tc[0] = 0.0;
	tc[1] = 0.0;
	tc[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1000.0;
	tf[2] = 0.0;
	carveline_prism4(win, 0xff0000, tc, tr, tf, tu);

	for(y=-1000.0;y<1001.0;y+=500.0)
	{
		tc[0] = 0.0;
		tc[1] = y;
		tc[2] = 0.0;
		carveline_rect(win, 0xff0000, tc, tr, tu);
	}
	return 0;
}
static int vrglass_swrite(
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
		else if(0x50 == ev->why)win->camera.vc[2] -= 10;
		else if(0x48 == ev->why)win->camera.vc[2] += 10;
	}
	else if(0x2b70 == ev->what)
	{
		id = (ev->why)>>48;
		if('f' == id)win->camera.vc[1] += 10;
		if('b' == id)win->camera.vc[1] -= 10;
	}
	else if(0x4070 == ev->what)
	{
		id = (ev->why)>>48;
		if('l' == id)id = 10;
		else if('r' == id)id = 11;
		else if(id > 10)return 0;
		if(0 == win->input[id].z0)return 0;

		x0 = win->input[id].xn;
		y0 = win->input[id].yn;
		x1 = (ev->why)&0xffff;
		y1 = ((ev->why)>>16)&0xffff;

		win->camera.vc[0] += x1-x0;
		win->camera.vc[2] -= y1-y0;
	}
	else if(joy_left == (ev->what & joy_mask))
	{
		t = (void*)ev;
		//printmemory(t, 16);
		//say("%d,%d\n", t[0], t[1]);
		if(t[3] & joyl_left)		//x-
		{
			win->camera.vc[0] -= 10;
		}
		if(t[3] & joyl_right)		//x+
		{
			win->camera.vc[0] += 10;
		}
		if(t[3] & joyl_down)		//y-
		{
			win->camera.vc[2] -= 10;
		}
		if(t[3] & joyl_up)			//y+
		{
			win->camera.vc[2] += 10;
		}
		if(t[3] & joyl_trigger)		//z-
		{
			win->camera.vc[1] -= 10.0;
		}
		if(t[3] & joyl_bumper)		//z+
		{
			win->camera.vc[1] += 10.0;
		}
		if(t[3] & joyl_stick)		//w-
		{
			win->camera.vc[0] = 0.0;
			win->camera.vc[1] = -2000.0;
			win->camera.vc[2] = 0.0;
		}
		if(t[3] & joyl_select)		//w+
		{
		}

		x0 = t[0];
		y0 = t[1];
		if((x0 < -4096) | (x0 > 4096) | (y0 < -4096) | (y0 > 4096))
		{
			win->camera.vc[0] += x0/1000.0;
			win->camera.vc[2] += y0/1000.0;
		}
	}
say("%f,%f,%f\n",win->camera.vc[0], win->camera.vc[1], win->camera.vc[2]);
	win->target.vc[0] = win->camera.vc[0];
	win->target.vc[1] = win->camera.vc[1] + 1.0;
	win->target.vc[2] = win->camera.vc[2];
	win->camera.vf[0] = 0.0;
	win->camera.vf[1] = 1.0;
	win->camera.vf[2] = 0.0;

	win->nearn = -1000 - win->camera.vc[1];
	win->nearl =-win->width/2 - win->camera.vc[0];
	win->nearr = win->width/2 - win->camera.vc[0];
	win->nearb =-win->height/2 - win->camera.vc[2];
	win->neart = win->height/2 - win->camera.vc[2];
	return 1;
}
static void vrglass_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void vrglass_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void vrglass_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void vrglass_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
    say("@vrglass_start\n");
}
static void vrglass_delete()
{
}
static void vrglass_create(void* addr)
{
    say("@vrglass_create\n");
}




void vrglass_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('v', 'r', 'g', 'l', 'a', 's', 's', 0);

	p->oncreate = (void*)vrglass_create;
	p->ondelete = (void*)vrglass_delete;
	p->onstart  = (void*)vrglass_start;
	p->onstop   = (void*)vrglass_stop;
	p->onget    = (void*)vrglass_cread;
	p->onpost   = (void*)vrglass_cwrite;
	p->onread   = (void*)vrglass_sread;
	p->onwrite  = (void*)vrglass_swrite;
}