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
void windowstart(struct window* win)
{
	win->type = 0x696c63;
	win->fmt = 0x696c63;
	win->buf = 0;

	win->w = 80;
	win->h = 25;
	win->d = 0;
	win->dim = 1;
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
