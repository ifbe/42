#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define __kbd__ hex32('k','b','d',0)
#define __char__ hex32('c','h','a','r')
#define __size__ hex32('s','i','z','e')




struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
struct point
{
	u16 x;
	u16 y;
	u16 z;
	u16 id;
};




struct window
{
	//local,cli,voice,vnc,rdp,...
	//rgba8888,vt100...
	u64 type;
	u64 fmt;
	union{
		struct relation* first;
		char pad0[8];
	};
	union{
		struct relation* last;
		char pad1[8];
	};

	//data
	union{
		void* buf;
		u64 fd;
	};
	union{
		u64 len;
		u64 dc;
		u64 gc;
	};
	u64 ximage;
	u64 thread;

	//where
	u64 cx;
	u64 cy;
	u64 cz;
	u64 cw;

	//size
	u64 w;
	u64 h;
	u64 d;
	u64 t;

	//
	union{
		u64 info[16];
		struct point touch[16];
	};
};
