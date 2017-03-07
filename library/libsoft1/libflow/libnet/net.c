#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int serve_first(u64 fd, u64 type, u8* buf, u64 len);
int serve_chat( u64 fd, u64 type, u8* buf, u64 len);
int serve_http( u64 fd, u64 type, u8* buf, u64 len);
int serve_ws(   u64 fd, u64 type, u8* buf, u64 len);
int serve_https(u64 fd, u64 type, u8* buf, u64 len);
int serve_wss(  u64 fd, u64 type, u8* buf, u64 len);
int serve_ssh(  u64 fd, u64 type, u8* buf, u64 len);
int http_read(void*, int);
int http_write(void*, int, void*, int);
int buf2net(u8* p, int max, u8* type, u8* addr, int* port, u8* extra);
int movsb(void*, void*, int);
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
        //0x20 = 4 * 8
        u64 type0;      //raw, bt, udp, tcp?
        u64 type1;      //ssh, tls?
        u64 type2;      //https, wss?
        u64 type3;

        //0x20 = 4 * 8
        u64 zero;
        u64 port_src;
        u64 port_xxx;
        u64 port_dst;

        //0xc0 = 3 * 0x40
        u8 addr_src[0x40];
        u8 addr_xxx[0x40];
        u8 addr_dst[0x40];
};
struct object* obj;
static u8* fshome = 0;
static u8* dirhome = 0;
static u8* datahome = 0;
char indexhtml[]="index.html";




int net_read(u64 fd)
{
	int count;
	u64 type;
	//say("[%d]before\n", fd);

	//read
	count = readsocket(fd, datahome, 0, 0x100000);
	if(count <= 0)
	{
		//say("[%d]stop\n",fd);
		stopsocket(fd);
		return;
	}
	datahome[count] = 0;
	//say("[%d]after net_read:%d\n", fd, count);

	//what
	type = obj[fd].type1;
	if(type == 0)
	{
		//
		type = serve_first(fd, type, datahome, count);
	}

//--------------------------------------------------------
/*
server:bit31=0, client:bit31=1
{
	00000000	default, chat
	00000100	http
	000002??	ws
	000003??	https
	000004??	wss
	000005??	ssh
	000006??	socks?
	000007??	rdp
	000008??	vnc
}
*/
//--------------------------------------------------------

	//chat
	if(type <= 0xff)
	{
		type = serve_chat(fd, type, datahome, count);
	}

	//http
	else if(type <= 0x1ff)
	{
		type = serve_http(fd, type, datahome, count);
	}

	//ws
	else if(type <= 0x2ff)
	{
		type = serve_ws(fd, type, datahome, count);
	}

	//https
	else if(type <= 0x3ff)
	{
		type = serve_https(fd, type, datahome, count);
	}

	//wss
	else if(type <= 0x4ff)
	{
		type = serve_wss(fd, type, datahome, count);
	}

	//ssh
	else if(type <= 0x5ff)
	{
		type = serve_ssh(fd, type, datahome, count);
	}

	//socks
	else if(type <= 0x6ff)
	{
		//type = serve_socks(fd, type, datahome, count);
	}

	//rdp
	else if(type <= 0x7ff)
	{
		//type = serve_rdp(fd, type, datahome, count);
	}

	//vnc
	else if(type <= 0x8ff)
	{
		//type = serve_vnc(fd, type, datahome, count);
	}

	//error
	else goto forceclose;

checkclose:
	if( (type>0) && (type<0x1000) )
	{
		obj[fd].type1 = type;
		return 0;
	}

forceclose:
	stopsocket(fd);
	return 0;
}
int net_write()
{
	return 0;
}
int net_list()
{
	return 0;
}
int net_choose(u8* p)
{
	//
	u64 fd;
	int ret;

	//
	u8 buf[128];
	int port;
	u8* ip = buf+0x10;
	u8* str = buf+0x80;

	//parse
	ret = buf2net(p, 128, buf, ip, &port, str);
	if(ret <= 0)return 0;
	say("type=%s, addr=%s, port=%d, extra=%s\n", buf, ip, port, str);

	//compare
	if(ncmp(buf, "TCP", 3) == 0)
	{
		startsocket("0,0,0,0", 2222, 'T');	//tcp server
	}
	else if(ncmp(buf, "raw", 3) == 0)
	{
		fd = startsocket(ip, port, 'r');
	}
	else if(ncmp(buf, "udp", 3) == 0)
	{
		fd = startsocket(ip, port, 'u');
	}
	else
	{
		fd = startsocket(ip, port, 't');
		if(fd == 0)return -1;

		if(ncmp(buf, "http", 4) == 0)
		{
			str = (u8*)indexhtml;
			ret = http_write(datahome, 0x100000, str, 10);
			ret = writesocket(fd, datahome, 0, ret);
		}
	}
	return 0;
}
int net_stop(u64 fd)
{
	say("[%d]out\n",fd);
	return 0;
}
int net_start(u64 fd)
{
	say("[%d]in\n",fd);
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
	p[10]=(u64)net_start;
	p[11]=(u64)net_stop;
	p[12]=(u64)net_list;
	p[13]=(u64)net_choose;
	p[14]=(u64)net_read;
	p[15]=(u64)net_write;

	return 0x80;
}




void network_explain(u64* p)
{
	u64 evfd = p[0];
	u64 type = p[1] & 0xffff;
	//say("%llx,%llx\n", evfd, type);

	if(type == 0x406e) net_read(evfd);
	else if(type == 0x2b6e) net_start(evfd);
	else if(type == 0x2d6e) net_stop(evfd);
}
