#include "libsoft.h"
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
//uart
int createuart(void*);
int deleteuart();
//
int startsocket(void* addr, int port, int type);
int stopsocket(int);
int startuart(void*, int);
int stopuart(int);
int startfile(void*, int);
int stopfile(int);
//
int parseurl(u8* buf, int len, u8* addr, int* port);
int ncmp(void*, void*, int);
int cmp(void*, void*);




//
static struct object* obj = 0;
static void* ppp = 0;
static int objlen = 0;
static int ppplen = 0;




int systemwrite_ev(struct event* ev)
{
	int ret;
	u64 type,name;
	struct relation* irel;
	struct relation* orel;
	u64 why = ev->why;
	u64 what = ev->what;
	u64 where = ev->where;
	say("%llx,%llx,%llx\n",why,what,where);

	if(why == '+')
	{
		say("come:%x(from:%x)\n", where, obj[where].thatfd);
		return 0;
	}
	else if(why == '-')
	{
		say("gone:%x\n", where);
		irel = obj[where].irel0;
		orel = obj[where].orel0;
		relationdelete(irel);
		relationdelete(orel);
		return 0;
	}

	type = obj[where].type;
	name = obj[where].name;
	irel = obj[where].irel0;
	orel = obj[where].orel0;
	if((0 == name)&&(0 == irel)&&(0 == orel))
	{
		ret = obj[where].thatfd;
		irel = obj[ret].irel0;
		orel = obj[ret].orel0;
	}
	//say("type=%llx,name=%llx,irel=%llx,orel=%llx\n", type, name, irel, orel);

	if(0 == orel)
	{
		ret = readsocket(where, 0, ppp, 0x100000);
		if(ret == 0)return 0;
		if(ret < 0)
		{
			stopsocket(where);
			return 0;
		}

		printmemory(ppp, ret);
		return 0;
	}

	//say("%llx,%llx,%llx\n", orel->dstchip, orel->dstfoot, orel->dsttype);
	ret = readsocket(where, 0, ppp, 0x100000);
	if(ret <= 0)return 0;

	while(1)
	{
		if(0 == orel)break;
		if(_act_ == orel->dsttype)
		{
			actorwrite(
				(void*)(orel->dstchip), (void*)(orel->dstfoot),
				&obj[where], 0,
				ppp, ret
			);
		}
		else if(_win_ == orel->dsttype)
		{
			arenawrite(
				(void*)(orel->dstchip), (void*)(orel->dstfoot),
				&obj[where], 0,
				ppp, ret
			);
		}
		else if(_art_ == orel->dsttype)
		{
			arterywrite(
				(void*)(orel->dstchip), (void*)(orel->dstfoot),
				&obj[where], 0,
				ppp, ret
			);
		}
		orel = samesrcnextdst(orel);
	}
	return 42;
}
int systemread_all()
{
	return 0;
}




int systemwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	if(0 == dc)return systemwrite_ev(buf);

	int fd = (dc - (void*)obj) / sizeof(struct object);
	return writesocket(fd, 0, buf, len);
}
int systemread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
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
		o = addr;
		if(0 != o->irel0)relationdelete(o->irel0);
		if(0 != o->orel0)relationdelete(o->orel0);
		stopsocket(fd);
	}
	return 0;
}
void* systemcreate(u64 type, u8* name)
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

	//file family
	if(_FILE_ == type)
	{
		fd = startfile(name, 'w');
		if(fd <= 0)return 0;

		obj[fd].type = _file_;
		obj[fd].name = _FILE_;
		goto success;
	}
	else if(_file_ == type)
	{
		fd = startfile(name, 'r');
		if(fd <= 0)return 0;

		obj[fd].type = _file_;
		obj[fd].name = _file_;
		goto success;
	}
	else if(_uart_ == type)
	{
		fd = startuart(name, 115200);
		if(fd <= 0)return 0;

		obj[fd].type = _uart_;
		obj[fd].name = _uart_;
		goto success;
		return 0;
	}

	//decode ipaddr
	port = 80;
	url = name + parseurl(name, 0x100, host, &port);
	say(
		"host=%s:%d\n"
		"url=%s\n",
		host, port,
		url
	);

	if(_RAW_ == type)		//raw server
	{
		fd = startsocket(host, port, 'R');
		if(0 >= fd)return 0;

		obj[fd].name = _RAW_;
	}
	else if(_raw_ == type)	//raw client
	{
		fd = startsocket(host, port, 'r');
		if(0 >= fd)return 0;

		obj[fd].name = _raw_;
	}
	else if(_UDP_ == type)	//udp server
	{
		fd = startsocket(host, port, 'U');
		if(0 >= fd)return 0;

		obj[fd].name = _UDP_;
	}
	else if(_udp_ == type)	//udp client
	{
		fd = startsocket(host, port, 'u');
		if(0 >= fd)return 0;

		obj[fd].name = _udp_;
	}
	else if(_TCP_ == type)	//tcp server
	{
		fd = startsocket(host, port, 'T');
		if(0 >= fd)return 0;

		obj[fd].name = _TCP_;
	}
	else if(_tcp_ == type)	//tcp client
	{
		fd = startsocket(host, port, 't');
		if(0 >= fd)return 0;

		obj[fd].name = _tcp_;
	}

success:
	return &obj[fd];
}
int systemchoose(u8* buf, int len)
{
	int j;
	u8 data[0x1000];
	if(0 == len)
	{
		systemcreate(0, buf);
	}
	else
	{
		for(j=0;j<len;j++)
		{
			if(0 == buf[j])break;
			data[j] = buf[j];
		}
		data[j] = 0;

		systemcreate(0, data);
	}
	return 0;
}
int systemlist(u8* buf, int len)
{
	int j,k=0;
	void* addr;
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].type)continue;

		k++;
		addr = (void*)(&obj[j]);
		say("[%03x]: %.8s,%.8s\n", j, addr, addr+8);
	}

	if(0 == k)say("empth system\n");
	return 0;
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
