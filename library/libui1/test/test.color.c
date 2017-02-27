#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void hexadecimal(int x,int y,u64 in);
//
void printmemory(char*,int);
void say(char*,...);




struct player
{
        u64 type;
        u64 name;
        u8 temp[0x30];

        u64 create;
        u64 delete;
        u64 start;
        u64 stop;
        u64 list;
        u64 choose;
        u64 read;
        u64 write;
};
struct window
{
        u64 buf;
        u64 fmt;
        u64 w;
        u64 h;
};
struct event
{
        u64 why;
        u64 what;
        u64 where;
        u64 when;
};
static int red=0x8d,green=0x63,blue=0x25;




static void color_read_pixel(struct window* win)
{
	int x,y,w,h,min;
	u32 color;
	u32* screenbuf;

	screenbuf = (u32*)(win->buf);
	w = win->w;
	h = win->h;

	if(w < h)min = w;
	else min = h;

	//(左边)各种颜色的色板
	if( ((win->fmt)&0xffffffff) == 0x61626772)
	{
		for(y=0;y<min;y++)
		{
			for(x=0;x<min;x++)
			{
				color = 0xff000000
					+ ( ( (x*256) / min ) << 16)
					+ ( ( (y*256) / min ) << 8 )
					+ red;
				screenbuf[w*y + x] = color;
			}
		}

		color=0xff000000+ (blue<<16) + (green<<8) + red;
	}
	else
	{
		for(y=0;y<min;y++)
		{
			for(x=0;x<min;x++)
			{
				color = 0xff000000
					+ (red<<16)
					+ ( ( (y*256) / min ) << 8 )
					+ ( (x*256) / min );
				screenbuf[w*y + x] = color;
			}
		}

		color=0xff000000+ (red<<16) + (green<<8) + blue;
	}

	//(右边)选中的颜色的方块
	if(w < h)
	{
		for(y=w;y<h;y++)
		{
			for(x=0;x<w;x++)
			{
				screenbuf[w*y + x] = color;
			}
		}
	}
	else
	{
		for(y=0;y<h;y++)
		{
			for(x=h;x<w;x++)
			{
				screenbuf[w*y + x] = color;
			}
		}
	}

	//(白点)被选中的颜色
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			screenbuf[w*(green*min/256+y) + (blue*min/256+x) ]
				= 0xffffffff;
		}
	}

	//
	hexadecimal(0, 0, (red<<16) + (green<<8) + blue);
}
static void color_read_html(struct window* win)
{
	u32* buf = (u32*)(win->buf);
	color_read_pixel(win);
	buf[0]=0;
}
static void color_read_text(struct window* win)
{
}
static void color_read(struct window* win)
{
	u32 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		color_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		color_read_html(win);
	}

	//pixel
	else
	{
		color_read_pixel(win);
	}
}
static void color_write(struct event* ev)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(type==0x64626b)
	{
		if(key==0x25)		//left
		{
			if(blue>0) blue--;
		}
		if(key==0x27)		//right
		{
			if(blue<255) blue++;
		}
		if(key==0x28)		//down
		{
			if(green<255) green++;
		}
		if(key==0x26)		//up
		{
			if(green>0) green--;
		}
	}
	else if(type==0x72616863)
	{
		if(key=='+')
		{
			if(red<255) red++;
		}
		if(key=='-')
		{
			if(red>0) red--;
		}
	}
	else if(type == 0x2d6d)
	{
		if((key>>48) == 1)	//left
		{
			int x = key&0xffff;
			int y = (key>>16)&0xffff;
			int min = 512;

			if((x>min)|(y>min))return;
			blue=x*256/min;
			green=y*256/min;
		}
		else if((key>>48) == 4)
		{
			if(red<0xff)red++;
		}
		else if((key>>48) == 5)
		{
			if(red>0)red--;
		}
	}
}




static void color_list()
{
}
static void color_into()
{
}
static void color_start()
{
}
static void color_stop()
{
}
void color_create(void* base, void* addr)
{
	struct player* p = addr;

	p->type = 0x74736574;
	p->name = 0x726f6c6f63;

	p->start = (u64)color_start;
	p->stop = (u64)color_stop;
	p->list = (u64)color_list;
	p->choose = (u64)color_into;
	p->read = (u64)color_read;
	p->write = (u64)color_write;
}
void color_delete()
{
}

