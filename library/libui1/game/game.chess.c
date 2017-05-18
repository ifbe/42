#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void rectbody(void*,
        int x1, int y1, int x2, int y2, u32 color);
//
int printmemory(void*, int);
int fmt(void*, int, void*, ...);
int say(void*, ...);




struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
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

	//z,y,x
	u8 data[3][8][8];
};
static struct player* pl;




static void chess_read_pixel(struct window* win)
{
	u32 color;
	int x,y;
	int w = win->w;
	int h = win->h;
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			if(((x+y+32)%2) == 0)color = 0;
			else color = 0xffffff;

			rectbody(win,
				x*w/8, y*h/8,
				(x+1)*w/8, (y+1)*h/8,
				color
			);
		}
	}
}
static void chess_read_html(struct window* win)
{
}
static void chess_read_text(struct window* win)
{
}
static void chess_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text 
	if(fmt == 0x74786574)
	{
		chess_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		chess_read_html(win);
	}

	//pixel
	else
	{
		chess_read_pixel(win);
	}
}
static void chess_write(struct event* ev)
{
}




static void chess_list()
{
}
static void chess_choose()
{
}
static void chess_start()
{
}
static void chess_stop()
{
}
void chess_create(void* base, void* addr)
{
	pl = addr;

	pl->type = 0x656d6167;
	pl->name = 0x7373656863;
	pl->start = (u64)chess_start;
	pl->stop = (u64)chess_stop;
	pl->list = (u64)chess_list;
	pl->choose = (u64)chess_choose;
	pl->read = (u64)chess_read;
	pl->write = (u64)chess_write;
}
void chess_delete()
{
}
