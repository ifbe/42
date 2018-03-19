#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include "arena.h"
static u32* screen = 0;




void windowread()
{
}
void windowwrite(struct arena* win)
{
	int j;
	u32* ibuf = win->buf;
	u32* obuf = screen;
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
void windowstart(struct arena* this)
{
	this->type = hex32('w','i','n',0);
	this->fmt = hex64('b','g','r','a','8','8','8','8');

	this->width = this->stride = 1024;
	this->height = 768;

	this->buf = (void*)0x2000000;
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
#define screeninfo 0x2000
	screen = *(u32*)screeninfo;
}
void freewindow()
{
}
