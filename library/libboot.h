typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
//
#define _act_ hex32('a','c','t',0)
#define _win_ hex32('w','i','n',0)
#define _art_ hex32('a','r','t',0)
#define _sys_ hex32('s','y','s',0)
#define _dri_ hex32('d','r','i',0)
#define _dev_ hex32('d','e','v',0)
#define _wrk_ hex32('w','r','k',0)
#define _pwr_ hex32('p','w','r',0)
//bare metal
#define _start_ hex64('s','t','a','r','t', 0, 0, 0)
#define _efimain_ hex64('e','f','i','m','a','i','n', 0)
//module
#define _lib42_ hex64('l','i','b','4','2', 0, 0, 0)
#define _kext_ hex32('k','e','x','t')
#define _ko_ hex32('k','o', 0, 0)
//application
#define _main_ hex32('m','a','i','n')
#define _ndkmain_ hex64('n','d','k','m','a','i','n', 0)
#define _win32_ hex64('w','i','n','3','2', 0, 0, 0)
#define _winmain_ hex64('w','i','n','m','a','i','n', 0)
//
#define _waiter_ hex64('w','a','i','t','e','r', 0, 0)
#define _poller_ hex64('p','o','l','l','e','r', 0, 0)
#define _pulser_ hex64('p','u','l','s','e','r', 0, 0)
#define _realer_ hex64('r','e','a','l','e','r', 0, 0)




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

	//[40,5f]: fd/handle
	union{
		u64 sz0;
		u64 selffd;
	};
	union{
		u64 sz1;
		void* selfobj;
	};
	union{
		u64 sz2;
		u64 tempfd;
	};
	union{
		u64 sz3;
		void* tempobj;
	};

	//[60,7f]: memory
	union{
		u64 padd0;
		void* addr0;
	};
	union{
		u64 padd1;
		void* addr1;
	};
	union{
		u64 padd2;
		void* addr2;
	};
	union{
		u64 padd3;
		void* addr3;
		void* buf;
	};
};
struct pwrclk
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
	u64 hfmt;
	u64 vfmt;

	//[40,5f]: fd/handle
	union{
		u64 sz0;
		u64 selffd;
	};
	union{
		u64 sz1;
		void* selfobj;
	};
	union{
		u64 sz2;
		u64 tempfd;
	};
	union{
		u64 sz3;
		void* tempobj;
	};

	//[60,7f]: memory
	union{
		u64 padd0;
		void* addr0;
	};
	union{
		u64 padd1;
		void* addr1;
	};
	union{
		u64 padd2;
		void* addr2;
	};
	union{
		u64 padd3;
		void* addr3;
		void* buf;
	};

	//[80, ff]
	u8 tmp[0x80];
};
struct worker
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
	u64 hfmt;
	u64 vfmt;

	//[40,5f]: fd/handle
	union{
		u64 sz0;
		u64 selffd;
	};
	union{
		u64 sz1;
		void* selfobj;
	};
	union{
		u64 sz2;
		u64 tempfd;
	};
	union{
		u64 sz3;
		void* tempobj;
	};

	//[60,7f]: memory
	union{
		u64 padd0;
		void* addr0;
	};
	union{
		u64 padd1;
		void* addr1;
	};
	union{
		u64 padd2;
		void* addr2;
	};
	union{
		u64 padd3;
		void* addr3;
		void* buf;
	};

	//[80,ff]
	u8 tmp[0x80];
};




//
int entityread(  struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int entitywrite( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int entitystop(  struct halfrel* self,struct halfrel* peer);
int entitystart( struct halfrel* self,struct halfrel* peer);
int entitydelete(void*);
void* entitycreate(u64 type, void* addr, int argc, u8** argv);
//
int supplyread(  struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int supplywrite( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int supplystop(  struct halfrel* self,struct halfrel* peer);
int supplystart( struct halfrel* self,struct halfrel* peer);
int supplydelete(void*);
void* supplycreate(u64 type, void* addr, int argc, u8** argv);
//
int arteryread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int arterywrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int arterystop( struct halfrel* self,struct halfrel* peer);
int arterystart(struct halfrel* self,struct halfrel* peer);
int arterydelete(void*);
void* arterycreate(u64 type, void* addr, int argc, u8** argv);
//
int systemread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int systemwrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int systemstop( struct halfrel* self,struct halfrel* peer);
int systemstart(struct halfrel* self,struct halfrel* peer);
int systemdelete(void*);
void* systemcreate(u64 type, void* addr, int argc, u8** argv);
//
int driverread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int driverwrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int driverstop( struct halfrel* self,struct halfrel* peer);
int driverstart(struct halfrel* self,struct halfrel* peer);
int driverdelete(void*);
void* drivercreate(u64 type, void* addr, int argc, u8** argv);
//
int deviceread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int devicewrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int devicestop( struct halfrel* self,struct halfrel* peer);
int devicestart(struct halfrel* self,struct halfrel* peer);
int devicedelete(void*);
void* devicecreate(u64 type, void* addr, int argc, u8** argv);
//
int workerread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int workerwrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int workerstop( struct halfrel* self,struct halfrel* peer);
int workerstart(struct halfrel* self,struct halfrel* peer);
int workerdelete(void*);
void* workercreate(u64 type, void* addr, int argc, u8** argv);
//
int pwrclkread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int pwrclkwrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int pwrclkstop( struct halfrel* self,struct halfrel* peer);
int pwrclkstart(struct halfrel* self,struct halfrel* peer);
int pwrclkdelete(void*);
void* pwrclkcreate(u64 type, void* addr, int argc, u8** argv);
//
int relationread( void* item, int foot, void* arg, int idx, void* buf, int len);
int relationwrite(void* item, int foot, void* arg, int idx, void* buf, int len);
int relationstop( struct halfrel* self,struct halfrel* peer);
int relationstart(struct halfrel* self,struct halfrel* peer);
int relationdelete(struct relation* rel);
void* relationcreate(void*,void*,u32,u32,void*,void*,u32,u32);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
void printmemory(void*, int);
void say(void*, ...);
