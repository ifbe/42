#include "libuser.h"




void windowread()
{
}
void windowwrite(struct arena* src)
{
	int x,y;
	u8* ibuf = src->buf;
	u8* obuf = (void*)0xa0000;
	for(x=0;x<320*200;x++)obuf[x] = ibuf[x];
}
void windowlist()
{
}
void windowchoose()
{
}
void windowstart(struct arena* win)
{
	this->type = hex32('w','i','n',0);
	this->fmt = hex32('8','b','i','t');

	this->len = 320*200;
	this->buf = (void*)(u64)0x2000000;

	this->width = this->stride = 320;
	this->height = 200;
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
