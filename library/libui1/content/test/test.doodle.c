#include "actor.h"
//
void drawline(void*,
	int ax, int ay, int bx, int by, u32 color);
void drawbezier(void*,
	int ax, int ay, int bx, int by, int cx, int cy, u32 color);
void drawrect_body(void*,
	int x1, int y1, int x2, int y2, u32 color);
void drawrect_frame(void*,
	int x1, int y1, int x2, int y2, u32 color);
void drawcircle_body(void*,
	int cx, int cy, int r, u32 color);
void drawcircle_frame(void*,
	int cx, int cy, int r, u32 color);



//
static int px=0,py=0;




void doodle_read(struct arena* win)
{
	backgroundcolor(win, 0);

	//rect
	drawrect_body(win,
	10, 10, 90, 90, 0xff00);
	drawrect_frame(win,
	110, 10, 190,90, 0xff00ff);

	//circle
	drawcircle_body(win,
	50, 150, 40, 0xff);
	drawcircle_frame(win,
	150, 150, 40, 0xff0000);

	//moon
	drawcircle_body(win,
	50, 250, 40, 0xffff00);
	drawcircle_body(win,
	40, 240, 40, 0);

	//bezier
	drawline(win,
	0, 256,  px,  py, 0xffff);
	drawline(win,
	512, 256,  px,  py, 0xffff);
	drawbezier(win,
	0, 256, 512, 256, px, py, 0xffff);
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
