#include "actor.h"
int openreadclose(void*, u64, void*, u64);
int openwriteclose(void*, u64, void*, u64);
int windowread(int type, void* buf);
int windowwrite(int type, void* buf);
void carvestl(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float sx, float sy, float sz,
	void* stlbuf, int stllen, float f);




void stl_prep(struct actor* act, void* name)
{
	float* p;
	int j,ret;
	
	act->len = openreadclose(name, 0, act->buf, 0x800000);
	say("stllen=%x\n", (act->len));
	if((act->len) <= 0)return;

	act->lx = act->nx = act->bx = 100000.0;
	act->rx = act->fx = act->ux = -100000.0;

	ret = *(u32*)((act->buf)+80);
	say("len=%x, count=%x\n", (act->len), ret);
	ret = ret%(0x200000/36);

	for(j=0;j<ret;j++)
	{
		p = (void*)(act->buf) + 84 + j*50;

		if(p[3] < act->lx)act->lx = p[3];
		if(p[3] > act->rx)act->rx = p[3];
		if(p[4] < act->nx)act->nx = p[4];
		if(p[4] > act->fx)act->fx = p[4];
		if(p[5] < act->bx)act->bx = p[5];
		if(p[5] > act->ux)act->ux = p[5];

		if(p[6] < act->lx)act->lx = p[6];
		if(p[6] > act->rx)act->rx = p[6];
		if(p[7] < act->nx)act->nx = p[7];
		if(p[7] > act->fx)act->fx = p[7];
		if(p[8] < act->bx)act->bx = p[8];
		if(p[8] > act->ux)act->ux = p[8];

		if(p[9] < act->lx)act->lx = p[9];
		if(p[9] > act->rx)act->rx = p[9];
		if(p[10] < act->nx)act->nx = p[10];
		if(p[10] > act->fx)act->fx = p[10];
		if(p[11] < act->bx)act->bx = p[11];
		if(p[11] > act->ux)act->ux = p[11];
	}
	say(
		"l=%f\n"
		"r=%f\n"
		"n=%f\n"
		"f=%f\n"
		"b=%f\n"
		"u=%f\n",
		act->lx,
		act->rx,
		act->nx,
		act->fx,
		act->bx,
		act->ux
	);

	act->cx = ((act->lx) + (act->rx))/2;
	act->cy = ((act->nx) + (act->fx))/2;
	act->cz = ((act->bx) + (act->ux))/2;
	act->width = (act->rx) - (act->lx);
	act->height = (act->fx) - (act->nx);
	act->depth = (act->ux) - (act->bx);
	say(
		"cx=%f\n"
		"cy=%f\n"
		"cz=%f\n"
		"w=%f\n"
		"h=%f\n"
		"d=%f\n",
		act->cx,
		act->cy,
		act->cz,
		act->width,
		act->height,
		act->depth
	);
}




static void stl_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* p;
	float f;
	float v[3][3];
	int j,ret;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;

	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);
	if(0 == (act->buf))return;
	if(0 == (act->len))return;

	if((act->width) > (act->height))f = 2.0*ww/(act->width);
	else f = 2.0*hh/(act->height);

	ret = *(u32*)((act->buf)+80);
	ret = ret % ((0x800000-0x84)/50);
	for(j=0;j<ret;j++)
	{
		p = (void*)(act->buf) + 84 + j*50;

		v[0][0] = cx + (p[ 3]-(act->cx))*f;
		v[0][1] = cy + (p[ 4]-(act->cy))*f;
		v[1][0] = cx + (p[ 6]-(act->cx))*f;
		v[1][1] = cy + (p[ 7]-(act->cy))*f;
		v[2][0] = cx + (p[ 9]-(act->cx))*f;
		v[2][1] = cy + (p[10]-(act->cy))*f;

		drawline(win, 0xffffff, v[0][0], v[0][1], v[1][0], v[1][1]);
		drawline(win, 0xffffff, v[0][0], v[0][1], v[2][0], v[2][1]);
		drawline(win, 0xffffff, v[1][0], v[1][1], v[2][0], v[2][1]);
	}
}
static void stl_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float f;
	float cx = sty->cx;
	float cy = sty->cy;
	float cz = sty->cz;
	float ww = sty->rx;
	float hh = sty->fy;
	float dd = sty->uz;
	if(act->buf == 0)return;
	if(act->len == 0)return;

	if((act->width) > (act->height))f = 2.0*ww/(act->width);
	else f = 2.0*hh/(act->height);
	carvestl(
		win, 0xffffff,
		cx, cy, cz,
		act->cx, act->cy, act->bx,
		(act->buf), (act->len), f
	);
}
static void stl_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void stl_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"stl\" style=\"width:50%%;height:100px;float:left;background-color:#3368a9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void stl_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void stl_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("stl(%x,%x,%x)\n",win,act,sty);
}
static void stl_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)stl_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)stl_read_tui(win, sty, act, pin);
	else if(fmt == _html_)stl_read_html(win, sty, act, pin);
	else if(fmt == _json_)stl_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)stl_read_vbo(win, sty, act, pin);
	else stl_read_pixel(win, sty, act, pin);
}




static void stl_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int j,ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == 0x2d70)
	{
		int x = key&0xffff;
		int y = (key>>16)&0xffff;
	}
	else if(_drop_ == type)
	{
		char buffer[0x1000];
		ret = windowread(type, buffer);
		say("%s", buffer);

		for(j=0;j<ret;j++)
		{
			if(buffer[j] < 0x20)
			{
				buffer[j] = 0;
				break;
			}
		}
		stl_prep(act, buffer);
	}
}




static void stl_list()
{
}
static void stl_change()
{
}
static void stl_stop(struct actor* act, struct pinid* pin)
{
}
static void stl_start(struct actor* act, struct pinid* pin)
{
	stl_prep(act, "42.stl");
}
static void stl_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void stl_create(struct actor* act)
{
	if(0 == act)return;
	act->buf = memorycreate(0x800000);
	act->len = 0;
}




void stl_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex32('s', 't', 'l', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)stl_create;
	p->ondelete = (void*)stl_delete;
	p->onstart  = (void*)stl_start;
	p->onstop   = (void*)stl_stop;
	p->onlist   = (void*)stl_list;
	p->onchoose = (void*)stl_change;
	p->onread   = (void*)stl_read;
	p->onwrite  = (void*)stl_write;
}
