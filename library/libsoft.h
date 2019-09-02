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
//
#define _FILE_ hex32('F','I','L','E')
#define _file_ hex32('f','i','l','e')
#define _mbr_ hex32('m','b','r',0)
#define _gpt_ hex32('g','p','t',0)
//
#define _spi_  hex32('s','p','i',0)
#define _i2c_  hex32('i','2','c',0)
//uart family
#define _uart_ hex32('u','a','r','t')
#define _Uart_ hex32('U','a','r','t')
#define _gcode_ hex64('g','c','o','d','e',0,0,0)
#define _Gcode_ hex64('G','c','o','d','e',0,0,0)
#define _gps_ hex32('g','p','s',0)
#define _Gps_ hex32('G','p','s',0)
//raw family
#define _RAW_ hex32('R','A','W',0)
#define _Raw_ hex32('R','a','w',0)
#define _raw_ hex32('r','a','w',0)
#define _ICMP_ 0x504d4349		//s
#define _icmp_ 0x706d6369		//c
#define _IGMP_ 0x504d4749		//s
#define _igmp_ 0x706d6769		//c
#define _ARP_ hex32('A','R','P',0)
#define _arp_ hex32('a','r','p',0)
#define _EAP_ hex32('E','A','P',0)
#define _eap_ hex32('e','a','p',0)
#define _IPX_ hex32('I','P','X',0)
#define _ipx_ hex32('i','p','x',0)
#define _PPPOE_ hex64('P','P','P','O','E',0,0,0)
#define _pppoe_ hex64('p','p','p','o','e',0,0,0)
#define _WOL_ hex32('W','O','L',0)
#define _wol_ hex32('w','o','l',0)
//udp family
#define _UDP_ hex32('U','D','P',0)
#define _Udp_ hex32('U','d','p',0)
#define _udp_ hex32('u','d','p',0)
#define _HACK_ hex32('H','A','C','K')
#define _hack_ hex32('h','a','c','k')
#define _bootp_ hex64('b','o','o','t','p',0,0,0)
#define _Bootp_ hex64('B','o','o','t','p',0,0,0)
#define _dns_ hex32('d','n','s',0)
#define _Dns_ hex32('D','n','s',0)
#define _tftp_ hex32('t','f','t','p')
#define _Tftp_ hex32('T','f','t','p')
#define _webrtc_ hex64('w','e','b','r','t','c',0,0)
#define _Webrtc_ hex64('W','e','b','r','t','c',0,0)
#define _WEBRTC_ hex64('W','E','B','R','T','C',0,0)
#define _quic_ hex32('q','u','i','c')
#define _Quic_ hex32('Q','u','i','c')
#define _QUIC_ hex32('Q','U','I','C')
#define _socks5_ hex64('s','o','c','k','s','5')
#define _Socks5_ hex64('S','o','c','k','s','5')
#define _SOCKS5_ hex64('S','O','C','K','S','5')
//tcp family
#define _TCP_ hex32('T','C','P',0)
#define _Tcp_ hex32('T','c','p',0)
#define _tcp_ hex32('t','c','p',0)
#define _SERVE_ hex64('S','E','R','V','E', 0, 0, 0)
#define _Serve_ hex64('S','e','r','v','e', 0, 0, 0)
#define _serve_ hex64('s','e','r','v','e', 0, 0, 0)
#define _SSH_ hex32('S','S','H',0)
#define _Ssh_ hex32('S','s','h',0)
#define _ssh_ hex32('s','s','h',0)
#define _TELNET_ hex64('T','E','L','N','E','T', 0, 0)
#define _Telnet_ hex64('T','e','l','n','e','t', 0, 0)
#define _telnet_ hex64('t','e','l','n','e','t', 0, 0)
#define _HTTP_ hex32('H','T','T','P')
#define _Http_ hex32('H','t','t','p')
#define _http_ hex32('h','t','t','p')
#define _WS_   hex32('W','S',0,0)
#define _Ws_   hex32('W','s',0,0)
#define _ws_   hex32('w','s',0,0)
#define _TLS_  hex32('T','L','S',0)
#define _Tls_  hex32('T','l','s',0)
#define _tls_  hex32('t','l','s',0)
//hole
#define _HOLE_ 0x454c4f48		//s
#define _hole_ 0x656c6f68		//c
#define _STUN_ 0x4e555453		//s
#define _stun_ 0x6e757473		//c
#define _TURN_ 0x4e525554		//s
#define _turn_ 0x6e727574		//c
#define _ICE_ 0x454349		//s
#define _ice_ 0x656369		//c
//http family
#define _HTTPS_ 0x5350545448	//parse, reply
#define _https_ 0x7370747468	//req url, get reply
#define _WSS_ 0x535357		//s
#define _wss_ 0x737377		//c
//live stream
#define _FLV_ 0x564c46		//s
#define _flv_ 0x766c66		//c
#define _RTMP_ 0x504d5452		//s
#define _rtmp_ 0x706d7472		//c
//remote desktop
#define _RDP_ 0x504452		//s
#define _rdp_ 0x706472		//c
#define _VNC_ 0x434e56		//s
#define _vnc_ 0x636e76		//c
#define _SPICE_ 0x4543495053	//s
#define _spice_ 0x6563697073	//c
//download
#define _ED2K_ 0x4b324445		//s
#define _ed2k_ 0x6b326465		//c
#define _MAGNET_ 0x54454e47414d	//s
#define _magnet_ 0x74656e67616d	//c
#define _TORRENT_ 0x544e4552524f54	//s
#define _torrent_ 0x746e6572726f74	//c
//
#define _PROXY_ 0x59584f5250	//s
#define _proxy_ 0x79786f7270	//c
#define _SQL_ 0x4c5153		//s
#define _sql_ 0x6c7173		//c




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




struct object
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
	u64 fmt;
	u64 name;

	//[40,5f]: data
	u64 selffd;
	u64 thatfd;
	u64 dc;
	union{
		u64 addr;
		void* buf;
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

	//[0x80,0xbf]
	u8 self[0x20];
	u8 peer[0x20];

	//[0xc0,0xff]
	u8 data[0x40];
};
struct element
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
		u64 fd;
		void* obj;
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




int artery_leafread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int artery_leafwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
int artery_rootread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int artery_rootwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
int system_leafread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int system_leafwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
int system_rootread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int system_rootwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
//
int actorread(  struct halfrel* self,struct halfrel* peer,void* buf,int len);
int actorwrite( struct halfrel* self,struct halfrel* peer,void* buf,int len);
int actorstop(  struct halfrel* self,struct halfrel* peer);
int actorstart( struct halfrel* self,struct halfrel* peer);
int actordelete(void*);
void* actorcreate(u64, void*);
//
int arenaread(  struct halfrel* self,struct halfrel* peer,void* buf,int len);
int arenawrite( struct halfrel* self,struct halfrel* peer,void* buf,int len);
int arenastop(  struct halfrel* self,struct halfrel* peer);
int arenastart( struct halfrel* self,struct halfrel* peer);
int arenadelete(void*);
void* arenacreate(u64, void*);
//
int arteryread( struct halfrel* self,struct halfrel* peer,void* buf,int len);
int arterywrite(struct halfrel* self,struct halfrel* peer,void* buf,int len);
int arterystop( struct halfrel* self,struct halfrel* peer);
int arterystart(struct halfrel* self,struct halfrel* peer);
int arterydelete(void*);
void* arterycreate(u64, void*);
//
int systemread( struct halfrel* self,struct halfrel* peer,void* buf,int len);
int systemwrite(struct halfrel* self,struct halfrel* peer,void* buf,int len);
int systemstop( struct halfrel* self,struct halfrel* peer);
int systemstart(struct halfrel* self,struct halfrel* peer);
int systemdelete(void*);
void* systemcreate(u64, void*);
//
int relationread( struct halfrel* self,struct halfrel* peer,void* buf,int len);
int relationwrite(struct halfrel* self,struct halfrel* peer,void* buf,int len);
int relationstop( struct halfrel* self,struct halfrel* peer);
int relationstart(struct halfrel* self,struct halfrel* peer);
int relationdelete(void*);
void* relationcreate(void*,void*,int,int,void*,void*,int,int);
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);
//
int threaddelete(u64);
u64 threadcreate(void*, void*);
int memorydelete(void*);
void* memorycreate(int, int);
void* eventread();
void* eventwrite(u64,u64,u64,u64);
//
u32 getrandom();
u64 dateread();
u64 timeread();
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
int mysnprintf(void*, int, void*, ...);
int printmemory(void*, int);
int say(void*, ...);
