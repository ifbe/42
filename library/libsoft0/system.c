#include "libsoft.h"
#define _ptmx_ hex32('p','t','m','x')
//
int initprocess();
int freeprocess();
//
int initthread();
int freethread();
//
int initosfile(void*);
int freeosfile();
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
int shell_reader(void*,void*, p64,int, void*,int);
int shell_writer(void*,void*, p64,int, void*,int);
//uart
int inituart(void*);
int freeuart();
void* uart_create(void*, int);
int uart_delete(void*);
int uart_reader(void*,void*, p64,int, void*,int);
int uart_writer(void*,void*, p64,int, void*,int);
//tun
int inittun(void*);
int freetun();
void* tun_create(void*, int);
int tun_delete(void*);
int tun_reader(void*,void*, p64,int, void*,int);
int tun_writer(void*,void*, p64,int, void*,int);
//socket
int initsocket(void*);
int freesocket();
void* socket_create(int fmt, u8* path);
int socket_delete(void*);
int socket_reader(void*,void*, p64,int, void*,int);
int socket_writer(void*,void*, p64,int, void*,int);
//
int parseuart(void*, int*, void*);
int ncmp(void*, void*, int);
int cmp(void*, void*);




//
static struct item* sysobj = 0;
static int objlen = 0;
static void* ppp = 0;
static int ppplen = 0;




#define maxitem (0x100000/sizeof(struct item))
//#define maxfoot 
void system_init(u8* addr, int size)
{
	logtoall("[8,a):system initing\n");

	int j;
	for(j=0;j<0x200000;j++)addr[j]=0;

	sysobj = (void*)(addr+0x000000);
	objlen = maxitem-1;
	for(j=0;j<maxitem;j++)sysobj[j].tier = _sys_;

	ppp = (void*)(addr+0x100000);

	initrandom(addr);
	initsignal(addr);
	initwatcher(addr);
	initshell(addr);
	inituart(addr);
	initsocket(addr);
	initosfile(addr);

	initprocess();
	initthread();

	logtoall("[8,a):system inited\n");
}
void system_exit()
{
	logtoall("[8,a):system exiting\n");

	//freeosfile();
	freesocket();
	freeuart();
	freeshell();
	freewatcher();
	freesignal();
	freerandom();

	logtoall("[8,a):system exited\n");
}


int system_obj2fd(_obj* obj)
{
	u8* a0 = (u8*)sysobj;
	u8* a1 = (u8*)obj;
	return (a1-a0)/sizeof(struct item);
}
void* system_fd2obj(int fd)
{
	return &sysobj[fd];
}




void system_recycle()
{
}
void* system_alloc()
{
	void* addr = &sysobj[objlen];
	objlen -= 1;
	return addr;
}
void* system_alloc_fromtype(u64 type)
{
	_obj* obj = system_alloc();
	if(0 == obj)return 0;

	//obj->tier = tier;		//should be tier: bootup
	//obj->kind = kind;		//should be class: usb
	obj->type = type;		//should be type: xhci
	//obj->vfmt = vfmt;		//should be model: intelxhci
	return obj;
}
void* system_alloc_frompath(u64 type, u8* path)
{
	int j,k,ret;
	u8 host[0x100];	//127.0.0.1
	int port;	//2222

	u8* t;		//http
	u8* name = path;
	if(0 == type){
		for(j=0;j<16;j++){
			if(name[j] <= 0x20)break;
			if( (':'==name[j]) && ('/'==name[j+1]) && ('/'==name[j+2]) ){
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
	//logtoall("type=%llx,name=%.16s\n", type, name);
	if(0 == type){
		logtoall("@system_create:unknown path:%s\n", name);
		return 0;
	}

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
		logtoall("parse: %s, %d\n", host, port);

		per = shell_create(host, port);
		if(0 == per)return 0;

		per->type = _ptmx_;
		return per;

	case _uart_:
		parseuart(host, &port, name);
		logtoall("parse: %s, %d\n", host, port);

		per = uart_create(host, port);
		if(0 == per)return 0;

		per->type = _uart_;
		return per;

	case _tap_:
		break;

	case _tun_:
		break;

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

	return per;
}
void* system_alloc_fromfd(u64 type, int fd)
{
	_obj* obj = system_fd2obj(fd);

	obj->type = type;
	return obj;
}




int system_create(_obj* obj, void* argstr, int argc, u8** argv)
{
	return 0;
}
int system_delete(_obj* oo)
{
	struct relation* rel;
	if(0 == oo)return 0;


	//del irel, orel
	rel = oo->irel0;
	if(0 != rel){
		relationdetach((void*)&rel->srcchip, (void*)&rel->dstchip);
		relationdelete(rel);
	}
	oo->irel0 = oo->ireln = 0;

	rel = oo->orel0;
	if(0 != rel){
		relationdetach((void*)&rel->srcchip, (void*)&rel->dstchip);
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
	oo->vfmt = 0;
	oo->type = 0;
	oo->kind = 0;
	oo->tier = 0;
	return 0;
}
int system_reader(_obj* sys,void* foot, p64 arg,int cmd, void* buf,int len)
{
	switch(sys->type){
	case _FILE_:
	case _file_:
		return file_reader(sys, 0, arg, cmd, buf, len);
	}
	return 0;
}
int system_writer(_obj* sys,void* foot, p64 arg,int cmd, void* buf,int len)
{
	switch(sys->type){
	case _FILE_:
	case _file_:
		return file_writer(sys, 0, arg, cmd, buf, len);
	}
	return 0;
}




int system_attach(_obj* ent,void* foot, struct halfrel* self, struct halfrel* peer)
{
	_obj* oo = self->pchip;
	switch(oo->type){
	case _file_:
		logtoall("@file_attach: (%.4s@%p, %.4s@%p) -> (%.4s@%p, %.4s@%p)\n",
			&self->nodetype, self->pchip, &self->foottype, self->pfoot,
			&peer->nodetype, peer->pchip, &peer->foottype, peer->pfoot);
		//file_attach(oo);
		break;
	case _ptmx_:
		logtoall("@ptmx_attach: (%.4s@%p, %.4s@%p) -> (%.4s@%p, %.4s@%p)\n",
			&self->nodetype, self->pchip, &self->foottype, self->pfoot,
			&peer->nodetype, peer->pchip, &peer->foottype, peer->pfoot);
		//shell_attach(oo);
		break;
	case _uart_:
		logtoall("@uart_attach: (%.4s@%p, %.4s@%p) -> (%.4s@%p, %.4s@%p)\n",
			&self->nodetype, self->pchip, &self->foottype, self->pfoot,
			&peer->nodetype, peer->pchip, &peer->foottype, peer->pfoot);
		//uart_attach(oo);
		break;
	default:
		logtoall("@socket_attach: (%.4s@%p, %.4s@%p) -> (%.4s@%p, %.4s@%p)\n",
			&self->nodetype, self->pchip, &self->foottype, self->pfoot,
			&peer->nodetype, peer->pchip, &peer->foottype, peer->pfoot);
		//socket_attach(oo);
	}
	return 0;
}
int system_detach(_obj* ent,void* foot, struct halfrel* self, struct halfrel* peer)
{
	_obj* oo = self->pchip;
	switch(oo->type){
	case _file_:
		logtoall("@file_detach: (%.4s@%p, %.4s@%p) -> (%.4s@%p, %.4s@%p)\n",
			&self->nodetype, self->pchip, &self->foottype, self->pfoot,
			&peer->nodetype, peer->pchip, &peer->foottype, peer->pfoot);
		//file_detach(oo);
		break;
	case _ptmx_:
		logtoall("@ptmx_detach: (%.4s@%p, %.4s@%p) -> (%.4s@%p, %.4s@%p)\n",
			&self->nodetype, self->pchip, &self->foottype, self->pfoot,
			&peer->nodetype, peer->pchip, &peer->foottype, peer->pfoot);
		//shell_detach(oo);
		break;
	case _uart_:
		logtoall("@uart_detach: (%.4s@%p, %.4s@%p) -> (%.4s@%p, %.4s@%p)\n",
			&self->nodetype, self->pchip, &self->foottype, self->pfoot,
			&peer->nodetype, peer->pchip, &peer->foottype, peer->pfoot);
		//uart_detach(oo);
		break;
	default:
		logtoall("@socket_detach: (%.4s@%p, %.4s@%p) -> (%.4s@%p, %.4s@%p)\n",
			&self->nodetype, self->pchip, &self->foottype, self->pfoot,
			&peer->nodetype, peer->pchip, &peer->foottype, peer->pfoot);
		//socket_detach(oo);
	}
	return 0;
}
int system_takeby(_obj* sys,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	switch(sys->type){
	case _FILE_:
	case _file_:
		return file_reader(sys, 0, arg, cmd, buf, len);
	}
	return 0;
}
int system_giveby(_obj* sys,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	//logtoall("@systemwrite: obj=%.8s,len=%x\n", &sys->type,len);
	switch(sys->type){
	case _FILE_:
	case _file_:
		return file_writer(sys,0, arg,cmd, buf,len);
	case _ptmx_:
		return shell_writer(sys,0, arg,cmd, buf,len);
	case _uart_:
		return uart_writer(sys,0, arg,cmd, buf,len);
	case _TCP_:
		sys = sys->sockinfo.child;
		if(0 == sys)return -1;
	default:
		return socket_writer(sys,0, arg,cmd, buf,len);
	}
	return 0;
}




int systemcommand_insert(u8* name, u8* arg)
{
	return 0;
}
int systemcommand_remove(u8* name)
{
	return 0;
}
int systemcommand_search(u8* name)
{
	int j;
	_obj* act;
	if(0 == name){
		for(j=0;j<maxitem;j++){
			act = &sysobj[j];
			if(0 == act->type)continue;
			logtoall("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&act->tier, &act->kind, &act->type, &act->vfmt);
		}
		if(0 == j)logtoall("empty system\n");
	}
	else{
		for(j=0;j<0x100;j++){
			if(0 == sysobj[j].type)break;
			if(0 == cmp(&sysobj[j].type, name))logtoall("name=%d,node=%p\n", name, &sysobj[j]);
			break;
		}
	}
	return 0;
}
int systemcommand_modify(int argc, u8** argv)
{
	return 0;
}
void* systemcommand(int argc, u8** argv)
{
	if(argc < 2){
		logtoall("system insert name arg\n");
		logtoall("system search name\n");
	}
	else if(0 == ncmp(argv[1], "insert", 6)){
		//system create name arg
		systemcommand_insert(argv[2], argv[3]);
	}
	else if(0 == ncmp(argv[1], "search", 6)){
		//system search <name>
		systemcommand_search((argc<3) ? 0 : argv[2]);
	}

	return 0;
}
