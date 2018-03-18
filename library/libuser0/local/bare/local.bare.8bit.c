#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include "arena.h"




void windowread()
{
}
void windowwrite(struct window* dst, struct window* src)
{
	int x,y;
	u8* ibuf = src->buf;
	u8* obuf = dst->buf;
	for(x=0;x<320*200;x++)obuf[x] = ibuf[x];
}
void windowlist()
{
}
void windowchoose()
{
}
void windowstart(struct window* this)
{
	if(this->type == hex32('b','u','f',0))
	{
		return;
	}
	else
	{
		this->type = hex32('w','i','n',0);
		this->fmt = hex32('8','b','i','t');

		this->len = 320*200;
		this->buf = (void*)(u64)0xa0000;

		this->width = this->stride = 320;
		this->height = 200;
	}
}
void windowstop()
{
}
void windowcreate()
{
}
void windowdelete()
{
}




void initwindow()
{
}
void freewindow()
{
}
