#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void backgroundcolor(void*,
	u32 color);
void rectbody(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
void rectframe(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
void drawstring(
	void*, void* str, int size,
	int x, int y, u32 fg, u32 bg);
//
void datastr2hexstr(void* dst, void* src, int len);
void md5sum(void*, void*, int);
void sha1sum(void*, void*, int);
//
int readfile(void*, void*, u64, u64);
int writefile(void*, void*, u64, u64);
//
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
	u8 data[0xc0];
};
static struct player* pl;
//
static u64 algtype[] = {
	0x35646d,		//md5
	0x31616873,		//sha1
	0
};
static int this = 0;
//
static u8* buffer;
static int buflen;
//
static u8* key;
static int keylen;
//
static int reslen;




static void algorithm_read_text(struct window* win)
{
}
static void algorithm_read_html(struct window* win)
{
}
static void algorithm_read_pixel(struct window* win)
{
	backgroundcolor(win, 0);

	//top
	rectbody(win,
		(win->w)/16, (win->h)/16,
		(win->w)*15/16, (win->h)*7/16,
		0x40
	);
	drawstring(
		win, "makefile", 1,
		(win->w)/2, (win->h)/4, 0xffffffff, 0
	);

	//middle
	rectbody(win,
		(win->w)/16, (win->h)*7/16,
		(win->w)*15/16, (win->h)*9/16,
		0
	);
	rectframe(win,
		(win->w/4)+32, (win->h/2)-16,
		(win->w*3/4)-32, (win->h/2)+16,
		0xffffffff
	);

	//middle.left
	rectframe(win,
		(win->w/4)-32, (win->h/2)-16,
		(win->w/4)+32, (win->h/2)+16,
		0xffffffff
	);
	drawstring(
		win, &algtype[this], 1,
		(win->w/4)-16, (win->h/2)-4, 0xffffffff, 0
	);

	//middle.right
	rectframe(win,
		(win->w*3/4)-32, (win->h/2)-16,
		(win->w*3/4)+32, (win->h/2)+16,
		0xffffffff
	);
	drawstring(
		win, "doit", 1,
		(win->w*3/4)-16, (win->h/2)-4, 0xffffffff, 0
	);

	//bottom
	rectbody(win,
		(win->w)/16, (win->h)*9/16,
		(win->w)*15/16, (win->h)*15/16,
		0x400000
	);
	drawstring(
		win, pl->data, 1,
		(win->w/2)-(reslen*4), (win->h)*3/4, 0xffffffff, 0
	);
}
static void algorithm_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		algorithm_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		algorithm_read_html(win);
	}

	//pixel
	else
	{
		algorithm_read_pixel(win);
	}
}
static void algorithm_write(struct event* ev)
{
	u64 key = ev->why;
	u64 type = ev->what;
	int x;
	int y;
	int ret;
	int temp[64];

	if(type==0x2d70)
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;
		if(y<0x7000)return;
		if(y>0x9000)return;
		if(x<0x3000)return;
		if(x<0x5000)
		{
			this = (this+1)%2;
			return;
		}
		if(x<0xb000)return;

		int ret = readfile("makefile", buffer, 0, 0x100000);
		if(algtype[this] == 0x35646d)
		{
			md5sum(temp, buffer, ret);
			datastr2hexstr(pl->data, temp, 16);
			pl->data[32] = 0;
			reslen = 32;
		}
		else if(algtype[this] == 0x31616873)
		{
			sha1sum(temp, buffer, ret);
			datastr2hexstr(pl->data, temp, 20);
			pl->data[40] = 0;
			reslen = 40;
		}
	}
}
static void algorithm_list()
{
}
static void algorithm_choose()
{
}
static void algorithm_start()
{
	pl->data[0] = 0;
}
static void algorithm_stop()
{
}
void algorithm_create(void* base, void* addr)
{
	pl = addr;
	buffer = base+0x300000;

	pl->type = 0x656d6167;
	pl->name = 0x6f676c61;
	pl->start = (u64)algorithm_start;
	pl->stop = (u64)algorithm_stop;
	pl->list = (u64)algorithm_list;
	pl->choose = (u64)algorithm_choose;
	pl->read = (u64)algorithm_read;
	pl->write = (u64)algorithm_write;
}
void algorithm_delete()
{
}