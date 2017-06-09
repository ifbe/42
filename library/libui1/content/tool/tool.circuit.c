#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void rectframe(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
void line(void*,
	int x1,int y1,
	int x2,int y2,
	u32 color);
void backgroundcolor(void*, u32);
//
void say(char*,...);




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

	u8 temp[0xc0];
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

	u8 temp[0xc0];
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
//元件
struct cell{
	int type;	//battery, resistor, ...
	int value;	//ohm, volt, ...
	int side1;	//1,2,3, ...
	int side2;	//1,2,3, ...
};

//(batt, 5000mv), (0, 1)
static struct cell c1 = {'V', 5000, 0, 1};
//(resistor, 1000ohm), (1,0)
static struct cell c2 = {'R', 1000, 1, 0};




static void battery(struct window* win, int x, int y, struct cell* c)
{
	line(win, x-16, y-8, x+16, y-8, 0xffffffff);
	line(win, x-8, y+8, x+8, y+8, 0xffffffff);
}
static void resistor(struct window* win, int x, int y, struct cell* c)
{
	rectframe(win, x-8, y-16, x+8, y+16, 0xffffffff);
}
static void autowire(struct window* win, int x1, int y1, int x2, int y2)
{
	int j;
	if(x1<x2)
	{
		j=y1-100;
		if(j > y2-100)j=y2-100;
		line(win, x1, y1, x1, j, 0xffffffff);
		line(win, x2, y2, x2, j, 0xffffffff);
		line(win, x1, j, x2, j, 0xffffffff);
	}
	else
	{
		j=y1+100;
		if(j < y2+100)j=y2+100;
		line(win, x1, y1, x1, j, 0xffffffff);
		line(win, x2, y2, x2, j, 0xffffffff);
		line(win, x1, j, x2, j, 0xffffffff);
	}
}




static void circuit_read_html(struct window* win)
{
}
static void circuit_read_pixel(struct window* win)
{
	int battx = (win->w)/3;
	int batty = (win->h)/2;
	int resx = (win->w)*2/3;
	int resy = (win->h)/2;
	backgroundcolor(win, 0);

	//5v battery
	battery(win, battx, batty, &c1);

	//1000ohm resistor
	resistor(win, resx, resy, &c2);

	//wire
	autowire(win, battx, batty-8, resx, resy-16);
	autowire(win, resx, resy+16, battx, batty+8);
}
static void circuit_read_text(struct window* win)
{
}
static void circuit_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		circuit_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		circuit_read_html(win);
	}

	//pixel
	else
	{
		circuit_read_pixel(win);
	}
}
static void circuit_write(struct event* ev)
{
}




static void circuit_list()
{
}
static void circuit_change()
{
}
static void circuit_start()
{
}
static void circuit_stop()
{
}
void circuit_create(void* base,void* addr)
{
	struct player* p = addr;

	p->type = 0x6c6f6f74;
	p->name = 0x74697563726963;

	p->start = (u64)circuit_start;
	p->stop = (u64)circuit_stop;
	p->list = (u64)circuit_list;
	p->choose = (u64)circuit_change;
	p->read = (u64)circuit_read;
	p->write = (u64)circuit_write;
}
void circuit_delete()
{
}
