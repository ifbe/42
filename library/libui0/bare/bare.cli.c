#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




struct window
{
	u64 buf1;
	u64 buf2;
	u64 fmt;
	u64 dim;

	u64 w;
	u64 h;
	u64 d;
	u64 t;

	u64 thread;
};




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
	win->buf1 = 0;
	win->buf2 = 0;
	win->fmt = 0x696c63;
	win->dim = 1;

	win->w = 80;
	win->h = 25;
	win->d = 0;
	win->t = 0;
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
