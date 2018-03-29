#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define _fd_ hex32('f','d',0,0)
#define _uart_ hex32('u','a','r','t')
#define _FILE_ hex32('F','I','L','E')
#define _file_ hex32('f','i','l','e')
//
#define _RAW_ hex32('R','A','W',0)
#define _raw_ hex32('r','a','w',0)
#define _UDP_ hex32('U','D','P',0)
#define _udp_ hex32('u','d','p',0)
#define _TCP_ hex32('T','C','P',0)
#define _tcp_ hex32('t','c','p',0)
#define _CHAT_ hex32('C','H','A','T')
#define _chat_ hex32('c','h','a','t')
//
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
//ip
#define _ICMP_ 0x504d4349		//s
#define _icmp_ 0x706d6369		//c
#define _IGMP_ 0x504d4749		//s
#define _igmp_ 0x706d6769		//c
//tran
#define _QUIC_ 0x43495551		//s
#define _quic_ 0x63697571		//c
#define _SOCKS5_ 0x35534b434f53	//s
#define _socks5_ 0x35736b636f73	//c
#define _SSH_ 0x485353		//s
#define _ssh_ 0x687373		//c
#define _TLS_ 0x534c54		//s
#define _tls_ 0x736c74		//c
//udp
#define _BOOTP_ 0x50544f4f42	//s
#define _bootp_ 0x70746f6f62	//c
#define _DNS_ 0x534e44		//s
#define _dns_ 0x736e64		//c
#define _TFTP_ 0x50544654		//s
#define _tftp_ 0x70746674		//c
#define _WEBRTC_ 0x435452424557	//s
#define _webrtc_ 0x637472626577	//c
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
#define _HTTP_ 0x50545448		//parse, reply
#define _http_ 0x70747468		//req url, get reply
#define _HTTPS_ 0x5350545448	//parse, reply
#define _https_ 0x7370747468	//req url, get reply
#define _WS_ 0x5357		//s 
#define _ws_ 0x7377		//c
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
struct element
{
	//[00,20]
	u32 tier;
	u32 type;	//http, tls, ssh
	u64 stage1;
	union{
		void* irel;
		u64 pad0;
	};
	union{
		void* orel;
		u64 pad1;
	};

	//[20,3f]
	u64 fd;
	u64 flag;
	u64 len;
	union{
		u64 addr;
		void* buf;
	};

	//[40,ff]
	u8 data[0xc0];
};




int actorread(void* dc,void* df,void* sc,void* sf);
int actorwrite(void* dc,void* df,void* sc,void* sf,void* buf, int len);
int arenaread(void* dc,void* df,void* sc,void* sf);
int arenawrite(void* dc,void* df,void* sc,void* sf,void* buf, int len);
int arteryread(void* dc,void* df,void* sc,void* sf);
int arterywrite(void* dc,void* df,void* sc,void* sf,void* buf, int len);
int systemread(void* dc,void* df,void* sc,void* sf);
int systemwrite(void* dc,void* df,void* sc,void* sf,void* buf, int len);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
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
void* relationcreate(void*,void*,u64,void*,void*,u64);
int relationdelete(void*);
//
void* eventread();
void* eventwrite(u64,u64,u64,u64);
//
int mysnprintf(void*, int, void*, ...);
int printmemory(void*, int);
int say(void*, ...);