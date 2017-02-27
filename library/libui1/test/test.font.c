#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void printascii(
	int x, int y, int size, char ch, u32 fgcolor, u32 bgcolor);
void printdecimal(
	int x, int y, int size, int data, u32 fg, u32 bg);
void backgroundcolor(void*, u32);
//
void say(char*,...);




struct player
{
        u64 type;
        u64 name;
        u8 temp[0x30];

        u64 create;
        u64 delete;
        u64 start;
        u64 stop;
        u64 list;
        u64 choose;
        u64 read;
        u64 write;
};
struct window
{
        u64 buf;
        u64 fmt;
        u64 w;
        u64 h;
};
struct event
{
        u64 why;
        u64 what;
        u64 where;
        u64 when;
};




void font_read(struct window* win)
{
	int x,y;
	backgroundcolor(win, 0);

	for(y=2;y<8;y++)
	{
		for(x=0;x<16;x++)
		{
			printascii(
				x*16,
				y*16,
				1,
				x+y*16,
				0xffffffff,
				0
			);
		}
	}
}
void font_write(struct event* ev)
{
}
void font_list()
{
}
void font_change()
{
}
void font_start()
{
}
void font_stop()
{
}
void font_create(void* base, void* addr)
{
	struct player* p = addr;
	p->type = 0x74736574;
	p->name = 0x746e6f66;

	p->start = (u64)font_start;
	p->stop = (u64)font_stop;
	p->list = (u64)font_list;
	p->choose = (u64)font_change;
	p->read = (u64)font_read;
	p->write = (u64)font_write;
}
void font_delete()
{
}
