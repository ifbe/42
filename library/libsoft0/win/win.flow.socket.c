#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>	//IP_HDRINCL
#include <ws2bth.h>		//BTHPROTO_RFCOMM
#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include "libsoft.h"
#define _BT_ hex16('B','T')
#define _bt_ hex16('b','t')
void iocp_add(SOCKET, int);
void iocp_del(SOCKET, int);
void iocp_mod(SOCKET, int);
int parsemyandto(void*,int, void*,int, char**,int*, char**,int*);




struct per_io_data
{
	OVERLAPPED overlap;
	WSABUF bufing;
	int count;
	SOCKET sock;
};
//
static struct object* obj = 0;
static struct object* getobjbysock(SOCKET sock)
{
	return &obj[sock/4];
}
static struct object* getobjbid(int idx)
{
	return &obj[idx];
}
static int getidbysock(SOCKET sock)
{
	return sock/4;
}
static SOCKET getsockbyid(int idx)
{
	return idx*4;
}


/*
void selfname(u64 fd, u32* buf)
{
	struct sockaddr_in addr;
	u32 len = sizeof(struct sockaddr_in);
	getsockname(fd, (void*)&addr, &len);

	buf[0] = *(u32*)&addr.sin_addr;
	buf[1] = addr.sin_port;
}
void peername(u64 fd, u32* buf)
{
	struct sockaddr_in addr;
	u32 len = sizeof(struct sockaddr_in);
	getpeername(fd, (void*)&addr, &len);

	buf[0] = *(u32*)&addr.sin_addr;
	buf[1] = addr.sin_port;
}*/
u32 resolvehostname(char* addr)
{
	struct hostent* host;
	char** ptr;
	int j;

	//get
	host = gethostbyname(addr);
	if(0 == host){
		printf("err@gethostbyname: %d\n", errno);
		return 0;
	}

	//alias
	j = 0;
	ptr = host->h_aliases;
	while(1){
		if(0 == ptr[j])break;
		printf("alias:%s\n", ptr[j]);
		j++;
	}

	//address
	j = 0;
	ptr = host->h_addr_list;
	while(1){
		if(0 == ptr[j])break;
		printf("ipadd:%s\n", inet_ntoa(*(struct in_addr*)ptr[j]));
		j++;
	}
	return *(u32*)ptr[0];
}
int socket_fixaddr(char* addr)
{
	int j;
	u8 ip[4];
	for(j=0;j<128;j++){
		if(addr[j] <= 0x20)return 0;
		if(addr[j] == '.')continue;
		if((addr[j] >= '0')&&(addr[j] <= '9'))continue;

		//ifnot 0123456789.
		break;
	}

	*(u32*)ip = resolvehostname(addr);
	return snprintf(addr, 16, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}




int createsocket_bt(char* addr, int port)
{
	SOCKET fd = WSASocket(
		AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM,
		0, 0, WSA_FLAG_OVERLAPPED
	);
	if(INVALID_SOCKET == fd){
		printf("errno=%d@socket\n",GetLastError());
		return 0;
	}

	//
	iocp_add(fd, _bt_);
	iocp_mod(fd, _bt_);
	return getidbysock(fd);
}
int createsocket_raw(char* addr, int port)
{
	int ret;
	SOCKET fd;

	fd = WSASocket(
		PF_INET, SOCK_RAW, IPPROTO_IP,
		0, 0, WSA_FLAG_OVERLAPPED
	);
	if(INVALID_SOCKET == fd){
		printf("errno=%d@socket\n", GetLastError());
		return 0;
	}

	//
	int one = 1;
	if(SOCKET_ERROR == WSAIoctl(fd, SIO_RCVALL, &one, 4, 0, 0, (LPDWORD)&ret, 0, 0)){
		printf("errno=%d@WSAIoctl\n", GetLastError());
		return 0;
	}
	if(SOCKET_ERROR == setsockopt(fd, IPPROTO_IP, IP_HDRINCL, (char *)&ret, 4)){
		printf("errno=%d@setsockopt\n", GetLastError());
		return 0;
	}

	//self
	struct sockaddr_in* self = (void*)(getobjbysock(fd)->self);
	memset(self, 0, sizeof(struct sockaddr_in));
	self->sin_family = PF_INET;
	self->sin_port = htons(0);
	self->sin_addr.s_addr = inet_addr("127.0.0.1");

	//bind
	if(SOCKET_ERROR == bind(fd, (void*)self, sizeof(struct sockaddr_in))){
		printf("errno=%d@bind\n", GetLastError());
		return 0;
	}

	//
	iocp_add(fd, _raw_);
	iocp_mod(fd, _raw_);
	return getidbysock(fd);
}
int createsocket_udpserver(char* addr, int port)
{
	int ret;
	SOCKET fd;

	//
	fd = WSASocket(
		AF_INET, SOCK_DGRAM, 0,
		0, 0, WSA_FLAG_OVERLAPPED
	);
	if(INVALID_SOCKET == fd){
		printf("errno=%d@socket\n",GetLastError());
		return 0;
	}

	//self
	struct sockaddr_in* self = (void*)(getobjbysock(fd)->self);
	memset(self, 0, sizeof(struct sockaddr_in));
	self->sin_family = AF_INET;
	self->sin_port = htons(port);
	self->sin_addr.s_addr = INADDR_ANY;

	//bind
	ret = bind(fd, (SOCKADDR*)self, sizeof(SOCKADDR_IN));
	if(SOCKET_ERROR == ret){
		printf("errno=%d@bind\n",GetLastError());
		closesocket(fd);
		return 0;
	}

	//
	iocp_add(fd, _UDP_);
	iocp_mod(fd, _UDP_);
	return getidbysock(fd);
}
int createsocket_udpclient(char* myaddr, int myport, char* toaddr, int toport)
{
	int ret;
	SOCKET fd;

	//
	fd = WSASocket(
		AF_INET, SOCK_DGRAM, IPPROTO_UDP,
		0, 0, WSA_FLAG_OVERLAPPED
	);
	if(INVALID_SOCKET == fd){
		printf("errno=%d@socket\n",GetLastError());
		return 0;
	}

if((0 != myaddr) && (0 != myport)){
	//self
	struct sockaddr_in* self = (void*)(getobjbysock(fd)->self);
	memset(self, 0, sizeof(struct sockaddr_in));
	self->sin_family = AF_INET;
	self->sin_port = htons(myport);
	self->sin_addr.s_addr = inet_addr(myaddr);

	//bind
	ret = bind(fd, (SOCKADDR*)self, sizeof(SOCKADDR_IN));
	if(SOCKET_ERROR == ret){
		printf("errno=%d@bind\n",GetLastError());
		closesocket(fd);
		return 0;
	}
}

	//peer
	struct sockaddr_in* peer = (void*)(getobjbysock(fd)->peer);
	memset(peer, 0, sizeof(struct sockaddr_in));
	peer->sin_family = AF_INET;
	peer->sin_port = htons(toport);
	peer->sin_addr.s_addr = inet_addr(toaddr);

	//connect
	ret = connect(fd, (struct sockaddr*)peer, sizeof(struct sockaddr_in));
	if(ret < 0){
		printf("errno=%d@connect\n",GetLastError());
		return 0;
	}

	//
	iocp_add(fd, _udp_);
	iocp_mod(fd, _udp_);
	return getidbysock(fd);
}
int createsocket_tcpserver(char* addr, int port)
{
	int ret;
	SOCKET fd;

	//new
	fd = WSASocket(
		AF_INET, SOCK_STREAM, IPPROTO_TCP,
		0, 0, WSA_FLAG_OVERLAPPED
	);
	if(INVALID_SOCKET == fd){
		printf("errno=%d@WSASocket\n",GetLastError());
		return 0;
	}

	//self
	struct sockaddr_in* self = (void*)(getobjbysock(fd)->self);
	memset(self, 0, sizeof(struct sockaddr_in));
	self->sin_family = AF_INET;
	self->sin_port = htons(port);
	self->sin_addr.s_addr = INADDR_ANY;

	//bind
	ret = bind(fd, (SOCKADDR*)self, sizeof(SOCKADDR_IN));
	if(SOCKET_ERROR == ret){
		printf("errno=%d@bind\n",GetLastError());
		closesocket(fd);
		return 0;
	}

	//listen
	ret = listen(fd, SOMAXCONN);
	if(-1 == ret){
		printf("errno=%d@listen\n",GetLastError());
		closesocket(fd);
		return 0;
	}

	//ioctl
	LPFN_ACCEPTEX acceptex = NULL;
	GUID guidacceptex = WSAID_ACCEPTEX;
	DWORD dwret = 0;
	ret = WSAIoctl(
		fd,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidacceptex,
		sizeof(guidacceptex),
		&acceptex,
		sizeof(acceptex),
		&dwret,
		NULL,
		NULL
	);
	if(ret != 0){
		printf("errno=%d@WSAIoctl\n",GetLastError());
		return 0;
	}

	//acceptex
	int j;
	SOCKET t;
	struct object* tmp;
	struct per_io_data* perio;
	for(j=0;j<0x100;j++)
	{
		t = WSASocket(
			AF_INET, SOCK_STREAM, IPPROTO_TCP,
			0, 0, WSA_FLAG_OVERLAPPED
		);
		printf("%x\n", t);

		tmp = getobjbysock(t);
		perio = (void*)(tmp->data);
		perio->count = 0;
		perio->sock = t;

		ret = acceptex(
			fd, t,
			(void*)(tmp->self), 0,
			0x20, 0x20,
			0, (void*)perio
		);
	}

	iocp_add(fd, _TCP_);
	return getidbysock(fd);
}
int createsocket_tcpclient(char* myaddr, int myport, char* toaddr, int toport)
{
	int ret;
	SOCKET fd;

	//
	fd = WSASocket(
		AF_INET, SOCK_STREAM, IPPROTO_TCP,
		0, 0, WSA_FLAG_OVERLAPPED
	);
	if(INVALID_SOCKET == fd){
		printf("errno=@socket\n",GetLastError());
		return 0;
	}

	ret = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*)&ret, 4);
	if(ret < 0){
		printf("errno=%d@SO_REUSEADDR\n",GetLastError());
		return 0;
	}

if((0 != myaddr) && (0 != myport)){
	//self
	struct sockaddr_in* self = (void*)(getobjbysock(fd)->self);
	memset(self, 0, sizeof(struct sockaddr_in));
	self->sin_family = AF_INET;
	self->sin_port = htons(myport);
	self->sin_addr.s_addr = inet_addr(myaddr);

	//bind
	ret = bind(fd, (SOCKADDR*)self, sizeof(SOCKADDR_IN));
	if(SOCKET_ERROR == ret){
		printf("errno=%d@bind\n",GetLastError());
		closesocket(fd);
		return 0;
	}
}

	//peer
	struct sockaddr_in* peer = (void*)(getobjbysock(fd)->peer);
	memset(peer, 0, sizeof(struct sockaddr_in));
	peer->sin_family = AF_INET;
	peer->sin_port = htons(toport);
	peer->sin_addr.s_addr = inet_addr(toaddr);

	//connect
	if(SOCKET_ERROR == connect(fd, (void*)peer, sizeof(SOCKADDR_IN))){
		printf("errno=%d@connect\n",GetLastError());
		//stopsocket(fd/4);
		return 0;
	}

	//get the random port
	socklen_t len = sizeof(struct sockaddr_in);
	getsockname(fd, (void*)(getobjbysock(fd)->self), &len);

	//
	iocp_add(fd, _tcp_);
	iocp_mod(fd, _tcp_);
	return getidbysock(fd);
}




int readsocket(int idx, void* tmp, void* buf, int len)
{
	int j,ret;
	char* src;
	char* dst;

	SOCKET sock = getsockbyid(idx);
	struct object* perfd = &obj[idx];
	struct per_io_data* perio = (void*)(perfd->data);

	//perio
	ret = perio->count;
	if(0 == ret)return 0;	//disconnect

	//data
	src = perio->bufing.buf;
	dst = buf;
	for(j=0;j<ret;j++)dst[j] = src[j];

	//peer
	if(_UDP_ == perfd->type)
	{
		dst = tmp;
		src = perfd->peer;
		for(j=0;j<8;j++)dst[j] = src[j];
	}

	perio->count = 0;
	iocp_mod(sock, perfd->type);
	return ret;
}
int writesocket(int idx, struct sockaddr_in* tmp, void* buf, int len)
{
	int ret;
	DWORD dwret;
	WSABUF wbuf;

	SOCKET sock = getsockbyid(idx);
	struct object* perfd = &obj[idx];

	if(_UDP_ == perfd->type)
	{
		struct sockaddr_in out;
		if(0 == tmp)tmp = (void*)(perfd->peer);
		else{
			memset(&out, 0, sizeof(struct sockaddr_in));
			out.sin_family = AF_INET;
			out.sin_port = tmp->sin_port;
			out.sin_addr.s_addr = tmp->sin_addr.s_addr;
			tmp = &out;
		}

		wbuf.buf = buf;
		wbuf.len = len;
		ret = WSASendTo(sock,
			&wbuf, 1,
			&dwret, 0,
			(void*)tmp, sizeof(struct sockaddr_in),
			0, 0
		);
		return len;
	}

	wbuf.buf = buf;
	wbuf.len = len;
	ret = WSASend(sock, &wbuf, 1, &dwret, 0, 0, 0);
	//printf("@send:len=%d,ret=%d,err=%d\n",len,ret,GetLastError());
	return len;
}




int searchsocket(int id)
{
	SOCKET sock = getsockbyid(id);
	return 0;
}
int modifysocket(int id)
{
	SOCKET sock = getsockbyid(id);
	return 0;
}
int deletesocket(int id)
{
	SOCKET sock = getsockbyid(id);
	LPFN_DISCONNECTEX disconnectex = NULL;
	GUID guiddisconnectex = WSAID_DISCONNECTEX;
	DWORD dwret = 0;
	int ret = WSAIoctl(
		sock,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guiddisconnectex,
		sizeof(guiddisconnectex),
		&disconnectex,
		sizeof(disconnectex),
		&dwret,
		NULL,
		NULL
	);
	if(ret != 0)
	{
		printf("error@stopsocket.WSAIoctl\n");
		return 0;
	}

	disconnectex(sock, 0, TF_REUSE_SOCKET, 0);
	return 0;
}
int createsocket(int fmt, char* arg)
{
	int j;
	char tmp[256];

	int myport = 0;
	char* myaddr = 0;
	int toport = 0;
	char* toaddr = 0;

	//my->to
	for(j=0;j<256;j++){
		if(arg[j] <= 0x20)break;
		parsemyandto(arg, 256, tmp, 256, &myaddr, &myport, &toaddr, &toport);
	}
	if(myaddr)socket_fixaddr(myaddr);
	if(toaddr)socket_fixaddr(toaddr);
	//printmemory(tmp,256);

	//type
	switch(fmt){
	case _RAW_:return createsocket_raw(myaddr, myport);
	case _UDP_:return createsocket_udpserver(myaddr, myport);
	case _udp_:{
		if((0 == toaddr)&&(0 == toport)){
			toaddr = myaddr;
			toport = myport;
			myaddr = 0;
			myport = 0;
		}
		return createsocket_udpclient(myaddr, myport, toaddr, toport);
	}
	case _TCP_:return createsocket_tcpserver(myaddr, myport);
	case _tcp_:{
		if((0 == toaddr)&&(0 == toport)){
			toaddr = myaddr;
			toport = myport;
			myaddr = 0;
			myport = 0;
		}
		return createsocket_tcpclient(myaddr, myport, toaddr, toport);
	}
	default:printf("error@type\n");
	}
	return 0;
}




void freesocket()
{
	WSACleanup();
}
void initsocket(void* addr)
{
	obj = addr;

	//socket
	WSADATA data; 
	WORD sockVersion = MAKEWORD(2,2);
	if(WSAStartup(sockVersion, &data) != 0)
	{
		printf("error@WSAStartup\n");
		return;
	}
}
