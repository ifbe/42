#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void clock_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u32 c[7]={0xff,0xff00,0xffff,0xff0000,0xff00ff,0xffff00,0xffffff};
	int j,k;
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
	u64 date = dateread();
	u8* p = (u8*)&date;

	if(ww < hh)j = ww;
	else j = hh;
	drawsolid_circle(win, 0x222222, cx, cy, j);
	for(j=6;j>=0;j--)
	{
		drawdecimal(win, c[j], cx+64-(j*24), cy-8, p[j]);
	}
}
static void clock_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	u8 j;
	float a,c,s;
	vec3 tc, tr, tf, tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	gl41solid_circle(ctx, 0x404040, vc, vr, vf);

	u64 date = dateread();
	u8* p = (u8*)&date;
	p[2] += 8;
	if(p[2] >= 24)p[2] = 0;p[3] += 1;

	a = PI/2 - (p[0]*PI*2.0/60.0);
	c = cosine(a);
	s = sine(a);
	tr[0] = vc[0]+(vr[0]*c+vf[0]*s);
	tr[1] = vc[1]+(vr[1]*c+vf[1]*s);
	tr[2] = vc[2]+(vr[2]*c+vf[2]*s);
	gl41line(ctx, 0xff0000, vc, tr);

	a = PI/2 - (p[1]*PI*2.0/60.0);
	c = cosine(a);
	s = sine(a);
	tr[0] = vc[0]+(vr[0]*c+vf[0]*s)*3/4;
	tr[1] = vc[1]+(vr[1]*c+vf[1]*s)*3/4;
	tr[2] = vc[2]+(vr[2]*c+vf[2]*s)*3/4;
	gl41line(ctx, 0x00ff00, vc, tr);

	a = PI/2 - (p[2]*PI*2.0/12.0);
	c = cosine(a);
	s = sine(a);
	tr[0] = vc[0]+(vr[0]*c+vf[0]*s)*2/4;
	tr[1] = vc[1]+(vr[1]*c+vf[1]*s)*2/4;
	tr[2] = vc[2]+(vr[2]*c+vf[2]*s)*2/4;
	gl41line(ctx, 0x0000ff, vc, tr);

	tr[0] = vr[0]/8;
	tr[1] = vr[1]/8;
	tr[2] = vr[2]/8;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	for(j=0;j<12;j++)
	{
		a = PI/2 - j*PI/6;
		c = cosine(a);
		s = sine(a);
		tc[0] = vc[0] + c*vr[0]*7/8 + s*vf[0]*7/8;
		tc[1] = vc[1] + c*vr[1]*7/8 + s*vf[1]*7/8;
		tc[2] = vc[2] + c*vr[2]*7/8 + s*vf[2]*7/8 + 1;
		carveascii_center(ctx, 0xffffff, tc, tr, tf, j<10 ? j+0x30 : j+0x37);
	}
}
static void clock_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void clock_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"clock\" style=\"width:50%%;height:100px;float:left;background-color:#7c89da;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void clock_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void clock_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 date = dateread();
	u8* p = (u8*)&date;

	say(	"_%02d_%02d_%02d_%02d_%02d_%02d_%02d\n",
		p[6],p[5],p[4],p[3],p[2],p[1],p[0]);
}
static void clock_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)clock_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)clock_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)clock_draw_html(act, pin, win, sty);
	else if(fmt == _json_)clock_draw_json(act, pin, win, sty);
	else clock_draw_pixel(act, pin, win, sty);
}




static void clock_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;

	//scene -> terminal
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)clock_draw_gl41(act,slot, scn,geom, wnd,area);
	}
}
static void clock_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void clock_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void clock_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void clock_search(struct entity* act)
{
}
static void clock_modify(struct entity* act)
{
}
static void clock_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void clock_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void clock_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('c', 'l', 'o', 'c', 'k', 0, 0, 0);

	p->oncreate = (void*)clock_create;
	p->ondelete = (void*)clock_delete;
	p->onsearch = (void*)clock_search;
	p->onmodify = (void*)clock_modify;

	p->onlinkup = (void*)clock_linkup;
	p->ondiscon = (void*)clock_discon;
	p->onread  = (void*)clock_read;
	p->onwrite = (void*)clock_write;
}
