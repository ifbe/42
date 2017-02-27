#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void yuyv2rgba(
	u8* src, int w1, int h1, 
	u8* dst, int w2, int h2
);
//
void startvision();
void stopvision();
//libboot
void printmemory(void*, int);
void say(void*, ...);




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
static u8* vision = 0;








void camera_read_text(struct window* win)
{
}
void camera_read_html(struct window* win)
{
}
void camera_read_pixel(struct window* win)
{
	int j;
	int w = win->w;
	int h = win->h;
	u8* screen = (u8*)(win->buf);
	if(vision == 0)return;

	for(j=0;j<640*480;j++)vision[j*2]=256-vision[j*2];
	yuyv2rgba(
		vision, 640, 480,
		screen, w, h
	);
	vision = 0;
}
static void camera_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		camera_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		camera_read_html(win);
	}

	//pixel
	else
	{
		camera_read_pixel(win);
	}
}
static void camera_write(struct event* ev)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(type==0x72616863)	//'char'
	{
	}
	else if(type=='v')
	{
		vision = (void*)key;
		printmemory(vision+0xfff, 16);
	}
}




static void camera_list()
{
}
static void camera_into()
{
}
static void camera_start()
{
	startvision();
}
static void camera_stop()
{
	stopvision();
}
void camera_create(void* base,void* addr)
{
	struct player* p = addr;

	p->type = 0x6c6f6f74;
	p->name = 0x6172656d6163;

	p->start = (u64)camera_start;
	p->stop = (u64)camera_stop;
	p->list = (u64)camera_list;
	p->choose = (u64)camera_into;
	p->read = (u64)camera_read;
	p->write = (u64)camera_write;
}
void camera_delete()
{
}
