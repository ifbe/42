#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
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
void say(char*,...);




//
struct player
{
	u64 type;
	u64 name;
	u64 start;
	u64 stop;
	u64 list;
	u64 choose;
	u64 read;
	u64 write;

	u8 data[0xc0];
};
struct window
{
	u64 buf1;
	u64 buf2;
	u64 fmt;
	u64 dim;

	u64 w;
	u64 h;
	u64 d;
	u64 t;

	u8 data[0xc0];
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
static int px=0,py=0;




void doodle_read(struct window* win)
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
	if(what == 0x2d6d)
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
	struct player* p = addr;
	p->type = 0x74736574;
	p->name = 0x656c646f6f64;

	p->start = (u64)doodle_start;
	p->stop = (u64)doodle_stop;
	p->list = (u64)doodle_list;
	p->choose = (u64)doodle_change;
	p->read = (u64)doodle_read;
	p->write = (u64)doodle_write;
}
void doodle_delete()
{
}
