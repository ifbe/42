#include "artery.h"
#define _uart_ hex32('u','a','r','t')
//
int tftp_write(void*, int);
int tls_write_client_hello(void*, int);
int secureshell_write_handshake(void*, int);
int websocket_write_handshake(void*, int);
int http_write_request(void*, int, void*, void*);
int dns_write_query(void*, int, void*, int);
void quic_start();
void quic_stop();
void ssh_start();
void ssh_stop();
void tls_start();
void tls_stop();
//
u64 netmgr_eth(void*, int, void*, int);
u64 netmgr_udp(void*, int, void*, int);
u64 netmgr_tcp(void*, int, void*, int);
//
int systemcreate(u64 type, u8* name);
int systemdelete(int);
int startsocket(void* addr, int port, int type);
int stopsocket(int);
int readsocket(int fd, void* buf, int off, int len);
int writesocket(int fd, void* buf, int off, int len);
int startfile(void*, int);
int stopfile(int);
int readfile(int fd, void* buf, int off, int len);
int writefile(int fd, void* buf, int off, int len);
//
int parseurl(u8* buf, int len, u8* addr, int* port);
int ncmp(void*, void*, int);
int cmp(void*, void*);



#define datahome qqq
static struct object* obj = 0;
static void* ppp = 0;
static struct element* ele = 0;
static void* qqq = 0;
static int elelen = 0;
static int qqqlen = 0;
void* allocelement()
{
	
}




int arterydelete(int fd)
{
	if(_file_ == obj[fd].type)
	{
		stopfile(fd);
	}
	else
	{
		stopsocket(fd);
	}
	return 0;
}
int arterycreate(u64 type, u8* name)
{
	int j,k,fd,ret;
	u8 host[0x100];	//127.0.0.1
	int port;	//2222
	u8* url;	//dir/file.html
	u8* t;		//http

	if(0 == type)
	{
		for(j=0;j<0x1000;j++)
		{
			if(0 == ncmp(name+j, "://", 3))
			{
				t = (u8*)&type;
				for(k=0;k<j;k++)
				{
					if(k >= 8)break;
					t[k] = name[k];
				}
				name += j+3;
				break;
			}
		}
	}
	if(0 == type)return 0;
	if(_uart_ == type)return systemcreate(_uart_, name);
	else if(_FILE_ == type)return systemcreate(_FILE_, name);
	else if(_file_ == type)return systemcreate(_file_, name);
	else if(_RAW_ == type)return systemcreate(_RAW_, name);
	else if(_raw_ == type)return systemcreate(_raw_, name);
	else if(_UDP_ == type)return systemcreate(_UDP_, name);
	else if(_udp_ == type)return systemcreate(_udp_, name);
	else if(_TCP_ == type)return systemcreate(_TCP_, name);
	else if(_tcp_ == type)return systemcreate(_tcp_, name);

	//decode ipaddr
	port = 80;
	url = name + parseurl(name, 0x100, host, &port);
	say("host=%s,port=%d,url=%s\n", host, port, url);

	//raw family
	if(_ICMP_ == type)
	{
	}
	else if(_icmp_ == type)
	{
	}

	//udp family
	else if(_DNS_ == type)	//DNS server
	{
		fd = startsocket(host, port, 'U');
		if(0 >= fd)return 0;

		obj[fd].name = _DNS_;
	}
	else if(_dns_ == type)	//DNS client
	{
		fd = startsocket(host, port, 'u');
		if(0 >= fd)return 0;

		obj[fd].name = _dns_;
		ret = dns_write_query(datahome, 666, url+1, 666);
		writesocket(fd, datahome, 0, ret);
	}
	else if(_HOLE_ == type)	//p2p server
	{
		fd = startsocket(host, port, 'U');
		if(0 >= fd)return 0;

		obj[fd].name = _HOLE_;
	}
	else if(_hole_ == type)	//p2p client
	{
		fd = startsocket(host, port, 'u');
		if(0 >= fd)return 0;

		obj[fd].name = _hole_;
		writesocket(fd, url+1, 0, 16);
	}
	else if(_TFTP_ == type)	//tftp server
	{
		fd = startsocket(host, port, 'U');
		if(0 >= fd)return 0;

		obj[fd].name = _TFTP_;
	}
	else if(_tftp_ == type)	//tftp client
	{
		fd = startsocket(host, port, 'u');
		if(0 >= fd)return 0;

		obj[fd].name = _tftp_;
		ret = tftp_write(datahome, 0x100000);
		ret = writesocket(fd, datahome, 0, ret);
	}

	//tcp family
	else if(_SSH_ == type)	//ssh server
	{
		ssh_start();
		fd = startsocket(host, port, 'T');
		if(0 >= fd)return 0;

		obj[fd].name = _SSH_;
	}
	else if(_ssh_ == type)	//ssh client
	{
		fd = startsocket(host, port, 't');
		if(0 >= fd)return 0;

		obj[fd].name = _ssh_;
		ret = secureshell_write_handshake(datahome, 0x100000);
		ret = writesocket(fd, datahome, 0, ret);
	}
	else if(_TLS_ == type)	//tls server
	{
		tls_start();
		fd = startsocket(host, port, 'T');
		if(0 >= fd)return 0;

		obj[fd].name = _TLS_;
	}
	else if(_tls_ == type)	//tls client
	{
		fd = startsocket(host, port, 't');
		if(0 >= fd)return 0;

		obj[fd].name = _tls_;
		ret = tls_write_client_hello(datahome, 0x100000);
		ret = writesocket(fd, datahome, 0, ret);
	}
	else if(_sql_ == type)	//sql client
	{
		fd = startsocket(host, port, 't');
		if(0 >= fd)return 0;

		obj[fd].name = _sql_;
	}
	else if(_http_ == type)	//http client
	{
		fd = startsocket(host, port, 't');
		if(0 >= fd)return 0;

		obj[fd].name = _http_;
		ret = http_write_request(datahome, 0x100000, url, host);
		printmemory(datahome, ret);
		ret = writesocket(fd, datahome, 0, ret);
	}
	else if(_ws_ == type)	//ws client
	{
		fd = startsocket(host, port, 't');
		if(0 >= fd)return 0;

		obj[fd].name = _ws_;
		ret = websocket_write_handshake(datahome, 0x100000);
		ret = writesocket(fd, datahome, 0, ret);
	}

	return fd;
}
int arterystop()
{
	return 0;
}
int arterystart()
{
	return 0;
}
void* arteryread(int fd)
{
	return &obj[fd];
}
int arterywrite(struct event* ev)
{
	return 0;
}
/*
	//raw
	if(type == 'R')
	{
		len = readsocket(where, datahome, 0, 0x100000);
		if(len <= 0)return 0;

		netmgr_eth(obj, where, datahome, len);
		return 0;
	}

	//udp
	if( (type == 'U')|(type == 'u') )
	{
		while(1)
		{
			len = readsocket(where, datahome, 0, 0x100000);
			if(len <= 0)return 0;

			netmgr_udp(obj, where, datahome, len);
		}
		return 0;
	}

	//read socket
	len = readsocket(where, datahome, 0, 0x100000);
	if(len == 0)return 0;		//sticky
	if(len < 0)goto fail;		//wrong
printmemory(datahome, len);
	//serve socket
	what = netmgr_tcp(obj, where, datahome, len);
	if(what == 0)goto fail;

	//change event
	obj[where].name = what;
	if(_WS_ == what)
	{
		ev->why = len;
		ev->what = hex32('w','@',0,0);
		return 42;
	}
	else if(_http_ == what)
	{
		ev->why = len;
		ev->what = _http_;
	}
	return 0;

fail:
	stopsocket(where);
	return 0;
}*/
int arterylist(u8* buf, int len)
{
	int j,k=0;
	void* addr;
	for(j=0;j<0x1000;j++)
	{
		if(0 == ele[j].type)continue;

		k++;
		addr = (void*)(&ele[j]);
		say("[%03x]: %.8s,%.8s\n", j, addr, addr+8);
	}

	if(0 == k)say("empth artery\n");
	return 0;
}
int arterychoose(u8* buf, int len)
{
	int j;
	u8 data[0x1000];
	if(0 == len)
	{
		arterycreate(0, buf);
	}
	else
	{
		for(j=0;j<len;j++)
		{
			if(0 == buf[j])break;
			data[j] = buf[j];
		}
		data[j] = 0;

		arterycreate(0, data);
	}
	return 0;
}




void freeartery()
{
	//say("[8,c):freeing artery\n");

	qqq = 0;
	ele = 0;
	ppp = 0;
	obj = 0;
}
void initartery(void* addr)
{
	obj = addr+0x000000;
	ele = addr+0x100000;
	ppp = addr+0x200000;
	qqq = addr+0x300000;

	//say("[8,c):inited artery\n");
}