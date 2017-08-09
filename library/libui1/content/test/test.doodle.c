#include "actor.h"
//
void line(void*,
	int ax, int ay, int bx, int by, u32 color);
void bezier(void*,
	int ax, int ay, int bx, int by, int cx, int cy, u32 color);
void rectbody(void*,
	int x1, int y1, int x2, int y2, u32 color);
void rectframe(void*,
	int x1, int y1, int x2, int y2, u32 color);
void circlebody(void*,
	int cx, int cy, int r, u32 color);
void circleframe(void*,
	int cx, int cy, int r, u32 color);
void sectorbody(void*,
	int cx, int cy, int r, int start, int end, u32 color);
void sectorframe(void*,
	int cx, int cy, int r, int start, int end, u32 color);
void backgroundcolor(void*,
	u32);



//
static int px=0,py=0;




void doodle_read(struct arena* win)
{
	backgroundcolor(win, 0);

	//rect
	rectbody(win,
	10, 10, 90, 90, 0xff00);
	rectframe(win,
	110, 10, 190,90, 0xff00ff);

	//circle
	circlebody(win,
	50, 150, 40, 0xff);
	circleframe(win,
	150, 150, 40, 0xff0000);

	//moon
	circlebody(win,
	50, 250, 40, 0xffff00);
	circlebody(win,
	40, 240, 40, 0);

	//taiji
	sectorbody(win,
	150, 250, 50,  90, 270, 0xffffff);
	sectorbody(win,
	150, 250, 50, 270,  90, 0);
	sectorbody(win,
	150, 225, 25,  90, 270, 0);
	sectorbody(win,
	150, 275, 25, 270,  90, 0xffffff);

	//bezier
	line(win,
	0, 256,  px,  py, 0xffff);
	line(win,
	512, 256,  px,  py, 0xffff);
	bezier(win,
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
