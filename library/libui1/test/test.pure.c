#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void printhexadecimal(void*,
	int x, int y, int size,
	u64 in, u32 fg, u32 bg);
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
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;

	u8 data[0xe0];
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
static int flag=0;




static void pure_read_pixel(struct window* win)
{
	int x,y,w,h;
	u32 color;
	u32* buf;

	buf = (u32*)(win->buf);
	w = win->w;
	h = win->h;

	color=0xff000000;
	if((flag&0x1) == 0x1)color |= 0xff;
	if((flag&0x2) == 0x2)color |= 0xff00;
	if((flag&0x4) == 0x4)color |= 0xff0000;

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			buf[w*y + x] = color;
		}
	}

	printhexadecimal(win,
		0, 0, 4,
		color & 0xffffff, 0x87654321, 0xfedcba98);
}
static void pure_read_html(struct window* win)
{
	u32* buf = (u32*)(win->buf);
	pure_read_pixel(win);
	buf[0]=0;
}
static void pure_read_text(struct window* win)
{
}
static void pure_read(struct window* win)
{
	u64 fmt = win->fmt;
	//say("(@2048.read)temp=%x\n",temp);

	//text
	if(fmt == 0x74786574)
	{
		pure_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		pure_read_html(win);
	}

	//pixel
	else
	{
		pure_read_pixel(win);
	}
}
static void pure_write(struct event* ev)
{
	u64 what = ev->what;
	if(what==0x2d6d | what==0x64626b)
	{
		flag = (flag+1)&0x7;
	}
}




static void pure_list()
{
}
static void pure_into()
{
}
static void pure_start()
{
}
static void pure_stop()
{
}
void pure_create(void* uibuf,void* addr)
{
	struct player* p = addr;
	p->type = 0x74736574;
	p->name = 0x65727570;

	p->start = (u64)pure_start;
	p->stop = (u64)pure_stop;
	p->list = (u64)pure_list;
	p->choose = (u64)pure_into;
	p->read = (u64)pure_read;
	p->write = (u64)pure_write;
}
void pure_delete()
{
}

