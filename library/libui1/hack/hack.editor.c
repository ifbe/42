#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
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




static void editor_read_text(struct window* win)
{
}
static void editor_read_html(struct window* win)
{
}
static void editor_read_pixel(struct window* win)
{
}
static void editor_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		editor_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		editor_read_html(win);
	}

	//pixel
	else
	{
		editor_read_pixel(win);
	}
}
static void editor_write(struct event* ev)
{
}
static void editor_list()
{
}
static void editor_choose()
{
}
static void editor_start()
{
}
static void editor_stop()
{
}
void editor_create(void* base,void* addr)
{
	struct player* p = addr;

	p->type = 0x6c6f6f74;
	p->name = 0x726f74696465;
	p->start = (u64)editor_start;
	p->stop = (u64)editor_stop;
	p->list = (u64)editor_list;
	p->choose = (u64)editor_choose;
	p->read = (u64)editor_read;
	p->write = (u64)editor_write;
}
void editor_delete()
{
}
