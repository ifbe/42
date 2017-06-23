#include "actor.h"
void yuyv2rgba(
	u8* src, int w1, int h1, 
	u8* dst, int w2, int h2
);
void startvision();
void stopvision();




static u8* vision = 0;




void camera_read_text(struct arena* win)
{
}
void camera_read_html(struct arena* win)
{
}
void camera_read_pixel(struct arena* win)
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
static void camera_read(struct arena* win)
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
	struct actor* p = addr;
	p->type = hexof('t','o','o','l',0,0,0,0);
	p->name = hexof('c','a','m','e','r','a',0,0);

	p->start = (void*)camera_start;
	p->stop = (void*)camera_stop;
	p->list = (void*)camera_list;
	p->choose = (void*)camera_into;
	p->read = (void*)camera_read;
	p->write = (void*)camera_write;
}
void camera_delete()
{
}
