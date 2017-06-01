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

