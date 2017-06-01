#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void background1(void*);
//
void readfile(void*, void*, int , int);
void writefile(void*, void*, int , int);
//
void printmemory(char*,int);
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
static int lastdim = 2;




static void stl_read_html(struct window* win)
{
}
static void stl_read_text(struct window* win)
{
}
static void stl_read_data(struct window* win)
{
	int j;
	char* p = (void*)(win->buf2);
	if(win->dim == 2)
	{
		background1(win);
	}
	else if( (win->dim == 3)&&(lastdim != 3) )
	{
		readfile("42.stl", p, 0, 0x800000);
	}

	lastdim = win->dim;
}
static void stl_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		stl_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		stl_read_html(win);
	}

	//pixel
	else
	{
		stl_read_data(win);
	}
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
}
static void stl_stop()
{
}
void stl_create(void* base,void* addr)
{
	struct player* p = addr;
	p->type = 0x6c6f6f74;
	p->name = 0x6c7473;
	p->start = (u64)stl_start;
	p->stop = (u64)stl_stop;
	p->list = (u64)stl_list;
	p->choose = (u64)stl_change;
	p->read = (u64)stl_read;
	p->write = (u64)stl_write;
}
void stl_delete()
{
}
