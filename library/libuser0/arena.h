#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define _win_ hex32('w','i','n',0)
#define _act_ hex32('a','c','t',0)
#define _kbd_ hex32('k','b','d',0)
#define _char_ hex32('c','h','a','r')
#define _size_ hex32('s','i','z','e')
#define _FILE_ hex32('F','I','L','E')
#define _file_ hex32('f','i','l','e')




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
	u32 samedstprevsrc;
	u32 samedstnextsrc;

	//0x20,0x3f
	u64 selfchip;
	u64 selffoot;
	u32 selftype;
	u32 selfflag;
	u32 samesrcprevdst;
	u32 samesrcnextdst;
};




struct arena
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
	int dirty;

	//[a0,af]: right
	int rx;
	int ry;
	int rz;
	int vkbd;

	//[b0,bf]: far
	int fx;
	int fy;
	int fz;
	int theone;

	//[c0,cf]: upper
	int ux;
	int uy;
	int uz;
	int edit;

	//[d0,df]
	int dirtyx0;
	int dirtyy0;
	int dirtyz0;
	int flag0;

	//[e0,ef]
	int dirtyx1;
	int dirtyy1;
	int dirtyz1;
	int flag1;

	//[f0,ff]
	int dirtyx2;
	int dirtyy2;
	int dirtyz2;
	int flag2;

	//[100,1ff]
	struct point touchdown[16];
	struct point touchmove[16];
};
struct actor
{
	//[0,1f]
	u64 type;
	u64 name;
	union{
		struct relation* irel;
		char pad0[8];
	};
	union{
		struct relation* orel;
		char pad1[8];
	};

	//[20,3f]
	union{
		u64 fd;
		void* what0;
	};
	union{
		u64 abc;
		void* what1;
	};
	union{
		u64 vbo;
		void* unique;
	};
	union{
		u64 addr;
		void* buf;
	};

	//[40,7f]vec
	union{
		int (*oncreate)(void* actor, void* buf);
		char padding0[8];
	};
	union{
		int (*ondelete)(void* actor, void* buf);
		char padding1[8];
	};
	union{
		int (*onstart)(void* actor, void* pinid);
		char padding2[8];
	};
	union{
		int (*onstop)(void* actor, void* pinid);
		char padding3[8];
	};
	union{
		int (*onlist)(void* actor, void* pinid, void* buf, int len);
		char padding4[8];
	};
	union{
		int (*onchoose)(void* actor, void* pinid, void* buf, int len);
		char padding5[8];
	};
	union{
		int (*onread)(void* arena, void* style, void* actor, void* compo);
		char padding6[8];
	};
	union{
		int (*onwrite)(void* actor, void* compo, void* event);
		char padding7[8];
	};

	//[+00,+0f]: center
	int width;
	int height;
	int depth;
	int stride;

	//[+10,+1f]: center
	int cx;
	int cy;
	int cz;
	int dirty;

	//[+20,+2f]: right
	int rx;
	int ry;
	int rz;
	int rw;

	//[+30,+3f]: far
	int fx;
	int fy;
	int fz;
	int fw;

	//[+40,+4f]: upper
	int ux;
	int uy;
	int uz;
	int uw;

	//[+50,+5f]: left
	int lx;
	int ly;
	int lz;
	int ltype;

	//[+60,+6f]: near
	int nx;
	int ny;
	int nz;
	int ntype;

	//[+70,+7f]: bottom
	int bx;
	int by;
	int bz;
	int btype;

	//[100,1ff]
	u8 detail[0x100];
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
struct pinid
{
	int flag00;
	int flag01;
	int flag02;
	int flag03;

	int flag04;
	int flag05;
	int flag06;
	int flag07;

	int flag08;
	int flag09;
	int flag0a;
	int flag0b;

	int flag0c;
	int flag0d;
	int flag0e;
	int flag0f;

	int flag10;
	int flag11;
	int flag12;
	int flag13;

	int flag14;
	int flag15;
	int flag16;
	int flag17;

	int flag18;
	int flag19;
	int flag1a;
	int flag1b;

	int flag1c;
	int flag1d;
	int flag1e;
	int flag1f;
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