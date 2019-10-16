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
//
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
#define _clk_ hex32('c','l','k',0)
//
#define _act_ hex32('a','c','t',0)
#define _win_ hex32('w','i','n',0)
#define _art_ hex32('a','r','t',0)
#define _sys_ hex32('s','y','s',0)
#define _dri_ hex32('d','r','i',0)
#define _dev_ hex32('d','e','v',0)
#define _wrk_ hex32('w','r','k',0)
#define _pwr_ hex32('p','w','r',0)
//
#define _FILE_ hex32('F','I','L','E')
#define _file_ hex32('f','i','l','e')
#define _mbr_ hex32('m','b','r',0)
#define _gpt_ hex32('g','p','t',0)
//raw family
#define _RAW_ hex32('R','A','W',0)
#define _Raw_ hex32('R','a','w',0)
#define _raw_ hex32('r','a','w',0)
#define _ICMP_ hex32('I','C','M','P')
#define _icmp_ hex32('i','c','m','p')
#define _IGMP_ hex32('I','G','M','P')
#define _igmp_ hex32('i','g','m','p')
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
#define _socks_ hex64('s','o','c','k','s', 0, 0, 0)
#define _Socks_ hex64('S','o','c','k','s', 0, 0, 0)
#define _SOCKS_ hex64('S','O','C','K','S', 0, 0, 0)
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
#define _HOLE_ hex32('H','O','L','E')
#define _Hole_ hex32('H','o','l','e')
#define _hole_ hex32('h','o','l','e')
#define _STUN_ hex32('S','T','U','N')
#define _Stun_ hex32('S','t','u','n')
#define _stun_ hex32('s','t','u','n')
#define _TURN_ hex32('T','U','R','N')
#define _Turn_ hex32('T','u','r','n')
#define _turn_ hex32('t','u','r','n')
#define _ICE_ hex32('I','C','E',0)
#define _Ice_ hex32('I','c','e',0)
#define _ice_ hex32('i','c','e',0)
//http family
#define _HTTPS_ hex64('H','T','T','P','S', 0, 0, 0)
#define _Https_ hex64('H','t','t','p','s', 0, 0, 0)
#define _https_ hex64('h','t','t','p','s', 0, 0, 0)
#define _WSS_ hex32('W','S','S', 0)
#define _Wss_ hex32('W','s','s', 0)
#define _wss_ hex32('w','s','s', 0)
//live stream
#define _FLV_ hex32('F','L','V',0)
#define _Flv_ hex32('F','l','v',0)
#define _flv_ hex32('f','l','v',0)
#define _RTMP_ hex32('R','T','M','P')
#define _Rtmp_ hex32('R','t','m','p')
#define _rtmp_ hex32('r','t','m','p')
//remote desktop
#define _RDP_ hex32('R','D','P',0)
#define _Rdp_ hex32('R','d','p',0)
#define _rdp_ hex32('r','d','p',0)
#define _VNC_ hex32('V','N','C',0)
#define _Vnc_ hex32('V','n','c',0)
#define _vnc_ hex32('v','n','c',0)
#define _SPICE_ hex64('S','P','I','C','E', 0, 0, 0)
#define _Spice_ hex64('S','p','i','c','e', 0, 0, 0)
#define _spice_ hex64('s','p','i','c','e', 0, 0, 0)
//download
#define _ED2K_ hex32('E','D','2','K')
#define _Ed2k_ hex32('E','d','2','k')
#define _ed2k_ hex32('e','d','2','k')
#define _MAGNET_ hex64('M','A','G','N','E','T', 0, 0)
#define _Magnet_ hex64('M','a','g','n','e','t', 0, 0)
#define _magnet_ hex64('m','a','g','n','e','t', 0, 0)
#define _TORRENT_ hex64('T','O','R','R','E','N','T', 0)
#define _Torrent_ hex64('T','o','r','r','e','n','t', 0)
#define _torrent_ hex64('t','o','r','r','e','n','t', 0)
//
#define _PROXY_ hex64('P','R','O','X','Y', 0, 0, 0)
#define _Proxy_ hex64('P','r','o','x','y', 0, 0, 0)
#define _proxy_ hex64('p','r','o','x','y', 0, 0, 0)
#define _SQL_ hex32('S','Q','L',0)
#define _Sql_ hex32('S','q','l',0)
#define _sql_ hex32('s','q','l',0)
//
#define _ptmx_ hex32('p','t','m','x')
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
	union{
		u64 chip;
		void* pchip;
	};
	union{
		u64 foot;
		void* pfoot;
	};
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
		void* buf0;
	};
	union{
		u64 padd1;
		void* buf1;
	};
	union{
		u64 padd2;
		void* buf2;
		u64 len;
	};
	union{
		u64 padd3;
		void* buf3;
		void* buf;
	};

	//[80,ff]
	u8 data[0x80];
};




//
int actorread(  struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int actorwrite( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int actorstop(  struct halfrel* self,struct halfrel* peer);
int actorstart( struct halfrel* self,struct halfrel* peer);
int actordelete(void*);
void* actorcreate(u64 type, void* addr, int argc, char** argv);
//
int arenaread(  struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int arenawrite( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int arenastop(  struct halfrel* self,struct halfrel* peer);
int arenastart( struct halfrel* self,struct halfrel* peer);
int arenadelete(void*);
void* arenacreate(u64 type, void* addr, int argc, char** argv);
//
int arteryread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int arterywrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int arterystop( struct halfrel* self,struct halfrel* peer);
int arterystart(struct halfrel* self,struct halfrel* peer);
int arterydelete(void*);
void* arterycreate(u64 type, void* addr, int argc, char** argv);
//
int systemread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int systemwrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int systemstop( struct halfrel* self,struct halfrel* peer);
int systemstart(struct halfrel* self,struct halfrel* peer);
int systemdelete(void*);
void* systemcreate(u64 type, void* addr, int argc, char** argv);
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
double sine(double);
double cosine(double);
double arcsin(double);
double arccos(double);
double arctan2(double, double);
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
