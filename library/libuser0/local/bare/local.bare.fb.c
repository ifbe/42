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
	int j;
	u32* ibuf = src->buf;
	u32* obuf = dst->buf;
	for(j=0;j<1024*768;j++)
	{
		obuf = (void*)obuf + 3;
		*obuf = ibuf[j];
	}
}
void windowlist()
{
}
void windowchoose()
{
}
void windowstart(struct window* this)
{
	int j;
	u64 tmp;
	this->w = 1024;
	this->h = 768;
	if(this->type == hex32('b','u','f',0))
	{
	}
	else
	{
		this->type = hex32('w','i','n',0);
		this->fmt = hex64('b','g','r','a','8','8','8','8');

#define screeninfo 0x2000
		tmp = *(u32*)screeninfo;
		this->buf = (void*)tmp;
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
