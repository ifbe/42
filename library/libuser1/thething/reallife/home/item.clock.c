#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void clock_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int j,rad;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
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

	if(ww < hh)rad = ww;
	else rad = hh;
	drawline_rect(win, 0xffffff, cx-ww,cy-hh, cx+ww,cy+hh);
	drawsolid_circle(win, 0x101010, cx, cy, rad);
	drawline_circle(win, 0xffffff, cx, cy, rad);

	//year
	drawdec8(win, 0xffffff, cx-8-48, cy-16, p[6]);
	drawdec8(win, 0xffffff, cx-8-32, cy-16, p[5]);
	//month
	drawdec8(win, 0xffffff, cx-8   , cy-16, p[4]);
	//day
	drawdec8(win, 0xffffff, cx-8+32, cy-16, p[3]);
	//hour
	drawdec8(win, 0xffffff, cx-8-32, cy, p[2]);
	//minute
	drawdec8(win, 0xffffff, cx-8   , cy, p[1]);
	//second
	drawdec8(win, 0xffffff, cx-8+32, cy, p[0]);
}
static void clock_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	u8 j;
	float a,c,s;
	vec3 tc, tr, tf, tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41solid_circle(ctx, 0x404040, vc, vr, vf);

	u64 date = dateread();
	u8* p = (u8*)&date;
	p[2] += 8;
	if(p[2] >= 24)p[2] = 0;p[3] += 1;

	a = PI/2 - (p[0]*PI*2.0/60.0);
	c = getcos(a);
	s = getsin(a);
	tr[0] = vc[0]+(vr[0]*c+vf[0]*s);
	tr[1] = vc[1]+(vr[1]*c+vf[1]*s);
	tr[2] = vc[2]+(vr[2]*c+vf[2]*s);
	gl41line(ctx, 0xff0000, vc, tr);

	a = PI/2 - (p[1]*PI*2.0/60.0);
	c = getcos(a);
	s = getsin(a);
	tr[0] = vc[0]+(vr[0]*c+vf[0]*s)*3/4;
	tr[1] = vc[1]+(vr[1]*c+vf[1]*s)*3/4;
	tr[2] = vc[2]+(vr[2]*c+vf[2]*s)*3/4;
	gl41line(ctx, 0x00ff00, vc, tr);

	a = PI/2 - (p[2]*PI*2.0/12.0);
	c = getcos(a);
	s = getsin(a);
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
		c = getcos(a);
		s = getsin(a);
		tc[0] = vc[0] + c*vr[0]*7/8 + s*vf[0]*7/8;
		tc[1] = vc[1] + c*vr[1]*7/8 + s*vf[1]*7/8;
		tc[2] = vc[2] + c*vr[2]*7/8 + s*vf[2]*7/8 + 1;
		gl41ascii_center(ctx, 0xffffff, tc, tr, tf, j<10 ? j+0x30 : j+0x37);
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




static void clock_read_bycam(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->fmt){
	case _dx11list_:
	case _mt20list_:
	case _gl41list_:
	case _vk12list_:
		clock_draw_gl41(ent,slot, wor,geom, wnd,area);
		break;
	}
}
static void clock_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	case _rgba_:
		clock_draw_pixel(ent,foot, wnd,area);
		break;
	default:
		clock_read_bycam(ent,foot, stack,sp, arg,key);
		break;
	}
}
static void clock_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
	p->ontaking = (void*)clock_taking;
	p->ongiving = (void*)clock_giving;
}
