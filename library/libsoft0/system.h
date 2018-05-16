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
#define _fd_ hex32('f','d',0,0)
#define _art_ hex32('a','r','t',0)
#define _win_ hex32('w','i','n',0)
#define _act_ hex32('a','c','t',0)




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
struct object
{
	//[0x00,0x0f]
	u32 tier;
	u32 type;	//raw, bt, udp, tcp?
	u64 name;
	union{
		void* irel;
		u64 pad0;
	};
	union{
		void* orel;
		u64 pad1;
	};

	//[0x20,0x3f]
	u64 selffd;
	u64 thatfd;
	u64 len;
	union{
		u64 addr;
		void* buf;
	};

	//[0x40,0x7f]
	u8 self[0x20];
	u8 peer[0x20];

	//[0x80,0xff]
	u8 data[0x80];
};




int actorread(  void* dc,void* df,void* sc,void* sf,void* buf,int len);
int actorwrite( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arenaread(  void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arenawrite( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arteryread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arterywrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
int systemread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int systemwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
//
int ncmp(void*,void*,int);
int cmp(void*,void*);
//
int readsocket(int,int,void*,int);
int writesocket(int,int,void*,int);
int readfile(int,int,void*,int);
int writefile(int,int,void*,int);
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