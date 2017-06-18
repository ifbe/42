#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
struct arena
{
	u64 type;		//local,cli,voice,vnc,rdp,...
	u64 fmt;		//rgba8888,vt100...
	u64 rlast;
	u64 rnext;

	u64 buf;
	u64 len;
	u64 dim;
	u64 hah;

	u64 w;
	u64 h;
	u64 d;
	u64 t;

	u64 oldw;
	u64 oldh;
	u64 oldd;
	u64 oldt;

	char priv[0x80];
};
struct actor
{
	//[0,1f]
	u64 type;
	u64 name;
	u64 rlast;
	u64 rnext;

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
	//[00,1f]:doubly link all arenas of this actor
	u64 parent_type;
	u64 parent_id;
	u64 parent_last;
	u64 parent_next;

	//[20,3f]:doubly link all actors of this arena
	u64 child_type;
	u64 child_id;
	u64 child_below;
	u64 child_above;

	//[40,5f]:cartesian coordinate
	u64 cx;		//centerx
	u64 cy;		//centery
	u64 cz;		//centerz
	u64 ct;

	//[60,7f]:eulerian angle
	u64 rx;		//pitch
	u64 ry;		//yaw
	u64 rz;		//roll
	u64 rt;

	//[80,9f]:total size(base 0x10000)
	u64 width;
	u64 height;
	u64 depth;
	u64 time;

	//[a0,bf]:show area(base 0x100000)
	u64 left;
	u64 top;
	u64 right;
	u64 bottom;

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




//
void content_create(void*, void*);
void content_delete();
void external_create(void*, void*);
void external_delete();
//
void lib1d_create(void*, void*);
void lib1d_delete();
void lib2d_create(void*, void*);
void lib2d_delete();
void lib3d_create(void*, void*);
void lib3d_delete();
//
int double2decstr(double,char*);
int data2decstr(u64 data,u8* string);
int ncmp(void*,void*,int);
int cmp(void*,void*);
//
u32 getrandom();
u64 gettime();
double squareroot(double);
//
void eventwrite(u64,u64,u64,u64);
int printmemory(void*, int);
int fmt(void*, int, void*, ...);
int say(void*, ...);