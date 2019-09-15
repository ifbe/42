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
#define _dri_ hex32('d','r','i',0)
#define _dev_ hex32('d','e','v',0)




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




struct halfrel
{
	u64 chip;
	u64 foot;
	u32 type;
	u32 flag;
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




//
int actorread(  struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int actorwrite( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int actorstop(  struct halfrel* self,struct halfrel* peer);
int actorstart( struct halfrel* self,struct halfrel* peer);
int actordelete(void*);
void* actorcreate(u64, void*);
//
int arenaread(  struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int arenawrite( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int arenastop(  struct halfrel* self,struct halfrel* peer);
int arenastart( struct halfrel* self,struct halfrel* peer);
int arenadelete(void*);
void* arenacreate(u64, void*);
//
int arteryread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int arterywrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int arterystop( struct halfrel* self,struct halfrel* peer);
int arterystart(struct halfrel* self,struct halfrel* peer);
int arterydelete(void*);
void* arterycreate(u64, void*);
//
int systemread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int systemwrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int systemstop( struct halfrel* self,struct halfrel* peer);
int systemstart(struct halfrel* self,struct halfrel* peer);
int systemdelete(void*);
void* systemcreate(u64, void*);
//
int driverread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int driverwrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int driverstop( struct halfrel* self,struct halfrel* peer);
int driverstart(struct halfrel* self,struct halfrel* peer);
int driverdelete(void*);
void* drivercreate(u64, void*);
//
int deviceread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int devicewrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int devicestop( struct halfrel* self,struct halfrel* peer);
int devicestart(struct halfrel* self,struct halfrel* peer);
int devicedelete(void*);
void* devicecreate(u64, void*);
//
int relationread( void* item, int foot, void* arg, int idx, void* buf, int len);
int relationwrite(void* item, int foot, void* arg, int idx, void* buf, int len);
int relationstop( struct halfrel* self,struct halfrel* peer);
int relationstart(struct halfrel* self,struct halfrel* peer);
int relationdelete(struct relation* rel);
void* relationcreate(void*,void*,u32,u32,void*,void*,u32,u32);
//
void printmemory(void*, int);
void say(void*, ...);
