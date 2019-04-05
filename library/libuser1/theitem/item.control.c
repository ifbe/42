#include "libuser.h"
int boardread( int,int,void*,int);
int boardwrite(int,int,void*,int);




static char statusbuffer[64] = {
't',  0 , 'b', 0, 0, 'b',  0 , 't',
 0 , 'f',  0 , 0, 0,  0 , 'y',  0 ,
'l', '-', 'r', 0, 0, 'x', '+', 'b',
 0 , 'n',  0 , 0, 0,  0 , 'a',  0
};




static void control_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u8* p;
	int x,y;
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

	p = act->buf;
	for(y=0;y<4;y++)
	{
		for(x=0;x<8;x++)
		{
			if(0 == p[y*8 + x])continue;

			drawsolid_circle(win, 0xc0c0c0, cx+(x+x-7)*ww/8, cy + (y+y-7)*hh/8, ww/8);
			drawascii(win, 0xff00ff, cx-4 + (x+x-7)*ww/8, cy-8 + (y+y-7)*hh/8, p[y*8+x]);
		}
	}

	drawsolid_circle(win, 0xc0c0c0, cx-ww/2, cy+hh/2, ww/4);
	drawsolid_circle(win, 0xc0c0c0, cx+ww/2, cy+hh/2, ww/4);
}
static void control_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	u8* p;
	vec3 tc,tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carveline_rect(win, 0xffffff, vc, vr, vf);

	//tu = vu/64
	tu[0] = vu[0]/64;
	tu[1] = vu[1]/64;
	tu[2] = vu[2]/64;

	//tr = vr/4
	tr[0] = vr[0]/4;
	tr[1] = vr[1]/4;
	tr[2] = vr[2]/4;

	//tr = vr/4
	tf[0] = vf[0]/4;
	tf[1] = vf[1]/4;
	tf[2] = vf[2]/4;

	//tc = vc-vr/2-vf/2
	tc[0] = vc[0] - vr[0]/2 - vf[0]/2;
	tc[1] = vc[1] - vr[1]/2 - vf[1]/2;
	tc[2] = vc[2] - vr[2]/2 - vf[2]/2;
	carvesolid_cylinder(win, 0xc0c0c0, tc, tr, tf, tu);

	//tc = vc+vr/2-vf/2
	tc[0] = vc[0] + vr[0]/2 - vf[0]/2;
	tc[1] = vc[1] + vr[1]/2 - vf[1]/2;
	tc[2] = vc[2] + vr[2]/2 - vf[2]/2;
	carvesolid_cylinder(win, 0xc0c0c0, tc, tr, tf, tu);

	//tr = vr/8
	tr[0] = vr[0]/8;
	tr[1] = vr[1]/8;
	tr[2] = vr[2]/8;

	//tf = vf/8
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;

	p = act->buf;
	for(y=0;y<4;y++)
	{
		for(x=0;x<8;x++)
		{
			if(0 == p[y*8 + x])continue;

			//tc = vc + (2*x-7)*vr/8 + (7-2*y)*vf/8
			tc[0] = vc[0] + (2*x-7)*vr[0]/8 + (7-2*y)*vf[0]/8;
			tc[1] = vc[1] + (2*x-7)*vr[1]/8 + (7-2*y)*vf[1]/8;
			tc[2] = vc[2] + (2*x-7)*vr[2]/8 + (7-2*y)*vf[2]/8;
			carvesolid_cylinder(win, 0xc0c0c0, tc, tr, tf, tu);

			tc[0] += tr[0]/2 + tu[0]*2;
			tc[1] += tr[1]/2 + tu[1]*2;
			tc[2] += tr[2]/2 + tu[2]*2;
			carveascii(win, 0xff00ff, tc, tr, tf, p[y*8+x]);
		}
	}
}
static void control_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void control_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void control_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void control_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void control_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)control_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)control_read_tui(win, sty, act, pin);
	else if(fmt == _html_)control_read_html(win, sty, act, pin);
	else if(fmt == _json_)control_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)control_read_vbo(win, sty, act, pin);
	else control_read_pixel(win, sty, act, pin);
}
static void control_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	void* dc;
	void* df;
	struct relation* irel;
	if((_char_ != ev->what)&&(_kbd_ != ev->what))return;
	say("%x\n", ev->why);

	irel = act->irel0;
	while(1)
	{
		if(0 == irel)break;

		dc = (void*)(irel->srcchip);
		df = (void*)(irel->srcfoot);
		if(_fd_ == irel->srctype)
		{
			system_leafwrite(dc, df, act, pin, &ev->why, 1);
		}

		irel = samedstnextsrc(irel);
	}
}
static void control_get()
{
}
static void control_post()
{
}
static void control_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void control_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void control_delete(struct actor* act)
{
	if(0 == act)return;
	if(act->buf)memorydelete(act->buf);
}
static void control_create(struct actor* act)
{
	int j;
	u8* p;
	if(0 == act)return;

	p = memorycreate(256);
	if(0 == p)return;

	for(j=0;j<32;j++)p[j] = statusbuffer[j];
	act->buf = p;
}




void control_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('c', 'o', 'n', 't', 'r', 'o', 'l', 0);

	p->oncreate = (void*)control_create;
	p->ondelete = (void*)control_delete;
	p->onstart  = (void*)control_start;
	p->onstop   = (void*)control_stop;
	p->onget    = (void*)control_get;
	p->onpost   = (void*)control_post;
	p->onread   = (void*)control_read;
	p->onwrite  = (void*)control_write;
}
