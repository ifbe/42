#include "libuser.h"




void draw_pwm(struct entity* win, u32 rgb,
	int x0, int x1, int x2, int x3, int y0, int y1)
{
	//heng
	drawline(win, 0xffffff, x0, y0, x1, y0);
	//shu
	drawline(win, 0xffffff, x1, y0, x1, y1);
	//heng
	drawline(win, 0xffffff, x1, y1, x2, y1);
	//shu
	drawline(win, 0xffffff, x2, y0, x2, y1);
	//heng
	drawline(win, 0xffffff, x2, y0, x3, y0);
}
static void pwmtool_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int j;
	int x0,x1,x2,x3,y0,y1;
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
	drawsolid_rect(win, 0, cx-ww, cy-hh, cx+ww, cy+hh);

	x0 = cx-ww*15/16;
	x1 = x0;
	x2 = cx-ww*15/16/2;
	x3 = cx+ww*15/16;
	y0 = cy-hh/16;
	y1 = cy-hh*15/16;
	draw_pwm(win,0xffffff,x0,x1,x2,x3,y0,y1);

	y0 = cy+hh*6/10;
	y1 = cy+hh*4/10;
	for(j=-2;j<=2;j++)
	{
		x0 = cx+ww*(j*4-2)/10;
		x1 = x0;
		x2 = cx+ww*(j*4-1)/10;
		x3 = cx+ww*(j*4+2)/10;
		draw_pwm(win,0xffffff,x0,x1,x2,x3,y0,y1);
	}
}
static void pwmtool_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	float* vc = sty->fs.vc;
	float* vr = sty->fs.vr;
	float* vf = sty->fs.vf;
	float* vu = sty->fs.vt;
}
static void pwmtool_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pwmtool_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pwmtool_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pwmtool_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("pwmtool(%x,%x,%x)\n",win,act,sty);
}




static void pwmtool_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void pwmtool_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void pwmtool_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void pwmtool_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void pwmtool_search(struct entity* act)
{
}
static void pwmtool_modify(struct entity* act)
{
}
static void pwmtool_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->buf0){
		//memorydelete(act->buf);
		act->buf0 = 0;
	}
}
static void pwmtool_create(struct entity* act)
{
	if(0 == act)return;
	//act->buf = memorycreate();
}




void pwmtool_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'w', 'm', 't', 'o', 'o', 'l', 0);

	p->oncreate = (void*)pwmtool_create;
	p->ondelete = (void*)pwmtool_delete;
	p->onsearch = (void*)pwmtool_search;
	p->onmodify = (void*)pwmtool_modify;

	p->onlinkup = (void*)pwmtool_linkup;
	p->ondiscon = (void*)pwmtool_discon;
	p->ontaking = (void*)pwmtool_taking;
	p->ongiving = (void*)pwmtool_giving;
}
