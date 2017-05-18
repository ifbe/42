#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printstring(void*,
	int x, int y, int size,
	char* str, u32 fg, u32 bg);
void rect(void*,
	int x0, int y0,
	int x1, int y1,
	u32 bc, u32 fc);
u32 getrandom();




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




static void roster_read(struct window* win)
{
	int x,y;
	int width = win->w;
	int height = win->h;

	rect(win,
		0, 0,
		width/4, height/4,
		0x554455, 0xff00ff
	);
	printstring(win,
		0, 0, 4,
		"game", 0, 0xffffff
	);

	rect(win,
		width/4, 0,
		width/2, height/4,
		0x998899, 0xff00ff
	);
	printstring(win,
		width/4, 0, 4,
		"test", 0, 0xffffff
	);

	rect(win,
		width/2, 0,
		width*3/4, height/4,
		0x887788, 0xff00ff
	);
	printstring(win,
		width/2, 0, 4,
		"tool", 0, 0xffffff
	);

	rect(win,
		width*3/4, 0,
		width, height/4,
		0xccbbcc, 0xff00ff
	);
	printstring(win,
		width*3/4, 0, 4,
		"haha", 0, 0xffffff
	);
}
static void roster_write(struct event* ev)
{
}
static void roster_into()
{
}
static void roster_list()
{
}
static void roster_start()
{
}
static void roster_stop()
{
}
void roster_create(void* base, void* addr)
{
	struct player* p = addr;

	p->type = 0;
	p->name = 0x726574736f72;

	p->start = (u64)roster_start;
	p->stop = (u64)roster_stop;
	p->list = (u64)roster_list;
	p->choose = (u64)roster_into;
	p->read = (u64)roster_read;
	p->write = (u64)roster_write;
}
void roster_delete()
{
}
