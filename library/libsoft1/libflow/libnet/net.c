#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int serve_first(u64 fd, u64 type, u8* buf, u64 len);
int serve_chat( u64 fd, u64 type, u8* buf, u64 len);
int serve_ssh(  u64 fd, u64 type, u8* buf, u64 len);
int serve_tls(  u64 fd, u64 type, u8* buf, u64 len);
int serve_rdp(  u64 fd, u64 type, u8* buf, u64 len);
int serve_vnc(  u64 fd, u64 type, u8* buf, u64 len);
int serve_http( u64 fd, u64 type, u8* buf, u64 len);
int serve_ws(   u64 fd, u64 type, u8* buf, u64 len);
int serve_https(u64 fd, u64 type, u8* buf, u64 len);
int serve_wss(  u64 fd, u64 type, u8* buf, u64 len);
int http_read(void*, int);
int http_write_request(void*, int, void*, void*);
int websocket_read_handshake(void*, int);
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
int readsocket(u64, void*, int, int);
int writesocket(u64, void*, int, int);
//
void printmemory(char*, int);
void say(char*, ...);




struct object
{
        //[0x00,0x0f]
        u64 type0;      //raw, bt, udp, tcp?
        u64 type1;      //ssh, tls?

        //[0x10,0x1f]
        u64 port_src;
        u64 port_dst;

        //[0x20,0x3f]
        u8 addr_src[0x10];
        u8 addr_dst[0x10];

	//[0x40,0xff]
	u8 data[0xc0];
};
struct object* obj;
static u8* fshome = 0;
static u8* dirhome = 0;
static u8* datahome = 0;




u64 serve_what(u64 fd, u64 type, u8* buf, int len)
{
	//0
	if(type == 0)
	{
		type = serve_first(fd, type, buf, len);
	}

#define chat 0x74616863
	if(type==chat)
	{
		type = serve_chat(fd, type, buf, len);
	}

#define HTTP 0x50545448		//connection
#define http 0x70747468		//client
	else if( (type==HTTP) | (type==http) )
	{
		type = serve_http(fd, type, buf, len);
	}

#define WS 0x5357
#define ws 0x7377
	else if( (type==WS) | (type==ws) )
	{
		type = serve_ws(fd, type, buf, len);
	}

#define TLS 0x534c54
#define tls 0x736c74
	else if( (type==TLS) | (type==tls) )
	{
		type = serve_tls(fd, type, buf, len);
	}

#define HTTPS 0x5350545448
#define https 0x7370747468
	else if( (type==HTTPS) | (type==https) )
	{
		type = serve_https(fd, type, buf, len);
	}

#define WSS 0x535357
#define wss 0x737377
	else if( (type==WSS) | (type==wss) )
	{
		type = serve_wss(fd, type, buf, len);
	}

#define SSH 0x485353
#define ssh 0x687373
	else if( (type==SSH) | (type==ssh) )
	{
		type = serve_ssh(fd, type, buf, len);
	}

#define SOCKS 0x534b434f53
#define socks 0x736b636f73
	else if( (type==SOCKS) | (type==socks) )
	{
		//type = serve_socks(fd, type, buf, len);
	}

#define RDP 0x504452
#define rdp 0x706472
	else if( (type==RDP) | (type==rdp) )
	{
		type = serve_rdp(fd, type, buf, len);
	}

#define VNC 0x434e56
#define vnc 0x636e76
	else if( (type==VNC) | (type==vnc) )
	{
		type = serve_vnc(fd, type, buf, len);
	}

	return type;
}
void network_explain(u64* p)
{
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
		int len = readsocket(where, datahome, 0, 0x100000);
		say("@@@@ %llx %d\n", where, len);
		if(len > 0)
		{
			datahome[len] = 0;
			what = serve_what(where, obj[where].type1, datahome, len);
			if(what != 0)
			{
				obj[where].type1 = what;
				return;
			}
		}

		//wrong(len) or wrong(type)
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
			say("%d:	%llx,%llx\n",
				j, obj[j].type0, obj[j].type1);
		}
		return 0;
	}

	decstr2data(p, &fd);
	say("%llx,%llx\n", obj[fd].type0, obj[fd].type1);
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
	ret = buf2net(p, 256, buf, addr, &port, url);
	if(ret <= 0)return 0;
	say("type=%s, addr=%s, port=%d, extra=%s\n", buf, addr, port, url);

	//compare
	if(ncmp(buf, "raw", 3) == 0)
	{
		startsocket("0,0,0,0", 2222, 'r');	//tcp server
	}
	else if(ncmp(buf, "UDP", 3) == 0)
	{
		startsocket("0,0,0,0", 2222, 'U');	//tcp server
	}
	else if(ncmp(buf, "TCP", 3) == 0)
	{
		startsocket("0,0,0,0", 2222, 'T');	//tcp server
	}
	else if(ncmp(buf, "udp", 3) == 0)
	{
		fd = startsocket(addr, port, 'u');
	}
	else
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return -1;

		if(ncmp(buf, "http", 4) == 0)
		{
			obj[fd].type1 = http;
			ret = http_write_request(datahome, 0x100000, url, addr);
			ret = writesocket(fd, datahome, 0, ret);
		}
		else if(ncmp(buf, "ws", 2) == 0)
		{
			obj[fd].type1 = ws;
			ret = websocket_write_handshake(datahome, 0x100000);
			ret = writesocket(fd, datahome, 0, ret);
		}
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
	fshome = w + 0x100000;
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
