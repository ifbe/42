#include "actor.h"



//
static int x1=-99,y1=10;
static int x2=25,y2=-77;
static int px=77,py=88;




void doodle_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int w = win->w;
	int h = win->h;
	int cx = (sty->cx) * w / 0x10000;
	int cy = (sty->cy) * h / 0x10000;
	int ww = (sty->wantw) * w / 0x20000;
	int hh = (sty->wanth) * h / 0x20000;

	//rect
	drawsolid_rect(win, 0x00ff00, 
		cx-90, cy-90, cx-10, cy-10);
	drawline_rect(win, 0xff00ff,
		cx+50, cy-30, cx+60, cy-50);

	//circle
	drawsolid_circle(win, 0x0000ff,
		cx-50, cy+40, 40);
	drawline_circle(win, 0xff0000,
		cx+70, cy+80, 40);

	//bezier
	drawline(win, 0x00ffff,
		cx+x1, cy+y1, cx+px, cy+py);
	drawline(win, 0x00ffff,
		cx+x2, cy+y2, cx+px, cy+py);
	drawline_bezier(win, 0xffff,
		cx+x1, cy+y1, cx+x2, cy+y2, cx+px, cy+py);
}
static void doodle_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	float cx = (float)(sty->cx) / 65536.0 - 0.5;
	float cy = (float)(sty->cy) / 65536.0 - 0.5;
	float w = (float)(sty->wantw) / 65536.0;
	float h = (float)(sty->wanth) / 65536.0;

	carvesolid_icosahedron(
		win, 0xffffff,
		cx, cy, 0.0,
		w/2, 0.0, 0.0,
		0.0, w/2, 0.0,
		0.0, 0.0, w/2
	);
}
static void doodle_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void doodle_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void doodle_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("doodle(%x,%x,%x)\n",win,act,sty);
}
static void doodle_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == __cli__)doodle_read_cli(win, act, sty);
	else if(fmt == __tui__)doodle_read_tui(win, act, sty);
	else if(fmt == __html__)doodle_read_html(win, act, sty);
	else if(fmt == __vbo__)doodle_read_vbo(win, act, sty);
	else doodle_read_pixel(win, act, sty);
}
void doodle_write(struct event* ev)
{
	u64 what = ev->what;
	u64 why = ev->why;
	if(what == 0x2d70)
	{
		px = why & 0xffff;
		py = (why >> 16) & 0xffff;
	}
}
void doodle_list()
{
}
void doodle_change()
{
}
void doodle_start()
{
}
void doodle_stop()
{
}
void doodle_create(void* base, void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('d', 'o', 'o', 'd', 'l', 'e', 0, 0);

	p->start = (void*)doodle_start;
	p->stop = (void*)doodle_stop;
	p->list = (void*)doodle_list;
	p->choose = (void*)doodle_change;
	p->read = (void*)doodle_read;
	p->write = (void*)doodle_write;
}
void doodle_delete()
{
}