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
	u64 dstchip;
	u64 dstfoot;
	u32 dsttype;
	u32 dstflag;
	u32 samedstprevsrc;
	u32 samedstnextsrc;

	//0x20,0x3f
	u64 srcchip;
	u64 srcfoot;
	u32 srctype;
	u32 srcflag;
	u32 samesrcprevdst;
	u32 samesrcnextdst;
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
	float rw;

	//[30,3f]: far
	float fx;
	float fy;
	float fz;
	float fw;

	//[40,4f]: upper
	float ux;
	float uy;
	float uz;
	float uw;

	//[50,5f]: left
	float lx;
	float ly;
	float lz;
	float lw;

	//[60,6f]: near
	float nx;
	float ny;
	float nz;
	float nw;

	//[70,7f]: bottom
	float bx;
	float by;
	float bz;
	float bw;
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




struct arena
{
	//[00,1f]
	u32 tier;
	u32 type;
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
		void* hp;
		void* mod;
		void* ximage;
		void* texture;
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

	//[80,ff]
	int width;
	int height;
	int depth;
	int stride;

	//[90,9f]
	int fwidth;
	int fheight;
	int fdepth;
	int fstride;

	//[a0,af]
	int flag20;
	int flag21;
	int flag22;
	int enq;

	//[b0,bf]
	int flag30;
	int flag31;
	int flag32;
	int deq;

	//[c0,cf]
	int flag40;
	int flag41;
	int vkbddata;
	int vkbdtype;

	//[d0,df]
	int flag50;
	int flag51;
	int menudata;
	int menutype;

	//[e0,ef]
	int flag60;
	int flag61;
	int flag62;
	int flag6;

	//[f0,ff]
	int flag70;
	int flag71;
	int flag72;
	int flag7;

	//[100,1ff]
	struct style target;
	struct style camera;

	//[200,2ff]
	struct point touchdown[16];
	struct point touchmove[16];

	//[300,3ff]
	u8 detail[0x100];
};
struct actor
{
	//[0,1f]
	u32 tier;
	u32 type;
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



#ifdef __cplusplus
extern "C" {
#endif

int actorread(void* dc,void* df,void* sc,void* sf);
int actorwrite(void* dc,void* df,void* sc,void* sf,void* buf, int len);
int arenaread(void* dc,void* df,void* sc,void* sf);
int arenawrite(void* dc,void* df,void* sc,void* sf,void* buf, int len);
int arteryread(void* dc,void* df,void* sc,void* sf);
int arterywrite(void* dc,void* df,void* sc,void* sf,void* buf, int len);
int systemread(void* dc,void* df,void* sc,void* sf);
int systemwrite(void* dc,void* df,void* sc,void* sf,void* buf, int len);
//
int ncmp(void*,void*,int);
int cmp(void*,void*);
//
u32 getrandom();
u64 getdate();
u64 gettime();
//
u64 threadcreate(void*, void*);
void threaddelete(u64);
//
void* memorycreate(int);
int memorydelete(void*);
//
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);
void* relationcreate(void*,void*,int,void*,void*,int);
int relationdelete(void*);
//
void* eventread();
void* eventwrite(u64,u64,u64,u64);
//
int mysnprintf(void*, int, void*, ...);
int printmemory(void*, int);
int say(void*, ...);

#ifdef __cplusplus
}
#endif