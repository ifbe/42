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
#define _fd_ hex32('f','d',0,0)
#define _dri_ hex32('d','r','i',0)
#define _dev_ hex32('d','e','v',0)
//
#define _ahci_ hex32('a','h','c','i')
#define _xhci_ hex32('x','h','c','i')
#define _usb_ hex32('u','s','b',0)
//
#define _spi_  hex32('s','p','i',0)
#define _i2c_  hex32('i','2','c',0)
#define _uart_ hex32('u','a','r','t')




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
struct device
{
	//[00,1f]: wire
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

	//[20,3f]: type
	u64 tier;
	u64 type;
	u64 stage1;
	u64 name;

	//[40,5f]: data
	union{
		int fd;
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
	};

	//[60,7f]: prop
	int width;
	int height;
	int depth;
	int stride;
	int fbwidth;
	int fbheight;
	int fbdepth;
	int fbstride;

	//[80,ff]
	u8 data[0x80];
};
struct driver
{
	//[00,1f]: wire
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

	//[20,3f]: type
	u64 tier;
	u64 type;
	u64 stage1;
	u64 name;

	//[40,5f]: data
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
	};

	//[60,7f]: prop
	int width;
	int height;
	int depth;
	int stride;
	int fbwidth;
	int fbheight;
	int fbdepth;
	int fbstride;

	//[80,ff]
	u8 data[0x80];
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
int relationread( void* item, int foot, void* arg, int idx, void* buf, int len);
int relationwrite(void* item, int foot, void* arg, int idx, void* buf, int len);
int relationstop( struct halfrel* self,struct halfrel* peer);
int relationstart(struct halfrel* self,struct halfrel* peer);
int relationdelete(void*);
void* relationcreate(void*,void*,int,int,void*,void*,int,int);
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void printmemory(void*, int);
void say(void*, ...);
