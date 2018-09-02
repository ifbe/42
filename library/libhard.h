typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;




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




int actorread(  void* dc,void* df,void* sc,void* sf,void* buf,int len);
int actorwrite( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arenaread(  void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arenawrite( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arteryread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arterywrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
int systemread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int systemwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);