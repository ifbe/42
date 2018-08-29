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
	void* dc;
	void* df;
	struct relation* irel;
	struct relation* orel;
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
		irel = obj[where].irel0;
		orel = obj[where].orel0;
		relationdelete(irel);
		relationdelete(orel);
		return 0;
	}

	irel = obj[where].irel0;
	orel = obj[where].orel0;
	if((0 == irel)&&(0 == orel))
	{
		ret = obj[where].thatfd;
		irel = obj[ret].irel0;
		orel = obj[ret].orel0;
	}

	if(0 == orel)
	{
		ret = readsocket(where, tmp, ppp, 0x100000);
		if(ret == 0)return 0;
		if(ret < 0)
		{
			stopsocket(where);
			return 0;
		}

		what = obj[where].type;
		if((_UDP_ == what)|(_udp_ == what))
		{
			say("%d,%d,%d,%d:%d\n",
				tmp[4],tmp[5],tmp[6],tmp[7],
				(tmp[2]<<8)+tmp[3]
			);
		}
		printmemory(ppp, ret);
		return 0;
	}

	//say("%llx,%llx,%llx\n", orel->dstchip, orel->dstfoot, orel->dsttype);
	ret = readsocket(where, tmp, ppp, 0x100000);
	if(ret <= 0)return 0;

//printmemory(ppp, ret);
say("systemwrite_ev@%x{\n", ret);
	while(1)
	{
		if(0 == orel)break;

		dc = (void*)(orel->dstchip);
		df = (void*)(orel->dstfoot);
		if(_act_ == orel->dsttype)
		{
			actorwrite(dc, df, &obj[where], tmp, ppp, ret);
		}
		else if(_win_ == orel->dsttype)
		{
			arenawrite(dc, df, &obj[where], tmp, ppp, ret);
		}
		else if(_art_ == orel->dsttype)
		{
			arterywrite(dc, df, &obj[where], tmp, ppp, ret);
		}

		orel = samesrcnextdst(orel);
	}

say("}@systemwrite_ev\n");
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
	return writesocket(fd, df, buf, len);
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
	else if(_uart_ == type)
	{
		fd = startuart(name, 115200);
		if(fd <= 0)return 0;

		obj[fd].type = _uart_;
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
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].type)continue;

		k++;
		say("[%03x]: %.8s\n", j, &obj[j].type);
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
