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
	u32* tmp;
	this->type = hex32('w','i','n',0);
	this->fmt = hex64('b','g','r','a','8','8','8','8');

	this->width = this->stride = 1024;
	this->height = 768;

#define screeninfo 0x2000
	tmp = *(u32*)screeninfo;
	this->buf = (void*)tmp;
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
