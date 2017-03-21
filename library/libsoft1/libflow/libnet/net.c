#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int check_ssh(  void* p, int fd, void* buf, int len);
int check_tls(  void* p, int fd, void* buf, int len);
int check_http( void* p, int fd, void* buf, int len);
int check_rtmp( void* p, int fd, void* buf, int len);
//
int serve_raw(  void* p, int fd, void* buf, int len);
int serve_chat( void* p, int fd, void* buf, int len);
int serve_ssh(  void* p, int fd, void* buf, int len);
int serve_tls(  void* p, int fd, void* buf, int len);
int serve_rdp(  void* p, int fd, void* buf, int len);
int serve_vnc(  void* p, int fd, void* buf, int len);
int serve_http( void* p, int fd, void* buf, int len);
int serve_https(void* p, int fd, void* buf, int len);
int serve_ws(   void* p, int fd, void* buf, int len);
int serve_wss(  void* p, int fd, void* buf, int len);
int serve_rtmp( void* p, int fd, void* buf, int len);
//
int tftp_write(void*, int);
int http_write_request(void*, int, void*, void*);
int websocket_write_handshake(void*, int);
//
int buf2net(u8* p, int max, u8* type, u8* addr, int* port, u8* extra);
int movsb(void*, void*, int);
int decstr2data(void*, void*);
int hexstr2data(void*, void*);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
int startsocket(void* addr, int port, int type);
int stopsocket(u64);
int readsocket(int, void*, int, int);
int writesocket(int, void*, int, int);
//
void printmemory(void*, int);
void say(void*, ...);




struct object
{
	//[0x00,0x0f]
	u64 type_sock;	//raw, bt, udp, tcp?
	u64 stage0;
	u64 type_road;	//ssh, tls?
	u64 stage1;
	u64 type_app;	//http2, ws, rdp, vnc?
	u64 stage2;
	u64 type_data;	//html, rgb?
	u64 stage3;

	//[0x40,0x7f]
	u8 addr_src[0x20];
	u8 addr_dst[0x20];

	//[0x80,0xff]
	u8 data[0x80];
};
struct filesys
{
	u8 data[256];
};
static struct object* obj;
static struct filesys* fs = 0;
static u8* dirhome = 0;
static u8* datahome = 0;




//capital(uppercase):		server
//small(lowercase):		client
#define RAW 0x574152		//parse, print
#define raw 0x776172		//send raw packet
#define CHAT 0x54414843		//check, broadcast
#define chat 0x74616863		//send user msg
#define HTTP 0x50545448		//parse, reply
#define http 0x70747468		//req url, get reply
#define HTTPS 0x5350545448	//parse, reply
#define https 0x7370747468	//req url, get reply
#define WS 0x5357		//c 
#define ws 0x7377		//s
#define WSS 0x535357		//s
#define wss 0x737377		//c
#define TLS 0x534c54		//s
#define tls 0x736c74		//c
#define SSH 0x485353		//s
#define ssh 0x687373		//c
#define SOCKS 0x534b434f53	//s
#define socks 0x736b636f73	//c
#define RDP 0x504452		//s
#define rdp 0x706472		//c
#define VNC 0x434e56		//s
#define vnc 0x636e76		//c
#define RTMP 0x504d5452		//s
#define rtmp 0x706d7472		//c
u64 serve_what(int fd, u8* buf, int len)
{
	int ret;
	u64 type = obj[fd].type_road;
	if(type != 0)goto protocol;




handshake:
	obj[fd].stage1 = 0;

	type = check_ssh(obj, fd, buf, len);
	if(type != 0)return type;

	type = check_tls(obj, fd, buf, len);
	if(type != 0)goto protocol;

	type = check_rtmp(obj, fd, buf, len);
	if(type != 0)goto protocol;

	type = check_http(obj, fd, buf, len);
	if(type != 0)goto protocol;

	if(type == 0)return 0;




protocol:
	if(type == RAW)
	{
		type = serve_raw(obj, fd, buf, len);
	}
	else if(type==CHAT)
	{
		type = serve_chat(obj, fd, buf, len);
	}
	else if( (type==HTTP) | (type==http) )
	{
		type = serve_http(obj, fd, buf, len);
	}
	else if( (type==WS) | (type==ws) )
	{
		type = serve_ws(obj, fd, buf, len);
	}
	else if( (type==TLS) | (type==tls) )
	{
		type = serve_tls(obj, fd, buf, len);
	}
	else if( (type==HTTPS) | (type==https) )
	{
		type = serve_https(obj, fd, buf, len);
	}
	else if( (type==WSS) | (type==wss) )
	{
		type = serve_wss(obj, fd, buf, len);
	}
	else if( (type==SSH) | (type==ssh) )
	{
		type = serve_ssh(obj, fd, buf, len);
	}
	else if( (type==SOCKS) | (type==socks) )
	{
		//type = serve_socks(obj, fd, buf, len);
	}
	else if( (type==RDP) | (type==rdp) )
	{
		type = serve_rdp(obj, fd, buf, len);
	}
	else if( (type==VNC) | (type==vnc) )
	{
		type = serve_vnc(obj, fd, buf, len);
	}
	else if( (type==RTMP) | (type==rtmp) )
	{
		type = serve_rtmp(obj, fd, buf, len);
	}
	else printmemory(buf, len);

	return type;
}
void network_explain(u64* p)
{
	int ret;
	//u64 why = p[0];
	u64 what = p[1] & 0xffff;
	u64 where = p[2];
	//u64 when = p[3];

	if(what == 0x2b6e)
	{
	}
	else if(what == 0x2d6e)
	{
	}
	else if(what == 0x406e)
	{
		//get data
		ret = readsocket(where, datahome, 0, 0x100000);
		if(ret > 0)
		{
			datahome[ret] = 0;
			what = serve_what(where, datahome, ret);
			if(what != 0)
			{
				obj[where].type_road = what;
				return;
			}
		}

		//wrong(ret) or wrong(type)
		stopsocket(where);
	}
}








int net_read()
{
	return 0;
}
int net_write()
{
	return 0;
}
int net_list(u8* p)
{
	int j;
	u64 fd=0;
	if(p==0)
	{
		for(j=0;j<1024;j++)
		{
			if(obj[j].type_sock == 0)continue;

			say("%d:	%llx,%llx\n",
				j, obj[j].type_sock, obj[j].type_road);
		}
		return 0;
	}

	decstr2data(p, &fd);
	say("%llx,%llx\n", obj[fd].type_sock, obj[fd].type_road);
	return 0;
}
int net_choose(u8* p)
{
	//
	u64 fd;
	int ret;

	//
	u8 buf[256];
	int port;
	u8* addr = buf+0x10;
	u8* url = buf+0x80;

	//parse
	url[0] = 0;
	ret = buf2net(p, 256, buf, addr, &port, url);
	if(ret <= 0)return 0;
	say("type=%s, addr=%s, port=%d, extra=%s\n", buf, addr, port, url);

	//compare
	if(ncmp(buf, "RAW", 3) == 0)
	{
		fd = startsocket("0,0,0,0", 2222, 'r');	//tcp server
		if(fd == 0)return 0;

		obj[fd].type_road = RAW;
	}
	else if(ncmp(buf, "UDP", 3) == 0)
	{
		fd = startsocket("0,0,0,0", 2222, 'U');	//tcp server
		if(fd == 0)return 0;

		obj[fd].type_road = CHAT;
	}
	else if(ncmp(buf, "TCP", 3) == 0)
	{
		fd = startsocket("0,0,0,0", 2222, 'T');	//tcp server
		if(fd == 0)return 0;
	}
	else if(ncmp(buf, "udp", 3) == 0)
	{
		fd = startsocket(addr, port, 'u');
		if(fd == 0)return 0;

		obj[fd].type_road = CHAT;
	}
	else if(ncmp(buf, "tcp", 3) == 0)
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;

		obj[fd].type_road = CHAT;
	}
	else if(ncmp(buf, "tftp", 4) == 0)
	{
		fd = startsocket(addr, port, 'u');
		if(fd == 0)return 0;

		obj[fd].type_road = CHAT;
		ret = tftp_write(datahome, 0x100000);
		ret = writesocket(fd, datahome, 0, ret);
	}
	else if(ncmp(buf, "http", 4) == 0)
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;

		obj[fd].type_road = http;
		ret = http_write_request(datahome, 0x100000, url, addr);
		ret = writesocket(fd, datahome, 0, ret);
	}
	else if(ncmp(buf, "ws", 2) == 0)
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;

		obj[fd].type_road = ws;
		ret = websocket_write_handshake(datahome, 0x100000);
		ret = writesocket(fd, datahome, 0, ret);
	}
	return 0;
}
int net_stop(u8* p)
{
	int j;
	u64 fd=0;
	if(p==0)
	{
		for(j=6;j<1024;j++)
		{
			stopsocket(j);
			return 0;
		}
	}

	decstr2data(p, &fd);
	stopsocket(fd);

	//say("[%d]out\n",fd);
	return 0;
}
int net_start(u64 fd)
{
	//say("[%d]in\n",fd);
	return 0;
}
int net_delete()
{
	return 0;
}
int net_create(void* w, u64* p)
{
	obj = w + 0x000000;
	fs = w + 0x100000;
	dirhome = w + 0x200000;
	datahome = w + 0x300000;

	p[0]=0x776f6c66;
	p[1]=0x74656e;
	p[2]=(u64)net_start;
	p[3]=(u64)net_stop;
	p[4]=(u64)net_list;
	p[5]=(u64)net_choose;
	p[6]=(u64)net_read;
	p[7]=(u64)net_write;

	return 0x100;
}
