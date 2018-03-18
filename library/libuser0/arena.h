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




struct point
{
	u16 x;
	u16 y;
	u16 z;
	u16 id;
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
struct relation
{
	//[0x00,0x1f]
	u64 destchip;
	u64 destfoot;
	u32 desttype;
	u32 destflag;
	u32 samepinprevchip;
	u32 samepinnextchip;

	//0x20,0x3f
	u64 selfchip;
	u64 selffoot;
	u32 selftype;
	u32 selfflag;
	u32 samechipprevpin;
	u32 samechipnextpin;
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
		u64 dc;
		u64 gc;
		void* er;
	};
	union{
		u64 len;
		void* ximage;
		void* texture;
		void* pass;
	};
	union{
		u64 addr;
		void* buf;
	};

	//[40,7f]func
	u64 oncreate;
	u64 ondelete;
	u64 onstart;
	u64 onstop;
	u64 onread;
	u64 onwrite;
	u64 onlist;
	u64 onchoose;

	//[80,ff]: w,h,d,s
	int width;
	int height;
	int depth;
	int stride;

	//[90,9f]: center
	int cx;
	int cy;
	int cz;
	int vkbd;

	//[a0,af]: right
	int rx;
	int ry;
	int rz;
	int theone;

	//[b0,bf]: far
	int fx;
	int fy;
	int fz;
	int edit;

	//[c0,cf]: upper
	int ux;
	int uy;
	int uz;
	int other;

	//[d0,df]
	int dirtyx0;
	int dirtyy0;
	int dirtyz0;
	int dirty;

	//[e0,ef]
	int dirtyx1;
	int dirtyy1;
	int dirtyz1;
	int dirtyw1;

	//[f0,ff]
	int dirtyx2;
	int dirtyy2;
	int dirtyz2;
	int dirtyw2;

	//[100,1ff]
	struct point touchdown[16];
	struct point touchmove[16];
};
struct style
{
	//[00,0f]:
	u64 type;
	u64 name;

	//[10,1f]: center
	float cx;
	float cy;
	float cz;
	float cw;

	//[20,2f]: right
	float rx;
	float ry;
	float rz;
	float rtype;

	//[30,3f]: far
	float fx;
	float fy;
	float fz;
	float ftype;

	//[40,4f]: upper
	float ux;
	float uy;
	float uz;
	float utype;

	//[50,5f]: left
	float lx;
	float ly;
	float lz;
	float ltype;

	//[60,6f]: near
	float nx;
	float ny;
	float nz;
	float ntype;

	//[70,7f]: bottom
	float bx;
	float by;
	float bz;
	float btype;
};



#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif