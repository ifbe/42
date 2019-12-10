#include "libsoft.h"
#define _ptmx_ hex32('p','t','m','x')
//random
int initrandom(void*);
int freerandom();
//signal
int initsignal(void*);
int freesignal();
//epoll,iocp,kqueue
int initwatcher(void*);
int freewatcher();
//shell
int initshell(void*);
int freeshell();
int startshell(void*, int);
int stopshell(int);
int readshell( int, int, void*, int);
int writeshell(int, int, void*, int);
//uart
int inituart(void*);
int freeuart();
int uart_stop(int);
int uart_start(void*, int);
int uart_read( int, int, void*, int);
int uart_write(int, int, void*, int);
//socket
int initsocket(void*);
int freesocket();
int startsocket(void* addr, int port, int type);
int stopsocket(int);
int readsocket( int,void*,void*, int);
int writesocket(int,void*,void*, int);
//
int parseurl(u8* buf, int len, u8* addr, int* port);
int parseuart(void*, int*, void*);
int ncmp(void*, void*, int);
int cmp(void*, void*);




//
static struct object* obj = 0;
static int objlen = 0;
static void* ppp = 0;
static int ppplen = 0;




int systemread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct object* oo;
	int fd;
	if(0 == self)return 0;
	//say("@systemread:%llx\n", self);

	oo = self->pchip;
	if(0 == oo)return 0;

	switch(oo->type){
		case _FILE_:
		case _file_:return readfile(oo, oo->selffd, arg, idx, buf, len);
	}
	return 0;
}
int systemwrite(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	int fd;
	struct object* oo;
	//say("@systemwrite:%llx\n", self);

	oo = self->pchip;
	if(0 == oo)return 0;

	switch(oo->type){
		case _FILE_:
		case _file_:{
			return writefile(oo, oo->selffd, arg, idx, buf, len);
		}
		case _ptmx_:{
			return writeshell(oo->selffd, idx, buf, len);
			break;
		}
		case _uart_:{
			return uart_write(oo->selffd, idx, buf, len);
			break;
		}
		case _TCP_:{
			oo = oo->tempobj;
			if(0 == oo)return -1;
		}
		default:{
			return writesocket(oo->selffd, 0, buf, len);
		}
	}
	return 0;
}
int systemstop(struct halfrel* self, struct halfrel* peer)
{
	say("@systemstop\n");
	return 0;
}
int systemstart(struct halfrel* self, struct halfrel* peer)
{
	say("@systemstart\n");
	return 0;
}




int systemdelete(void* addr)
{
	int fd;
	struct object* oo;
	struct relation* rel;
	if(0 == addr)return 0;

	oo = addr;
	fd = oo->selffd;


	//del irel, orel
	rel = oo->irel0;
	if(0 != rel){
		relationstop((void*)&rel->srcchip, (void*)&rel->dstchip);
		relationdelete(rel);
	}
	oo->irel0 = oo->ireln = 0;

	rel = oo->orel0;
	if(0 != rel){
		relationstop((void*)&rel->srcchip, (void*)&rel->dstchip);
		relationdelete(rel);
	}
	oo->orel0 = oo->oreln = 0;


	//del it self
	switch(oo->type){
	case _file_:{
		stopfile(fd);
		break;
	}
	case _ptmx_:{
		stopshell(fd);
		break;
	}
	case _uart_:{
		uart_stop(fd);
		break;
	}
	default:{
		stopsocket(fd);
	}
	}
	oo->type = 0;
	oo->type = 0;
	oo->fmt = 0;
	oo->name = 0;
	return 0;
}
void* systemcreate(u64 type, void* argstr, int argc, u8** argv)
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
		obj[fd].selffd = fd;
		return &obj[fd];
	}
	if(_file_ == type)
	{
		fd = startfile(name, 'r');
		if(fd <= 0)return 0;

		obj[fd].type = _file_;
		obj[fd].selffd = fd;
		return &obj[fd];
	}
	else if(_ptmx_ == type)
	{
		parseuart(host, &port, name);
		say("parse: %s, %d\n", host, port);

		fd = startshell(host, port);
		if(fd <= 0)return 0;

		obj[fd].type = _ptmx_;
		obj[fd].selffd = fd;
		return &obj[fd];
	}
	else if(_uart_ == type)
	{
		parseuart(host, &port, name);
		say("parse: %s, %d\n", host, port);

		fd = uart_start(host, port);
		if(fd <= 0)return 0;

		obj[fd].type = _uart_;
		obj[fd].selffd = fd;
		return &obj[fd];
	}

	//decode ipaddr
	port = 80;
	if(name)url = name + parseurl(name, 0x100, host, &port);
	//say("systemcreate: %.8s://%s:%d/%s\n", &type, host, port, url);

	if(_RAW_ == type)		//raw server
	{
		fd = startsocket(host, port, _RAW_);
		if(0 >= fd)return 0;

		obj[fd].type = _RAW_;
		obj[fd].selffd = fd;
		return &obj[fd];
	}
	else if(_raw_ == type)	//raw client
	{
		fd = startsocket(host, port, _raw_);
		if(0 >= fd)return 0;

		obj[fd].type = _raw_;
		obj[fd].selffd = fd;
		return &obj[fd];
	}
	else if(_UDP_ == type)	//udp master
	{
		fd = startsocket(host, port, _UDP_);
		if(0 >= fd)return 0;

		obj[fd].type = _UDP_;
		obj[fd].selffd = fd;
		return &obj[fd];
	}
	else if(_Udp_ == type)	//udp server
	{
		obj[fd].type = _Udp_;
	}
	else if(_udp_ == type)	//udp client
	{
		fd = startsocket(host, port, _udp_);
		if(0 >= fd)return 0;

		obj[fd].type = _udp_;
		obj[fd].selffd = fd;
		return &obj[fd];
	}
	else if(_TCP_ == type)	//tcp master
	{
		fd = startsocket(host, port, _TCP_);
		if(0 >= fd)return 0;

		obj[fd].type = _TCP_;
		obj[fd].selffd = fd;
		return &obj[fd];
	}
	else if(_Tcp_ == type)	//tcp server
	{
		obj[fd].type = _Tcp_;
	}
	else if(_tcp_ == type)	//tcp client
	{
		fd = startsocket(host, port, _tcp_);
		if(0 >= fd)return 0;

		obj[fd].type = _tcp_;
		obj[fd].selffd = fd;
		return &obj[fd];
	}

	return 0;
}
void* systemmodify(int argc, u8** argv)
{
	int j;
	u64 name = 0;
	u8* tmp = (u8*)&name;
	if(argc < 2)return 0;
//say("%s,%s,%s,%s\n",argv[0],argv[1],argv[2],argv[3]);
	if(0 == ncmp(argv[1], "create", 6))
	{
		for(j=0;j<8;j++)
		{
			if(argv[2][j] <= 0x20)break;
			tmp[j] = argv[2][j];
		}
		say("%llx,%llx\n",name, argv[3]);
		systemcreate(name, argv[3], argc-3, &argv[3]);
	}
	return 0;
}
void* systemsearch(u8* buf, int len)
{
	int j,k=0;
	struct object* tmp;
	for(j=0;j<0x1000;j++)
	{
		tmp = &obj[j];
		if(0 == tmp->type)continue;

		k++;
		say("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
			&tmp->tier, &tmp->type, &tmp->fmt, &tmp->name);
	}

	if(0 == k)say("empth system\n");
	return 0;
}




void freesystem()
{
	//say("[8,9):freeing system\n");

	freesocket();
	freeuart();
	freeshell();
	freewatcher();
	freesignal();
	freerandom();
}
void initsystem(u8* addr)
{
	int j;
	obj = (void*)(addr+0x000000);
	ppp = (void*)(addr+0x100000);

#define max (0x100000/sizeof(struct object))
	for(j=0;j<0x200000;j++)addr[j]=0;
	for(j=0;j<max;j++)obj[j].tier = _sys_;

	initrandom(addr);
	initsignal(addr);
	initwatcher(addr);
	initshell(addr);
	inituart(addr);
	initsocket(addr);

	//say("[8,a):inited system\n");
}
