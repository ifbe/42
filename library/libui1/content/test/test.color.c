#include "actor.h"
void drawhexadecimal(
	void*, u64 in, int size,
	int x, int y, u32 fg, u32 bg);




static int red=0x8d,green=0x63,blue=0x25;




static void color_read_pixel(struct arena* win)
{
	int x,y,w,h,min;
	u32 color;
	u32* buf = (u32*)(win->buf);
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
				buf[w*y + x] = color;
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
				buf[w*y + x] = color;
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
				buf[w*y + x] = color;
			}
		}
	}
	else
	{
		for(y=0;y<h;y++)
		{
			for(x=h;x<w;x++)
			{
				buf[w*y + x] = color;
			}
		}
	}

	//(白点)被选中的颜色
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			buf[w*(green*min/256+y) + (blue*min/256+x) ]
				= 0xffffffff;
		}
	}

	//
	drawhexadecimal(
		win, (red<<16) + (green<<8) + blue, 1,
		0, 0, 0xffffffff, 0
	);
}
static void color_read_html(struct arena* win)
{
	u32* buf = (u32*)(win->buf);
	color_read_pixel(win);
	buf[0]=0;
}
static void color_read_text(struct arena* win)
{
}
static void color_read(struct arena* win)
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
	else if(type == 0x2d70)
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
	struct actor* p = addr;
	p->type = hexof('t','e','s','t',0,0,0,0);
	p->name = hexof('c','o','l','o','r',0,0,0);

	p->start = (void*)color_start;
	p->stop = (void*)color_stop;
	p->list = (void*)color_list;
	p->choose = (void*)color_into;
	p->read = (void*)color_read;
	p->write = (void*)color_write;
}
void color_delete()
{
}

