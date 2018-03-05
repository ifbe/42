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
	//[00,1f]
	u64 type;
	u64 fmt;
	union{
		struct relation* irel;
		char pad0[8];
	};
	union{
		struct relation* orel;
		char pad1[8];
	};

	//[20,3f]data
	union{
		u64 fd;
		void* win;
	};
	union{
		u64 len;
		u64 dc;
		u64 gc;
		void* er;
	};
	union{
		u64 haha;
		void* ximage;
		void* texture;
		void* pass;
	};
	union{
		u64 addr;
		void* buf;
	};

	//[40,7f]vec
	int cx;
	int cy;
	int cz;
	int flag0;

	union{
		int rx;
		int w;
	};
	int ry;
	int rz;
	int flag1;

	int fx;
	union{
		int fy;
		int h;
	};
	int fz;
	int flag2;

	int ux;
	int uy;
	union{
		int uz;
		int d;
	};
	int flag3;

	//[80,ff]
	union{
		u64 info[16];
		struct point touch[16];
	};
};




//
u64 startthread(void*, void*);
void stopthread(u64);
//
void* startmemory();
void* stopmemory(void*);
//
void* eventread();
void eventwrite(u64,u64,u64,u64);
//
void printmemory(void*, int);
void say(void*, ...);
