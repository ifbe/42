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
static void pwmtool_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j;
	int x0,x1,x2,x3,y0,y1;
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
static void pwmtool_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
}
static void pwmtool_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void pwmtool_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
static void pwmtool_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void pwmtool_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("pwmtool(%x,%x,%x)\n",win,act,sty);
}
static void pwmtool_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)pwmtool_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)pwmtool_read_tui(win, sty, act, pin);
	else if(fmt == _html_)pwmtool_read_html(win, sty, act, pin);
	else if(fmt == _json_)pwmtool_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)pwmtool_read_vbo(win, sty, act, pin);
	else pwmtool_read_pixel(win, sty, act, pin);
}
static void pwmtool_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void pwmtool_get()
{
}
static void pwmtool_post()
{
}
static void pwmtool_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void pwmtool_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
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
	p->name = hex64('p', 'w', 'm', 't', 'o', 'o', 'l', 0);

	p->oncreate = (void*)pwmtool_create;
	p->ondelete = (void*)pwmtool_delete;
	p->onstart  = (void*)pwmtool_start;
	p->onstop   = (void*)pwmtool_stop;
	p->onget    = (void*)pwmtool_get;
	p->onpost   = (void*)pwmtool_post;
	p->onread   = (void*)pwmtool_read;
	p->onwrite  = (void*)pwmtool_write;
}
