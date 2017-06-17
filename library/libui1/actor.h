#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




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

	char data[0xc0];
};
struct working
{
	//[0,7]:种类
	u64 type;

	//[8,f]:名字
	u64 name;

	//[10,17]:开始
	int (*start)();
	char padding2[ 8 - sizeof(char*) ];

	//[18,1f]:结束
	int (*stop)();
	char padding3[ 8 - sizeof(char*) ];

	//[20,27]:观察
	int (*list)();
	char padding4[ 8 - sizeof(char*) ];

	//[28,2f]:调整
	int (*choose)();
	char padding5[ 8 - sizeof(char*) ];

	//[30,37]:输出
	int (*read)(void* arena, void* actor, void* treaty);
	char padding6[ 8 - sizeof(char*) ];

	//[38,3f]:输入
	int (*write)(void* event);
	char padding7[ 8 - sizeof(char*) ];

	char data[0xc0];
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
