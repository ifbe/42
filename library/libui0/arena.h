#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
struct window
{
	u64 type;		//local,cli,voice,vnc,rdp,...
	u64 fmt;		//rgba8888,vt100...
	u64 bot;
	u64 top;

	u64 w;
	u64 h;
	u64 d;
	u64 t;

	u64 buf;
	u64 len;
	u64 dim;
	u64 hah;

	u64 wnd;
	u64 dc;
	u64 xx;
	u64 thread;

	u64 input[16];
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};