#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void rectbody(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
void backgroundcolor(void*, u32);
int qrcode_generate(char* src,char* dst,int sidelength);
int diary(char*,int,char*,...);
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
//
static int sidelength;
static char* databuf;




static void qrcode_read_pixel(struct window* win)
{
	u32 color;
	int x,y,x1,y1,x2,y2;
	int width,height,min;

	width = win->w;
	height = win->h;
	if(width < height)min = width;
	else min = height;

	backgroundcolor(win, 0);
	for(y=0;y<sidelength;y++)
	{
		for(x=0;x<sidelength;x++)
		{
			x1=x*min/sidelength;
			y1=y*min/sidelength;
			x2=(x+1)*min/sidelength-1;
			y2=(y+1)*min/sidelength-1;
			if( databuf[(y*sidelength)+x] == 0 )color=0;
			else color=0xffffffff;
//say("%d",databuf[(y*sidelength)+x]);
			rectbody(win,
				x1, y1,
				x2, y2,
				color
			);
		}
//say("\n");
	}
}
static void qrcode_read_html(struct window* win)
{
	int x,y;
	u32 color;
	char* p = (char*)(win->buf);

	*(u32*)p = 0x6c6d7468;
	p += 0x1000;

	p += diary(p, 0x1000, "<div style=\"width:500px;height:500px;background:#fff\">");
	p += diary(
		p, 0x1000,
		"<style type=\"text/css\">"
		".rect{"
		"border:1px solid #000;"
		"position:absolute;"
		"width:%dpx;"
		"height:%dpx;"
		"}"
		"</style>",

		500/sidelength, 500/sidelength
	);
	for(y=0;y<sidelength;y++)
	{
		for(x=0;x<sidelength;x++)
		{
			if( databuf[(y*sidelength)+x] != 0 )continue;

			p += diary(
				p, 0x1000,
				"<div class=\"rect\" style=\""
				"left:%dpx;"
				"top:%dpx;"
				"background:#000;"
				"\"></div>",
				x*500/sidelength, y*500/sidelength
			);
		}
	}
	p += diary(p, 99, "</div>");
}
static void qrcode_read_text(struct window* win)
{
	int x,y;
	int width = win->w;
	int height = win->h;
	u8* p = (u8*)(win->buf);
	for(x=0;x<width*height*4;x++)p[x] = 0;

	for(y=0;y<100;y++)
	{
		if(y >= sidelength)break;
		if(y >= height)break;
		for(x=0;x<100;x++)
		{
			if(x >= sidelength)break;
			if(x >= width/2)break;
			if( databuf[(y*sidelength)+x] != 0 )continue;

			p[( (y*width+x*2)<<2 ) + 3] = 7;
			p[( (y*width+x*2)<<2 ) + 7] = 7;
		}
	}
}
static void qrcode_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		qrcode_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		qrcode_read_html(win);
	}

	//pixel
	else
	{
		qrcode_read_pixel(win);
	}
}
static void qrcode_write(struct event* ev)
{
}




static void qrcode_list()
{
}
static void qrcode_into()
{
}
static void qrcode_start()
{
	sidelength=49;
	qrcode_generate("haha",databuf,sidelength);
}
static void qrcode_stop()
{
}
void qrcode_create(void* base,void* addr)
{
	struct player* p = addr;
	databuf = base+0x300000;

	p->type = 0x6c6f6f74;
	p->name = 0x65646f637271;

	p->start = (u64)qrcode_start;
	p->stop = (u64)qrcode_stop;
	p->list = (u64)qrcode_list;
	p->choose = (u64)qrcode_into;
	p->read = (u64)qrcode_read;
	p->write = (u64)qrcode_write;
}
void qrcode_delete()
{
}
