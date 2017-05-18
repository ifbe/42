#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char




struct player
{
	u64 type;
	u64 name;
	u64 start;
	u64 stop;
	u64 list;
	u64 choose;
	u64 read;
	u64 write;

	u8 data[0xc0];
};      
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

	u8 data[0xc0];
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};




static void fs_read(struct window* win)
{
}
static void fs_write(struct event* ev)
{
}




static void fs_list()
{
}
static void fs_into()
{
}
static void fs_start()
{
}
static void fs_stop()
{
}
void fs_create(void* base,void* addr)
{
	struct player* p = addr;

	p->type = 0x6c6f6f74;
	p->name = 0x7366;
	p->start = (u64)fs_start;
	p->stop = (u64)fs_stop;
	p->list = (u64)fs_list;
	p->choose = (u64)fs_into;
	p->write = (u64)fs_read;
	p->write = (u64)fs_write;
}
void fs_delete()
{
}
