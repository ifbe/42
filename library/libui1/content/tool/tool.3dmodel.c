#include "actor.h"
void* startmemory(int);
void carvesphere(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz);




static char* buf;




static void stl_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
}
static void stl_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void stl_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
}
static void stl_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	float cx = (float)(sty->cx) / 65536.0 - 0.5;
	float cy = (float)(sty->cy) / 65536.0 - 0.5;
	float w = (float)(sty->wantw) / 65536.0;
	float h = (float)(sty->wanth) / 65536.0;
	float d = (w+h)/2;

	carvesphere(
		win, 0xffffff,
		cx, cy, d/2,
		d/2, 0.0, 0.0,
		0.0, 0.0, d/2
	);
}
static void stl_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	//cli
	if(fmt == hex32('c','l','i',0))stl_read_cli(win, act, sty);

	//html
	else if(fmt == hex32('h','t','m','l'))stl_read_html(win, act, sty);

	//vbo
	else if(fmt == hex32('v','b','o',0))stl_read_vbo(win, act, sty);

	//pixel
	else stl_read_pixel(win, act, sty);
}




static void stl_write(struct event* ev)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == 0x2d70)
	{
		int x = key&0xffff;
		int y = (key>>16)&0xffff;
	}
	if(type == 0x72616863)
	{
	}
}




static void stl_list()
{
}
static void stl_change()
{
}
static void stl_start()
{
	buf = (void*)startmemory(0x800000);
	readfile("42.stl", buf, 0, 0x800000);
}
static void stl_stop()
{
}
void stl_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex32('s', 't', 'l', 0);

	p->start = (void*)stl_start;
	p->stop = (void*)stl_stop;
	p->list = (void*)stl_list;
	p->choose = (void*)stl_change;
	p->read = (void*)stl_read;
	p->write = (void*)stl_write;
}
void stl_delete()
{
}
