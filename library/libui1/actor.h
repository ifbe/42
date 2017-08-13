#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))




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




struct arena
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
		u64 buf;
		u64 fd;
	};
	union{
		u64 len;
		u64 dc;
	};
	union{
		u64 dim;
		u64 info;
	};
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
	struct point touch[16];
};
struct actor
{
	//[0,1f]
	u64 type;
	u64 name;
	union{
		struct relation* first;
		char pad0[8];
	};
	union{
		struct relation* last;
		char pad1[8];
	};

	//[20,3f]
	u64 aa;
	u64 bb;
	u64 cc;
	u64 dd;

	//[40,47]
	union{
		int (*create)();
		char padding0[8];
	};

	//[48,4f]
	union{
		int (*delete)();
		char padding1[8];
	};

	//[50,57]:开始
	union{
		int (*start)();
		char padding2[8];
	};

	//[58,5f]:结束
	union{
		int (*stop)();
		char padding3[8];
	};

	//[60,67]:观察
	union{
		int (*list)();
		char padding4[8];
	};

	//[68,6f]:调整
	union{
		int (*choose)();
		char padding5[8];
	};

	//[70,77]:输出
	union{
		int (*read)(void* win, void* act, void* rel);
		char padding6[8];
	};

	//[78,7f]:输入
	union{
		int (*write)(void* event);
		char padding7[8];
	};

	//
	char priv[0x80];
};
struct relation
{
	//[00,1f]: the pin
	u64 pininfo;
	u64 destiny;
	union{
		struct relation* samepinlastchip;
		struct relation* samepinlastfunc;
		struct relation* samepinlastwin;
		struct relation* samepinlastact;
		char padding2[8];
	};
	union{
		struct relation* samepinnextchip;
		struct relation* samepinnextfunc;
		struct relation* samepinnextwin;
		struct relation* samepinnextact;
		char padding3[8];
	};

	//[20,3f]: the chip
	u64 chipinfo;
	u64 footinfo;		//style
	union{
		struct relation* samechiplastpin;
		struct relation* samefunclastpin;
		struct relation* samewinlastpin;
		struct relation* sameactlastpin;
		char padding6[8];
	};
	union{
		struct relation* samechipnextpin;
		struct relation* samefuncnextpin;
		struct relation* samewinnextpin;
		struct relation* sameactnextpin;
		char padding7[8];
	};
};
struct style
{
	//[00,1f]:cartesian coordinate
	u64 cx;
	u64 cy;
	u64 cz;
	u64 dim;

	//[20,3f]:eulerian angle
	u64 pitch;
	u64 yaw;
	u64 roll;
	u64 aaa;

	//[40,5f]:total size(base 0x10000)
	u64 wantw;
	u64 wanth;
	u64 wantd;
	u64 hehe;

	//[60,7f]:show area(base 0x100000)
	u64 realw;
	u64 realh;
	u64 reald;
	u64 haha;
};




//libsoft1
int double2decstr(double, void*);
int double2hexstr(double, void*);

int data2decstr(u64 data,u8* str);
int data2hexstr(u64 data,u8* str);

int datastr2decstr(void* dst, void* src, int len);
int datastr2hexstr(void* dst, void* src, int len);

int decstr2data(void* str, u64 data);
int decstr2datastr(void* dst, void* src, int len);
int decstr2double(void* dst, void* src, int len);

int hexstr2data(void* str, u64 data);
int hexstr2datastr(void* dst, void* src, int len);
int hexstr2double(void* dst, void* src, int len);

int cmp(void*,void*);
int ncmp(void*,void*,int);

int md5sum(void*, void*, int);
int sha1sum(void*, void*, int);
int sha256sum(void*, void*, int);
int sha384sum(void*, void*, int);
int sha512sum(void*, void*, int);
//libsoft0
u32 getrandom();
u64 gettime();

int readfile(void*, void*, u64, u64);
int writefile(void*, void*, u64, u64);
//libhard1
double squareroot(double);
//libboot1
void eventwrite(u64,u64,u64,u64);
int printmemory(void*, int);
int fmt(void*, int, void*, ...);
int say(void*, ...);