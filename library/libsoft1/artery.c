#include "artery.h"
//
int file_create(void*);
int file_delete();
int file_cd(void*, int);
int file_write(void*);
int netmgr_create(void*);
int netmgr_delete();
int netmgr_cd(void*, int);
int netmgr_write(void*);
//
int startsocket(void* addr, int port, int type);
int stopsocket(u64);
int readsocket(int, void*, int, int);
int writesocket(int, void*, int, int);
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




int arterydelete()
{
	return 0;
}
int arterycreate()
{
	return 0;
}
int arterystop()
{
	return 0;
}
int arterystart(u64 type, u8* name)
{
	u8* t;
	int j,k,fd,ret;
	u8* addr;
	int port;
	u8* url;

	if(0 == type)
	{
		for(j=0;j<0x1000;j++)
		{
			if(0 == ncmp(name+j, "://", 3))
			{
				t = (u8*)&type;
				if(j > 8)j = 8;
				for(k=0;k<j;k++)
				{
					if(k >= 8)break;
					t[k] = name[j];
				}
				name += j+3;
				break;
			}
		}
	}
	if(0 == type)return 0;

	//parse
	say("type=%llx, name=%s\n", type, name);

	//raw family
	if(_RAW_ == type)		//raw server
	{
		fd = startsocket(addr, port, 'R');
		if(fd == 0)return 0;

		obj[fd].name = _RAW_;
	}
	else if(_raw_ == type)	//raw client
	{
		fd = startsocket(addr, port, 'r');
		if(fd == 0)return 0;

		obj[fd].name = _raw_;
	}
	else if(_ICMP_ == type)
	{
	}
	else if(_icmp_ == type)
	{
	}

	//udp family
	else if(_UDP_ == type)	//udp server
	{
		fd = startsocket(addr, port, 'U');
		if(fd == 0)return 0;

		obj[fd].name = _UDP_;
	}
	else if(_udp_ == type)	//udp client
	{
		fd = startsocket(addr, port, 'u');
		if(fd == 0)return 0;

		obj[fd].name = _udp_;
	}
	else if(_DNS_ == type)	//DNS server
	{
		fd = startsocket(addr, port, 'U');
		if(fd == 0)return 0;

		obj[fd].name = _DNS_;
	}
	else if(_dns_ == type)	//DNS client
	{
		fd = startsocket(addr, port, 'u');
		if(fd == 0)return 0;

		obj[fd].name = _dns_;
		ret = dns_write_query(datahome, 666, url+1, 666);
		writesocket(fd, datahome, 0, ret);
	}
	else if(_HOLE_ == type)	//p2p server
	{
		fd = startsocket(addr, port, 'U');
		if(fd == 0)return 0;

		obj[fd].name = _HOLE_;
	}
	else if(_hole_ == type)	//p2p client
	{
		fd = startsocket(addr, port, 'u');
		if(fd == 0)return 0;

		obj[fd].name = _hole_;
		writesocket(fd, url+1, 0, 16);
	}
	else if(_TFTP_ == type)	//tftp server
	{
		fd = startsocket(addr, port, 'U');
		if(fd == 0)return 0;

		obj[fd].name = _TFTP_;
	}
	else if(_tftp_ == type)	//tftp client
	{
		fd = startsocket(addr, port, 'u');
		if(fd == 0)return 0;

		obj[fd].name = _tftp_;
		ret = tftp_write(datahome, 0x100000);
		ret = writesocket(fd, datahome, 0, ret);
	}

	//tcp family
	else if(_TCP_ == type)	//tcp server
	{
		fd = startsocket(addr, port, 'T');
		if(fd == 0)return 0;

		obj[fd].name = _TCP_;
	}
	else if(_tcp_ == type)	//tcp client
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;

		obj[fd].name = _tcp_;
	}
	else if(_SSH_ == type)	//ssh server
	{
		ssh_start();
		fd = startsocket(addr, port, 'T');
		if(fd == 0)return 0;

		obj[fd].name = _SSH_;
	}
	else if(_ssh_ == type)	//ssh client
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;

		obj[fd].name = _ssh_;
		ret = secureshell_write_handshake(datahome, 0x100000);
		ret = writesocket(fd, datahome, 0, ret);
	}
	else if(_TLS_ == type)	//tls server
	{
		tls_start();
		fd = startsocket(addr, port, 'T');
		if(fd == 0)return 0;

		obj[fd].name = _TLS_;
	}
	else if(_tls_ == type)	//tls client
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;

		obj[fd].name = _tls_;
		ret = tls_write_client_hello(datahome, 0x100000);
		ret = writesocket(fd, datahome, 0, ret);
	}
	else if(_sql_ == type)	//sql client
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;

		obj[fd].name = _sql_;
	}
	else if(_http_ == type)	//http client
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;

		obj[fd].name = _http_;
		ret = http_write_request(datahome, 0x100000, url, addr);
		ret = writesocket(fd, datahome, 0, ret);
	}
	else if(_ws_ == type)	//ws client
	{
		fd = startsocket(addr, port, 't');
		if(fd == 0)return 0;

		obj[fd].name = _ws_;
		ret = websocket_write_handshake(datahome, 0x100000);
		ret = writesocket(fd, datahome, 0, ret);
	}

	return fd;
}
void* arteryread(int fd)
{
	return &obj[fd];
}
int arterywrite(struct event* ev)
{
	return netmgr_write(ev);
}
int arterylist(u8* buf, int len)
{
	say("empth artery\n");
	return 0;
}
int arterychoose(u8* buf, int len)
{
	int j;
	u8* type = 0;
	u8* name = buf;

	for(j=0;j<0x1000;j++)
	{
		if(0 == ncmp(buf+j, "://", 3))
		{
			say("type=%.*s, name=%.*s\n", j, buf, 256, buf+j+3);
			type = buf;
			name = buf+j+3;
			break;
		}
	}

	if(0 == type)
	{
		file_cd(name, _file_);
	}
	else if(ncmp(type, "file", 4) == 0)
	{
		file_cd(name, _file_);
	}
	else if(ncmp(type, "FILE", 4) == 0)
	{
		file_cd(name, _FILE_);
	}
	else
	{
		netmgr_cd(buf, 0);
	}
	return 0;
}




void freeartery()
{
	//say("[8,c):freeing artery\n");

	netmgr_delete();
	file_delete();

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

	file_create(addr);
	netmgr_create(addr);

	//say("[8,c):inited artery\n");
}