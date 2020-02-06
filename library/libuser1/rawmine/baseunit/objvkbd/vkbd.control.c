#include "libuser.h"
int boardread( int,int,void*,int);
int boardwrite(int,int,void*,int);




static char statusbuffer[64] = {
't',  0 , 'b', 0, 0, 'b',  0 , 't',
 0 , 'f',  0 , 0, 0,  0 , 'y',  0 ,
'l', '-', 'r', 0, 0, 'x', '+', 'b',
 0 , 'n',  0 , 0, 0,  0 , 'a',  0
};




static void control_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u8* p;
	int x,y;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

	p = act->buf0;
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
static void control_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y;
	u8* p;
	vec3 tc,tr,tf,tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	gl41line_rect(win, 0xffffff, vc, vr, vf);

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
	gl41solid_cylinder(win, 0xc0c0c0, tc, tr, tf, tu);

	//tc = vc+vr/2-vf/2
	tc[0] = vc[0] + vr[0]/2 - vf[0]/2;
	tc[1] = vc[1] + vr[1]/2 - vf[1]/2;
	tc[2] = vc[2] + vr[2]/2 - vf[2]/2;
	gl41solid_cylinder(win, 0xc0c0c0, tc, tr, tf, tu);

	//tr = vr/8
	tr[0] = vr[0]/8;
	tr[1] = vr[1]/8;
	tr[2] = vr[2]/8;

	//tf = vf/8
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;

	p = act->buf0;
	for(y=0;y<4;y++)
	{
		for(x=0;x<8;x++)
		{
			if(0 == p[y*8 + x])continue;

			//tc = vc + (2*x-7)*vr/8 + (7-2*y)*vf/8
			tc[0] = vc[0] + (2*x-7)*vr[0]/8 + (7-2*y)*vf[0]/8;
			tc[1] = vc[1] + (2*x-7)*vr[1]/8 + (7-2*y)*vf[1]/8;
			tc[2] = vc[2] + (2*x-7)*vr[2]/8 + (7-2*y)*vf[2]/8;
			gl41solid_cylinder(win, 0xc0c0c0, tc, tr, tf, tu);

			tc[0] += tr[0]/2 + tu[0]*2;
			tc[1] += tr[1]/2 + tu[1]*2;
			tc[2] += tr[2]/2 + tu[2]*2;
			carveascii(win, 0xff00ff, tc, tr, tf, p[y*8+x]);
		}
	}
}
static void control_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void control_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void control_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void control_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void control_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	void* dc;
	void* df;
	struct relation* irel;
	if((_char_ != ev->what)&&(_kbd_ != ev->what))return;
	say("%x\n", ev->why);
}




static void control_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void control_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void control_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void control_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void control_search(struct entity* act)
{
}
static void control_modify(struct entity* act)
{
}
static void control_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->buf0){
		memorydelete(act->buf0);
		act->buf0 = 0;
	}
}
static void control_create(struct entity* act)
{
	int j;
	u8* p;
	if(0 == act)return;

	p = memorycreate(256, 0);
	if(0 == p)return;

	for(j=0;j<32;j++)p[j] = statusbuffer[j];
	act->buf0 = p;
}




void control_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('c', 'o', 'n', 't', 'r', 'o', 'l', 0);

	p->oncreate = (void*)control_create;
	p->ondelete = (void*)control_delete;
	p->onsearch = (void*)control_search;
	p->onmodify = (void*)control_modify;

	p->onlinkup = (void*)control_linkup;
	p->ondiscon = (void*)control_discon;
	p->onread  = (void*)control_read;
	p->onwrite = (void*)control_write;
}
