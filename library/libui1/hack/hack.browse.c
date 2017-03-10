#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printstring(void*,
        int x, int y, int size,
        void*, u32 fgcolor, u32 bgcolor);
void backgroundcolor(void*, u32);
//
int net_choose(void*);
void say(char*,...);




struct event
{
        u64 why;
        u64 what;
        u64 where;
        u64 when;
};
struct window
{
        u64 buf;
        u64 fmt;
        u64 w;
        u64 h;

        u8 data[0xe0];
};
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
static struct player* pl;
//
static u64 fd = 0;
static int len;
static u8* srcbuf = 0;
static u8* dstbuf = 0;




static void browse_read_text(struct window* win)
{
}
static void browse_read_html(struct window* win)
{
}
static void browse_read_pixel(struct window* win)
{
	backgroundcolor(win, 0);
	printstring(win, 0, 16, 1, dstbuf, 0xffffffff, 0);
}
static void browse_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		browse_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		browse_read_html(win);
	}

	//pixel
	else
	{
		browse_read_pixel(win);
	}
}
static void browse_write(struct event* ev)
{
#define kbd 0x72616863
#define http 0x70747468
	int j;
	u64 type = ev->what;
	u64 key = ev->why;
	if(type == kbd)
	{
		if(key == 0xd)
		{
			net_choose("http://127.0.0.1:2222/42.html");
		}
	}
	else if(type == http)
	{
		fd = ev->where;
		len = key;
		for(j=0;j<len;j++)dstbuf[j]=srcbuf[j];
	}
}




static void browse_list()
{
}
static void browse_change()
{
}
static void browse_start()
{
}
static void browse_stop()
{
}
void browse_create(void* base,void* addr)
{
	srcbuf = base-0x100000;
	dstbuf = base+0x300000;
	pl = addr;

	pl->type = 0x74736574;
	pl->name = 0x6573776f7262;
	pl->start = (u64)browse_start;
	pl->stop = (u64)browse_stop;
	pl->list = (u64)browse_list;
	pl->choose = (u64)browse_change;
	pl->read = (u64)browse_read;
	pl->write = (u64)browse_write;
}
void browse_delete()
{
}
