#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include "arena.h"




void windowread()
{
}
void windowwrite()
{
}
void windowlist()
{
}
void windowchoose()
{
}
void windowstart(struct window* this)
{
	this->type = hex32('w','i','n',0);
	this->fmt = hex32('c','l','i',0);

	this->buf = 0;
	this->len = 0;

	this->width = this->stride = 80;
	this->height = 25;
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
