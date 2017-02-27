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




int netscnread()
{
}
int netscnwrite(struct window* win)
{
}
int netscnlist()
{
}
int netscnchoose()
{
}
int netscnstop()
{
}
int netscnstart(struct window* win)
{
}
int netscndelete()
{
}
int netscncreate()
{
}
