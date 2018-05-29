typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef float vec1[1];
typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];
typedef float mat1[1][1];
typedef float mat2[2][2];
typedef float mat3[3][3];
typedef float mat4[4][4];
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define _win_ hex32('w','i','n',0)
#define _act_ hex32('a','c','t',0)
#define _kbd_ hex32('k','b','d',0)
#define _char_ hex32('c','h','a','r')
#define _size_ hex32('s','i','z','e')
#define _drag_ hex32('d','r','a','g')
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
	vec4 vl;	//[00,0f]: left
	vec4 vr;	//[10,1f]: right
	vec4 vn;	//[20,2f]: near
	vec4 vf;	//[30,3f]: far
	vec4 vb;	//[40,4f]: bottom
	vec4 vu;	//[50,5f]: upper
	vec4 vv;	//[60,6f]: info
	vec4 vc;	//[70,7f]: center
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
	int enq;
	int deq;

	//[b0,bf]
	int flag30;
	int flag31;
	int time;
	int fps;

	//[c0,cf]: layer0
	int backx;
	int backy;
	int backdata;
	int backtype;

	//[d0,df]: layer1
	int modex;
	int modey;
	int modedata;
	int modetype;

	//[e0,ef]: layer2
	int menux;
	int menuy;
	int menudata;
	int menutype;

	//[f0,ff]: layer3
	int vkbdx;
	int vkbdy;
	int vkbddata;
	int vkbdtype;

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

	//[80,ff]
	struct style sty;

	//[100,1ff]
	u8 detail[0x100];
};



#ifdef __cplusplus
extern "C" {
#endif

int actorread_all(void* win);
int actorwrite_ev(void* ev);
int actorread(    void* dc,void* df,void* sc,void* sf,void* buf,int len);
int actorwrite(   void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arenaread(    void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arenawrite(   void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arteryread(   void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arterywrite(  void* dc,void* df,void* sc,void* sf,void* buf,int len);
int systemread(   void* dc,void* df,void* sc,void* sf,void* buf,int len);
int systemwrite(  void* dc,void* df,void* sc,void* sf,void* buf,int len);
//
int ncmp(void*,void*,int);
int cmp(void*,void*);
//
u32 getrandom();
u64 getdate();
u64 gettime();
void sleep_us(int);
//
u64 threadcreate(void*, void*);
int threaddelete(u64);
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