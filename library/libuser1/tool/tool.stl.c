#include "actor.h"
int openreadclose(void*, void*, u64, u64);
int openwriteclose(void*, void*, u64, u64);
int windowread(int type, void* buf);
int windowwrite(int type, void* buf);
void carvestl(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float sx, float sy, float sz,
	void* stlbuf, int stllen, float f);




static u8* stlbuf;
static int stllen;
static float left,right,front,back,bottom,upper;
void stl_prep(void* name)
{
	float* p;
	int j,ret;
	
	stllen = openreadclose(name, stlbuf, 0, 0x800000);
	say("len=%x, count=%x, ", stllen, *(u32*)(stlbuf+80));

	left = back = bottom = 100000.0;
	right = front = upper = -100000.0;

	ret = *(u32*)(stlbuf+80);
	ret = ret%(0x200000/36);
	for(j=0;j<ret;j++)
	{
		p = (void*)stlbuf + 84 + j*50;

		if(p[3] < left)left = p[3];
		if(p[3] > right)right = p[3];
		if(p[4] < back)back = p[4];
		if(p[4] > front)front = p[4];
		if(p[5] < bottom)bottom = p[5];
		if(p[5] > upper)upper = p[5];

		if(p[6] < left)left = p[6];
		if(p[6] > right)right = p[6];
		if(p[7] < back)back = p[7];
		if(p[7] > front)front = p[7];
		if(p[8] < bottom)bottom = p[8];
		if(p[8] > upper)upper = p[8];

		if(p[9] < left)left = p[9];
		if(p[9] > right)right = p[9];
		if(p[10] < back)back = p[10];
		if(p[10] > front)front = p[10];
		if(p[11] < bottom)bottom = p[11];
		if(p[11] > upper)upper = p[11];
	}
	say("%f,%f,%f,%f,%f,%f\n",left,right,back,front,bottom,upper);
}




static void stl_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void stl_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* p;
	float sx,sy,f;
	float v[3][3];
	int x,y,j,ret;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	sx = (left+right)/2;
	sy = (back+front)/2;

	if(right-left > front-back)f = 2.0*ww/(right-left);
	else f = 2.0*hh/(front-back);

	ret = *(u32*)(stlbuf+80);
	ret = ret % ((0x800000-0x84)/50);
	for(j=0;j<ret;j++)
	{
		p = (void*)stlbuf + 84 + j*50;

		v[0][0] = cx + (p[3]-sx)*f;
		v[0][1] = cy + (p[4]-sy)*f;
		v[0][2] = (p[5]-bottom)*f;
		v[1][0] = cx + (p[6]-sx)*f;
		v[1][1] = cy + (p[7]-sy)*f;
		v[1][2] = (p[8]-bottom)*f;
		v[2][0] = cx + (p[9]-sx)*f;
		v[2][1] = cy + (p[10]-sy)*f;
		v[2][2] = (p[11]-bottom)*f;
/*
		for(y=0;y<3;y++)
		{
			say("%f,%f,%f\n", v[y][0], v[y][1], v[y][2]);
		}
		say("\n");
*/
		drawline(win, 0xffffff, v[0][0], v[0][1], v[1][0], v[1][1]);
		drawline(win, 0xffffff, v[0][0], v[0][1], v[2][0], v[2][1]);
		drawline(win, 0xffffff, v[1][0], v[1][1], v[2][0], v[2][1]);
	}
}
static void stl_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	float f;
	float sx = (left+right)/2;
	float sy = (back+front)/2;
	if(right-left > front-back)f = 2.0*ww/(right-left);
	else f = 2.0*hh/(front-back);

	carvestl(
		win, 0xffffff,
		cx, cy, cz,
		sx, sy, bottom,
		stlbuf, stllen, f
	);
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
	else if(fmt == _vbo_)stl_read_vbo(win, sty, act, pin);
	else if(fmt == _html_)stl_read_html(win, sty, act, pin);
	else stl_read_pixel(win, sty, act, pin);
}




static void stl_write(
	struct actor* act, struct pinid* pin,
	struct event* ev)
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
		stl_prep(buffer);
	}
}




static void stl_list()
{
}
static void stl_change()
{
}
static void stl_stop()
{
}
static void stl_start()
{
	stl_prep("42.stl");
}
static void stl_delete(struct actor* act)
{
	if(0 == act)return;
	stopmemory(act->buf);
}
static void stl_create(struct actor* act)
{
	if(0 == act)return;
	act->buf = startmemory(0x800000);
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
