#include "libuser.h"
#define _car_ hex32('c','a','r',0)
int boardread(int,int,void*,int);
int boardwrite(int,int,void*,int);




static void rccar_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void rccar_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
    vec3 tc,tr,tf,tu;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

    tr[0] = vr[0] * 3 / 4;
    tr[1] = vr[1] * 3 / 4;
    tr[2] = vr[2] * 3 / 4;
    tf[0] = vf[0] * 3 / 4;
    tf[1] = vf[1] * 3 / 4;
    tf[2] = vf[2] * 3 / 4;
    carvesolid2d_rect(win, 0x808080, vc, tr, tf);

	tr[0] = vr[0] / 4;
	tr[1] = vr[1] / 4;
	tr[2] = vr[2] / 4;
	tf[0] = vf[0] / 4;
	tf[1] = vf[1] / 4;
	tf[2] = vf[2] / 4;
	for(y=-1;y<2;y+=2)
	{
		for(x=-1;x<2;x+=2)
		{
			tc[0] = vc[0] + x*vr[0]*3/4 + y*vf[0]*3/4;
			tc[1] = vc[1] + x*vr[1]*3/4 + y*vf[1]*3/4;
			tc[2] = vc[2] + x*vr[2]*3/4 + y*vf[2]*3/4;
			carvesolid2d_rect(win, 0x202020, tc, tr, tf);
		}
	}
}
static void rccar_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
    vec3 tc,tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

    tr[0] = vr[0] * 3 / 4;
    tr[1] = vr[1] * 3 / 4;
    tr[2] = vr[2] * 3 / 4;
    tf[0] = vf[0] * 3 / 4;
    tf[1] = vf[1] * 3 / 4;
    tf[2] = vf[2] * 3 / 4;
	tu[0] = vu[0] / 8;
	tu[1] = vu[1] / 8;
	tu[2] = vu[2] / 8;
	tc[0] = vc[0] + vu[0]/4;
	tc[1] = vc[1] + vu[1]/4;
	tc[2] = vc[2] + vu[2]/4;
    carvesolid_prism4(win, 0x808080, tc, tr, tf, tu);

	tr[0] = vf[0] / 4;
	tr[1] = vf[1] / 4;
	tr[2] = vf[2] / 4;
	tf[0] = vu[0] / 4;
	tf[1] = vu[1] / 4;
	tf[2] = vu[2] / 4;
	tu[0] = vr[0] / 4;
	tu[1] = vr[1] / 4;
	tu[2] = vr[2] / 4;

	for(y=-1;y<2;y+=2)
	{
		for(x=-1;x<2;x+=2)
		{
			tc[0] = vc[0] + x*vr[0]*3/4 + y*vf[0]*3/4 + vu[0]/4;
			tc[1] = vc[1] + x*vr[1]*3/4 + y*vf[1]*3/4 + vu[1]/4;
			tc[2] = vc[2] + x*vr[2]*3/4 + y*vf[2]*3/4 + vu[2]/4;
			carvesolid_cylinder(win, 0x202020, tc, tr, tf, tu);
		}
	}
}
static void rccar_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rccar_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rccar_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rccar_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rccar_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)rccar_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)rccar_read_tui(win, sty, act, pin);
	else if(fmt == _html_)rccar_read_html(win, sty, act, pin);
	else if(fmt == _json_)rccar_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)rccar_read_vbo2d(win, sty, act, pin);
		else rccar_read_vbo(win, sty, act, pin);
	}
	else rccar_read_pixel(win, sty, act, pin);
}
static void rccar_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	u8* p;
	short* t;
	if(len > 0)
	{
		p = (void*)ev;
		switch(p[0])
		{
			case 'a':boardwrite(_car_, 0, "l", 0);break;
			case 'd':boardwrite(_car_, 0, "r", 0);break;
			case 's':boardwrite(_car_, 0, "n", 0);break;
			case 'w':boardwrite(_car_, 0, "f", 0);break;
			default:boardwrite(_car_, 0, " ", 0);
		}
		return;
	}

	if(_char_ == ev->what)
	{
		switch(ev->why)
		{
			case 'a':boardwrite(_car_, 0, "l", 0);break;
			case 'd':boardwrite(_car_, 0, "r", 0);break;
			case 's':boardwrite(_car_, 0, "n", 0);break;
			case 'w':boardwrite(_car_, 0, "f", 0);break;
			default:boardwrite(_car_, 0, " ", 0);
		}
		return;
	}

	if(joy_event == (ev->what & 0xff))
	{
		if(joy_left == (ev->what & joy_mask))
		{
			t = (void*)ev;
			if(t[3] & joyl_left)		//x-
			{
				boardwrite(_car_, 0, "l", 0);
				return;
			}
			if(t[3] & joyl_right)		//x+
			{
				boardwrite(_car_, 0, "r", 0);
				return;
			}
			if(t[3] & joyl_down)		//y-
			{
				boardwrite(_car_, 0, "n", 0);
				return;
			}
			if(t[3] & joyl_up)			//y+
			{
				boardwrite(_car_, 0, "f", 0);
				return;
			}
			boardwrite(_car_, 0, " ", 0);
		}
	}
}
static void rccar_get()
{
}
static void rccar_post()
{
}
static void rccar_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void rccar_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void rccar_delete(struct actor* act)
{
	if(0 == act)return;
	boardwrite(_car_, 0, "-", 0);
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void rccar_create(struct actor* act)
{
	if(0 == act)return;
	boardwrite(_car_, 0, "+", 0);
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void rccar_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('r', 'c', 'c', 'a', 'r', 0, 0, 0);

	p->oncreate = (void*)rccar_create;
	p->ondelete = (void*)rccar_delete;
	p->onstart  = (void*)rccar_start;
	p->onstop   = (void*)rccar_stop;
	p->onget    = (void*)rccar_get;
	p->onpost   = (void*)rccar_post;
	p->onread   = (void*)rccar_read;
	p->onwrite  = (void*)rccar_write;
}
