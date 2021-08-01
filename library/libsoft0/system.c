#include "libsoft.h"
#define _ptmx_ hex32('p','t','m','x')
//
int initprocess();
int freeprocess();
//
int initthread();
int freethread();
//
int initfilemgr();
int freefilemgr();
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
void* shell_create(void*, int);
int shell_delete(void*);
int shell_take(void*,void*, void*,int, void*,int);
int shell_give(void*,void*, void*,int, void*,int);
//uart
int inituart(void*);
int freeuart();
void* uart_create(void*, int);
int uart_delete(void*);
int uart_take(void*,void*, void*,int, void*,int);
int uart_give(void*,void*, void*,int, void*,int);
//socket
int initsocket(void*);
int freesocket();
void* socket_create(int fmt, void* url);
int socket_delete(void*);
int socket_search(int);
int socket_modify(int);
int socket_take(void*,void*, void*,int, void*,int);
int socket_give(void*,void*, void*,int, void*,int);
//
int parseuart(void*, int*, void*);
int ncmp(void*, void*, int);
int cmp(void*, void*);




//
static struct item* obj = 0;
static int objlen = 0;
static void* ppp = 0;
static int ppplen = 0;




void system_init(u8* addr)
{
	say("[8,a):system initing\n");

	int j;
	obj = (void*)(addr+0x000000);
	ppp = (void*)(addr+0x100000);

#define max (0x100000/sizeof(struct item))
	for(j=0;j<0x200000;j++)addr[j]=0;
	for(j=0;j<max;j++)obj[j].tier = _sys_;

	initrandom(addr);
	initsignal(addr);
	initwatcher(addr);
	initshell(addr);
	inituart(addr);
	initsocket(addr);
	initfilemgr(addr);

	initprocess();
	initthread();

	say("[8,a):system inited\n");
}
void system_exit()
{
	say("[8,a):system exiting\n");

	freesocket();
	freeuart();
	freeshell();
	freewatcher();
	freesignal();
	freerandom();

	say("[8,a):system exited\n");
}
void* system_alloc()
{
	void* addr = 0;
	if(1){		//windows uses this function
		addr = &obj[objlen];
		objlen += 1;
	}
	return addr;
}
void system_recycle()
{
}




void* systemcreate(u64 type, void* argstr, int argc, u8** argv)
{
	int j,k,ret;
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
	struct item* per = 0;
	switch(type){
	case _FILE_:
		per = file_create(name, 'w');
		if(0 == per)return 0;

		per->type = _FILE_;
		return per;

	case _file_:
		per = file_create(name, 'r');
		if(0 == per)return 0;

		per->type = _file_;
		return per;

	case _ptmx_:
		parseuart(host, &port, name);
		say("parse: %s, %d\n", host, port);

		per = shell_create(host, port);
		if(0 == per)return 0;

		per->type = _ptmx_;
		return per;

	case _uart_:
		parseuart(host, &port, name);
		say("parse: %s, %d\n", host, port);

		per = uart_create(host, port);
		if(0 == per)return 0;

		per->type = _uart_;
		return per;

	case _RAW_:		//raw server
		per = socket_create(_RAW_, name);
		if(0 == per)return 0;

		per->type = _RAW_;
		return per;

	case _raw_:		//raw client
		per = socket_create(_raw_, name);
		if(0 == per)return 0;

		per->type = _raw_;
		return per;

	case _UDP_:		//udp master
		per = socket_create(_UDP_, name);
		if(0 == per)return 0;

		per->type = _UDP_;
		return per;

	case _Udp_:		//udp server
		per = socket_create(_Udp_, name);
		if(0 == per)return 0;

		per->type = _Udp_;
		return per;

	case _udp_:		//udp client
		per = socket_create(_udp_, name);
		if(0 == per)return 0;

		per->type = _udp_;
		return per;

	case _TCP_:		//tcp master
		per = socket_create(_TCP_, name);
		if(0 == per)return 0;

		per->type = _TCP_;
		return per;

	case _tcp_:		//tcp client
		per = socket_create(_tcp_, name);
		if(0 == per)return 0;

		per->type = _tcp_;
		return per;
	}

	return 0;
}
int systemdelete(struct item* oo)
{
	struct relation* rel;
	if(0 == oo)return 0;


	//del irel, orel
	rel = oo->irel0;
	if(0 != rel){
		relationdiscon((void*)&rel->srcchip, (void*)&rel->dstchip);
		relationdelete(rel);
	}
	oo->irel0 = oo->ireln = 0;

	rel = oo->orel0;
	if(0 != rel){
		relationdiscon((void*)&rel->srcchip, (void*)&rel->dstchip);
		relationdelete(rel);
	}
	oo->orel0 = oo->oreln = 0;


	//del it self
	switch(oo->type){
	case _file_:{
		file_delete(oo);
		break;
	}
	case _ptmx_:{
		shell_delete(oo);
		break;
	}
	case _uart_:{
		uart_delete(oo);
		break;
	}
	default:{
		socket_delete(oo);
	}
	}
	oo->type = 0;
	oo->type = 0;
	oo->hfmt = 0;
	oo->vfmt = 0;
	return 0;
}
void* systemsearch(u8* buf, int len)
{
	int j,k=0;
	struct item* tmp;
	for(j=0;j<0x1000;j++)
	{
		tmp = &obj[j];
		if(0 == tmp->type)continue;

		k++;
		say("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
			&tmp->tier, &tmp->type, &tmp->hfmt, &tmp->vfmt);
	}

	if(0 == k)say("empth system\n");
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




int systemlinkup(struct halfrel* self, struct halfrel* peer)
{
	say("@system_linkup\n");
	return 0;
}
int systemdiscon(struct halfrel* self, struct halfrel* peer)
{
	say("@system_discon\n");
	return 0;
}
int system_take(_obj* sys,void* foot, _syn* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	switch(sys->type){
	case _FILE_:
	case _file_:
		return file_take(sys, 0, arg, cmd, buf, len);
	}
	return 0;
}
int system_give(_obj* sys,void* foot, _syn* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	switch(sys->type){
	case _FILE_:
	case _file_:
		return file_give(sys,0, arg,cmd, buf,len);
	case _ptmx_:
		return shell_give(sys,0, arg,cmd, buf,len);
		break;
	case _uart_:
		return uart_give(sys,0, arg,cmd, buf,len);
		break;
	case _TCP_:
		sys = sys->sockinfo.child;
		if(0 == sys)return -1;
	default:
		return socket_give(sys,0, arg,cmd, buf,len);
	}
	return 0;
}