#include "libuser.h"




void draw_pwm(struct arena* win, u32 rgb,
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
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int j;
	int x0,x1,x2,x3,y0,y1;
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
static void pwmtool_draw_vbo(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
}
static void pwmtool_draw_json(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void pwmtool_draw_html(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"pwmtool\" style=\"width:50%%;height:100px;float:left;background-color:#e8194a;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void pwmtool_draw_tui(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void pwmtool_draw_cli(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	say("pwmtool(%x,%x,%x)\n",win,act,sty);
}
static void pwmtool_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)pwmtool_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)pwmtool_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)pwmtool_draw_html(act, pin, win, sty);
	else if(fmt == _json_)pwmtool_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)pwmtool_draw_vbo(act, pin, win, sty);
	else pwmtool_draw_pixel(act, pin, win, sty);
}




static void pwmtool_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	pwmtool_draw(act, pin, win, sty);
}
static void pwmtool_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void pwmtool_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void pwmtool_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void pwmtool_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void pwmtool_start(struct halfrel* self, struct halfrel* peer)
{
}
static void pwmtool_delete(struct actor* act)
{
	if(0 == act)return;
	if(act->buf)
	{
		//memorydelete(act->buf);
		act->buf = 0;
	}
}
static void pwmtool_create(struct actor* act)
{
	if(0 == act)return;
	//act->buf = memorycreate();
}




void pwmtool_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'w', 'm', 't', 'o', 'o', 'l', 0);

	p->oncreate = (void*)pwmtool_create;
	p->ondelete = (void*)pwmtool_delete;
	p->onstart  = (void*)pwmtool_start;
	p->onstop   = (void*)pwmtool_stop;
	p->oncread  = (void*)pwmtool_cread;
	p->oncwrite = (void*)pwmtool_cwrite;
	p->onsread  = (void*)pwmtool_sread;
	p->onswrite = (void*)pwmtool_swrite;
}
