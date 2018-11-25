typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define _act_ hex32('a','c','t',0)
#define _win_ hex32('w','i','n',0)
#define _art_ hex32('a','r','t',0)
#define _fd_ hex32('f','d',0,0)




struct str
{
	int len;
	u8 buf[0];
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
struct item
{
	union{
		void* irel0;
		u64 ipad0;
	};
	union{
		void* ireln;
		u64 ipadn;
	};
	union{
		void* orel0;
		u64 opad0;
	};
	union{
		void* oreln;
		u64 opadn;
	};

	u64 tier;
	u64 type;
	u64 fmt;
	u64 name;
};




int actor_leafread(  void* dc,void* df,void* sc,void* sf,void* buf,int len);
int actor_leafwrite( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int actor_rootread(  void* dc,void* df,void* sc,void* sf,void* buf,int len);
int actor_rootwrite( void* dc,void* df,void* sc,void* sf,void* buf,int len);
//
int arena_leafread(  void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arena_leafwrite( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arena_rootread(  void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arena_rootwrite( void* dc,void* df,void* sc,void* sf,void* buf,int len);
//
int artery_leafread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int artery_leafwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
int artery_rootread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int artery_rootwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
//
int system_leafread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int system_leafwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
int system_rootread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int system_rootwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
//
void printmemory(void*, int);
void say(void*, ...);