#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
struct window
{
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;
};




int netwinread()
{
	return 0;
}
int netwinwrite(struct window* win)
{
	//say("%s\n", (void*)(win->buf));
	return 0;
}
int netwinlist()
{
	return 0;
}
int netwinchoose()
{
	return 0;
}
int netwinstop()
{
	return 0;
}
int netwinstart(struct window* win)
{
	win->buf = malloc(0x100000);
	win->fmt = 0x6c6d7468;
	win->w = 512;
	win->h = 512;
	return 0;
}
int netwindelete()
{
	return 0;
}
int netwincreate()
{
	return 0;
}
