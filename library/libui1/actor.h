#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hexof(a,b,c,d,e,f,g,h) (a | (b<<8) | (c<<16) | (d<<24) | (((u64)e)<<32) | (((u64)f)<<40) | (((u64)g)<<48) | (((u64)h)<<56) )




struct arena
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
struct actor
{
	//[0,1f]
	u64 type;
	u64 name;
	u64 first;
	u64 last;

	//[20,3f]
	u64 aa;
	u64 bb;
	u64 cc;
	u64 dd;

	//[40,47]
	int (*create)();
	char padding0[ 8 - sizeof(char*) ];

	//[48,4f]
	int (*delete)();
	char padding1[ 8 - sizeof(char*) ];

	//[50,57]:开始
	int (*start)();
	char padding2[ 8 - sizeof(char*) ];

	//[58,5f]:结束
	int (*stop)();
	char padding3[ 8 - sizeof(char*) ];

	//[60,67]:观察
	int (*list)();
	char padding4[ 8 - sizeof(char*) ];

	//[68,6f]:调整
	int (*choose)();
	char padding5[ 8 - sizeof(char*) ];

	//[70,77]:输出
	int (*read)(void* arena, void* actor, void* treaty);
	char padding6[ 8 - sizeof(char*) ];

	//[78,7f]:输入
	int (*write)(void* event);
	char padding7[ 8 - sizeof(char*) ];

	char priv[0x80];
};
struct relation
{
	//[00,1f]:doubly link all shows of this arena
	u64 parent_type;
	void* parent_this;
	char padding1[ 8 - sizeof(char*) ];
	struct relation* parent_prev;
	char padding2[ 8 - sizeof(char*) ];
	struct relation* parent_next;
	char padding3[ 8 - sizeof(char*) ];

	//[20,3f]:doubly link all shows of this actor
	u64 child_type;
	void* child_this;
	char padding5[ 8 - sizeof(char*) ];
	struct relation* child_below;
	char padding6[ 8 - sizeof(char*) ];
	struct relation* child_above;
	char padding7[ 8 - sizeof(char*) ];

	//[40,5f]:cartesian coordinate
	u64 cx;
	u64 cy;
	u64 cz;
	u64 cw;

	//[60,7f]:eulerian angle
	u64 pitch;
	u64 yaw;
	u64 roll;
	u64 aaa;

	//[80,9f]:total size(base 0x10000)
	u64 wantw;
	u64 wanth;
	u64 wantd;
	u64 dim;

	//[a0,bf]:show area(base 0x100000)
	u64 realw;
	u64 realh;
	u64 reald;
	u64 haha;

	//[c0,df]:
	u64 a0;
	u64 a1;
	u64 a2;
	u64 a3;

	//[e0,ff]:
	u64 a4;
	u64 a5;
	u64 a6;
	u64 a7;
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};




//
void content_create(void*, void*);
void content_delete();
void external_create(void*, void*);
void external_delete();
void lib1d_create(void*, void*);
void lib1d_delete();
void lib2d_create(void*, void*);
void lib2d_delete();
void lib3d_create(void*, void*);
void lib3d_delete();
//libsoft1
int double2decstr(double, void*);
int double2hexstr(double, void*);
int decstr2data(void* str, u64 data);
int decstr2datastr(void* dst, void* src, int len);
int hexstr2data(void* str, u64 data);
int hexstr2datastr(void* dst, void* src, int len);
int data2decstr(u64 data,u8* str);
int data2hexstr(u64 data,u8* str);
int datastr2decstr(void* dst, void* src, int len);
int datastr2hexstr(void* dst, void* src, int len);
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