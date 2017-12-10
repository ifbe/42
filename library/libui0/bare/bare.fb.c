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
void windowstart()
{
}
void windowstop()
{
}
void windowcreate()
{
#define screeninfo 0x2000
	int j;
	u64 tmp = *(u32*)screeninfo;
	u8* buf = (u8*)tmp;
	//fmt = *(u32*)(screeninfo+8);

	for(j=0;j<1024*512*3;j++)buf[j] = 0x54;
}
void windowdelete()
{
}
