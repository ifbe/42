#include "libsoft.h"
#define _ptmx_ hex32('p','t','m','x')
//random
int createrandom(void*);
int deleterandom();
//signal
int createsignal(void*);
int deletesignal();
//epoll,iocp,kqueue
int createwatcher(void*);
int deletewatcher();
//socket
int createsocket(void*);
int deletesocket();
//shell
int createshell(void*);
int deleteshell();
int startshell(void*, int);
int stopshell(int);
//uart
int createuart(void*);
int deleteuart();
int startuart(void*, int);
int stopuart(int);
//i2c
int systemi2c_create(void*, int);
int systemi2c_delete(int);
//spi
int systemspi_create(void*, int);
int systemspi_delete(int);
//
int startfile(void*, int);
int stopfile(int);
int startsocket(void* addr, int port, int type);
int stopsocket(int);
//
int readfile(   int, int, void*, int);
int writefile(  int, int, void*, int);
int readuart(   int, int, void*, int);
int writeuart(  int, int, void*, int);
int readshell(  int, int, void*, int);
int writeshell( int, int, void*, int);
int readsocket( int,void*,void*, int);
int writesocket(int,void*,void*, int);
//
int parseurl(u8* buf, int len, u8* addr, int* port);
int ncmp(void*, void*, int);
int cmp(void*, void*);




//
static struct object* obj = 0;
static void* ppp = 0;
static int objlen = 0;
static int ppplen = 0;




int systemwrite_in(struct object* chip, u8* foot, u8* buf, int len)
{
	int ret;
	void* dc;
	void* df;
	struct relation* irel;
	struct relation* orel;

	irel = chip->irel0;
	orel = chip->orel0;
	if((0 == irel)&&(0 == orel))
	{
		ret = chip->thatfd;
		irel = obj[ret].irel0;
		orel = obj[ret].orel0;
	}

	if(0 == orel)
	{
		ret = chip->type;
		if((_UDP_ == ret)|(_udp_ == ret))
		{
			say("%d,%d,%d,%d:%d\n",
				foot[4], foot[5], foot[6], foot[7],
				(foot[2]<<8)+foot[3]
			);
		}
		printmemory(buf, len);
		return 0;
	}

	while(1)
	{
		if(0 == orel)break;

		dc = (void*)(orel->dstchip);
		df = (void*)(orel->dstfoot);
		if(_act_ == orel->dsttype)
		{
			actor_rootwrite(dc, df, chip, foot, buf, len);
		}
		else if(_win_ == orel->dsttype)
		{
			arena_rootwrite(dc, df, chip, foot, buf, len);
		}
		else if(_art_ == orel->dsttype)
		{
			artery_rootwrite(dc, df, chip, foot, buf, len);
		}

		orel = samesrcnextdst(orel);
	}

	return 42;
}
int systemevent(struct event* ev)
{
	int ret;
	u64 why = ev->why;
	u64 what = ev->what;
	u64 where = ev->where;
	u8 tmp[0x40];
	//say("%llx,%llx,%llx\n",why,what,where);

	if(why == '+')
	{
		say("come:%x(from:%x)\n", where, obj[where].thatfd);
		return 0;
	}
	else if(why == '-')
	{
		say("gone:%x\n", where);
		return 0;
	}

	ret = readsocket(where, tmp, ppp, 0x100000);
	if(ret <= 0)return 0;

	return systemwrite_in(&obj[where], tmp, ppp, ret);
}
void* systemcommand(u8* buf, int len)
{
	int j;
	u8* src;
	u8 data[0x1000];
	if(0 == len)
	{
		systemcreate(0, buf);
	}
	else
	{
		src = buf;
		for(j=0;j<len;j++)
		{
			if(0 == src[j])break;
			data[j] = src[j];
		}
		data[j] = 0;

		systemcreate(0, data);
	}
	return 0;
}




int systemread_all()
{
	return 0;
}
void* systemlist(u8* buf, int len)
{
	int j,k=0;
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].type)continue;

		k++;
		say("[%03x]: %.8s\n", j, &obj[j].type);
	}

	if(0 == k)say("empth system\n");
	return 0;
}




int system_rootwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	return 0;
}
int system_rootread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	return 0;
}
int system_leafwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	if(0 != sc)
	{
		int fd = (dc - (void*)obj) / sizeof(struct object);
		if(_uart_ == obj[fd].type)
		{
			return writeuart(fd, 0, buf, len);
		}
		if(_ptmx_ == obj[fd].type)
		{
			return writeshell(fd, 0, buf, len);
		}
		else
		{
			return writesocket(fd, df, buf, len);
		}
	}

	say("systemwrite@{\n");
	systemwrite_in(dc, df, buf, len);
	say("}@systemwrite\n");

	return 0;
}
int system_leafread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	if(0 == sc)return systemread_all();
	return 0;
}
int systemstop()
{
	return 0;
}
int systemstart()
{
	return 0;
}
int systemdelete(void* addr)
{
	struct object* o;
	int fd = (addr - (void*)obj) / sizeof(struct object);
	if(_file_ == obj[fd].type)
	{
		stopfile(fd);
	}
	else
	{
		stopsocket(fd);

		o = addr;
		if(0 != o->irel0)relationdelete(o->irel0);
		if(0 != o->orel0)relationdelete(o->orel0);

		o->type = 0;
		o->fmt = 0;
		o->name = 0;

		o->selffd = 0;
		o->thatfd = 0;

		o->irel0 = o->ireln = 0;
		o->orel0 = o->oreln = 0;
	}
	return 0;
}
void* systemcreate(u64 type, void* argstr)
{
	int j,k,fd,ret;
	u8 host[0x100];	//127.0.0.1
	int port;	//2222
	u8* url;	//dir/file.html
	u8* t;		//http

	u8* name = argstr;
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

	//file family
	if(_FILE_ == type)
	{
		fd = startfile(name, 'w');
		if(fd <= 0)return 0;

		obj[fd].type = _FILE_;
		goto success;
	}
	else if(_file_ == type)
	{
		fd = startfile(name, 'r');
		if(fd <= 0)return 0;

		obj[fd].type = _file_;
		goto success;
	}
	else if(_i2c_ == type)
	{
		fd = systemi2c_create(name, 0);
		if(fd <= 0)return 0;

		obj[fd].type = _i2c_;
		obj[fd].selffd = fd;

		goto success;
	}
	else if(_spi_ == type)
	{
		fd = systemspi_create(name, 0);
		if(fd <= 0)return 0;

		obj[fd].type = _spi_;
		goto success;
	}
	else if(_uart_ == type)
	{
		for(j=0;j<0x100;j++)
		{
			if(name[j] < 0x20)break;
			host[j] = name[j];
		}
		host[j] = 0;

		fd = startuart(host, 115200);
		if(fd <= 0)return 0;

		obj[fd].type = _uart_;
		goto success;
	}
	else if(_ptmx_ == type)
	{
		for(j=0;j<0x100;j++)
		{
			if(name[j] < 0x20)break;
			host[j] = name[j];
		}
		host[j] = 0;

		fd = startshell(host, 115200);
		if(fd <= 0)return 0;

		obj[fd].type = _ptmx_;
		goto success;
	}

	//decode ipaddr
	port = 80;
	url = name + parseurl(name, 0x100, host, &port);
	say("systemcreate: %.8s://%s:%d/%s\n", &type, host, port, url);

	if(_RAW_ == type)		//raw server
	{
		fd = startsocket(host, port, _RAW_);
		if(0 >= fd)return 0;

		obj[fd].type = _RAW_;
	}
	else if(_raw_ == type)	//raw client
	{
		fd = startsocket(host, port, _raw_);
		if(0 >= fd)return 0;

		obj[fd].type = _raw_;
	}
	else if(_UDP_ == type)	//udp server
	{
		fd = startsocket(host, port, _UDP_);
		if(0 >= fd)return 0;

		obj[fd].type = _UDP_;
	}
	else if(_udp_ == type)	//udp client
	{
		fd = startsocket(host, port, _udp_);
		if(0 >= fd)return 0;

		obj[fd].type = _udp_;
	}
	else if(_TCP_ == type)	//tcp server
	{
		fd = startsocket(host, port, _TCP_);
		if(0 >= fd)return 0;

		obj[fd].type = _TCP_;
	}
	else if(_tcp_ == type)	//tcp client
	{
		fd = startsocket(host, port, _tcp_);
		if(0 >= fd)return 0;

		obj[fd].type = _tcp_;
	}

success:
	return &obj[fd];
}




void freesystem()
{
	//say("[8,c):freeing system\n");

	deleteuart();
	deleteshell();
	deletesocket();
	deletewatcher();
	deletesignal();
	deleterandom();
}
void initsystem(u8* addr)
{
	int j;
	obj = (void*)(addr+0x000000);
	ppp = (void*)(addr+0x200000);

#define max (0x100000/sizeof(struct object))
	for(j=0;j<0x400000;j++)addr[j]=0;
	for(j=0;j<max;j++)obj[j].tier = _fd_;

	createrandom(addr);
	createsignal(addr);
	createwatcher(addr);
	createsocket(addr);
	createshell(addr);
	createuart(addr);

	//say("[8,c):inited system\n");
}
