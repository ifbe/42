/*
1:	add name in library/libui1/makefile
2:	add code in character.c character_create() and character_delete()
3:	replace "example" to "whatname"----->		:9,$s/example/whatname/g
4:	do your logic in this code
*/
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
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




static void example_read_html(struct window* win)
{
}
static void example_read_pixel(struct window* win)
{
}
static void example_read_text(struct window* win)
{
}
static void example_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		example_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		example_read_html(win);
	}

	//pixel
	else
	{
		example_read_pixel(win);
	}
}
static void example_write(struct event* ev)
{
}




static void example_list()
{
}
static void example_change()
{
}
static void example_start()
{
}
static void example_stop()
{
}
void example_create(void* base,void* addr)
{
	struct player* p = addr;

	p->type = 0x74736574;
	p->name = 0x656c706d617865;
	p->start = (u64)example_start;
	p->stop = (u64)example_stop;
	p->list = (u64)example_list;
	p->choose = (u64)example_change;
	p->read = (u64)example_read;
	p->write = (u64)example_write;
}
void example_delete()
{
}
