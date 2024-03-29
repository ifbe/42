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
void iocp_add(SOCKET, int);
void iocp_del(SOCKET, int);
void iocp_mod(SOCKET, int);
int parseipv6addr(u8* buf, u8* out);
int parsemyandto(void*,int, void*,int, char**,int*, char**,int*);




union addrv4v6{
	struct sockaddr sa;		//just make compiler happy
	struct sockaddr_in v4;
	struct sockaddr_in6 v6;
};
struct perio
{
	OVERLAPPED overlap;		//start with struct overlap
	WSABUF bufing;		//follow by own data
	SOCKET childsock;
};
struct perfd{
	struct perio perio[2];
	SOCKET sock;
};




static _obj* g_obj = 0;
static _obj* getobjbysock(SOCKET sock)
{
	return &g_obj[sock/4];
}
static SOCKET getsockbyobj(_obj* oo)
{
	struct perfd* perfd = (void*)(oo->priv_256b);
	return perfd->sock;
}




void initsocket(void* addr)
{
	g_obj = addr;

	//socket
	WSADATA data; 
	WORD sockVersion = MAKEWORD(2,2);
	if(WSAStartup(sockVersion, &data) != 0)
	{
		printf("error@WSAStartup\n");
		return;
	}
}
void freesocket()
{
	WSACleanup();
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
u32 resolvehostname4(char* addr)
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
		printf("addr[%d] = %s\n", j, inet_ntoa(*(struct in_addr*)ptr[j]));
		j++;
	}
	return *(u32*)ptr[0];
}
u32 resolvehostname6(char* addr, union addrv4v6* out)
{
	struct addrinfo hint;
	memset(&hint, 0, sizeof(struct addrinfo));
	hint.ai_family = AF_INET6;
	hint.ai_socktype = SOCK_STREAM;
	//hint.ai_flags = AI_DEFAULT;

	struct addrinfo* res;
	int err = getaddrinfo(addr, NULL, &hint, &res);
	if(err){
		perror ("[ERROR] getaddrinfo ");
		return 0;
	}

	u32 ret = 0;
	struct addrinfo* tmp = res;
	while (tmp){
		printf("ai_flags -> %x\n", res->ai_flags) ;
		printf("ai_family -> %x\n", res->ai_family) ;
		printf("ai_socktype -> %x\n", res->ai_socktype) ;
		printf("ai_protocol -> %x\n", res->ai_protocol) ;
		printf("ai_addrlen -> %x\n", res->ai_addrlen) ;
		if(tmp->ai_family == AF_INET6) {
			union addrv4v6* xx = (void*)tmp->ai_addr;
			printmemory(xx, 32);
			out->v6.sin6_family = AF_INET6;
			memcpy(&out->v6.sin6_addr, &xx->v6.sin6_addr, tmp->ai_addrlen);
			ret = 1;
			break;
		}

		tmp = tmp->ai_next;
	}

	freeaddrinfo(res);
	return ret;
}
int socket_str2sockaddr(char* addr, union addrv4v6* out)
{
	int j;
	int isv4=0,isv6=0;
	for(j=0;j<128;j++){
		if(addr[j] <= 0x20)break;
		else if(addr[j] == '.'){
			isv4++;
		}
		else if(addr[j] == '`'){
			isv6++;
		}
		else if((addr[j] >= '0')&&(addr[j] <= '9')){
			isv4++;
			isv6++;
		}
		else if((addr[j] >= 'a')&&(addr[j] <= 'f')){
			isv6++;
		}
		else if((addr[j] >= 'A')&&(addr[j] <= 'F')){
			isv6++;
		}
	}
	if(j==isv4){
		out->v4.sin_family = AF_INET;
		out->v4.sin_addr.s_addr = inet_addr(addr);
		return 0;
	}
	if(j==isv6){
		out->v4.sin_family = AF_INET6;
		parseipv6addr((u8*)addr, (void*)&out->v6.sin6_addr);
		return 0;
	}

	logtoall("resolvehostname4\n");
	u32 tmp = resolvehostname4(addr);
	if(tmp){
		out->v4.sin_family = AF_INET;
		out->v4.sin_addr.s_addr = tmp;
		return 0;
	}

	logtoall("resolvehostname6\n");
	tmp = resolvehostname6(addr, out);
	if(tmp){
		logtoall("66666666\n");
		return 0;
	}

	return 0;
}




_obj* createsocket_bt(char* addr, int port)
{
	SOCKET fd = WSASocket(
		AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM,
		0, 0, WSA_FLAG_OVERLAPPED
	);
	if(INVALID_SOCKET == fd){
		printf("errno=%d@socket\n",GetLastError());
		return 0;
	}

	//mem
	_obj* oo = getobjbysock(fd);
	logtoall("fd=%x,oo=%p\n", fd, oo);

	struct perfd* perfd = (void*)(oo->priv_256b);
	perfd->sock = fd;

	//
	iocp_add(fd, _bt_);
	iocp_mod(fd, _bt_);
	return oo;
}
_obj* createsocket_raw(char* addr, int port)
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

	//mem
	_obj* oo = getobjbysock(fd);
	logtoall("fd=%x,oo=%p\n", fd, oo);

	struct perfd* perfd = (void*)(oo->priv_256b);
	perfd->sock = fd;

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
	struct sockaddr_in* self = (void*)(oo->sockinfo.self);
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
	return oo;
}
_obj* createsocket_udpserver(union addrv4v6* my)
{
	int sockfmt,socklen;
	if(AF_INET6 == my->sa.sa_family){
		sockfmt = AF_INET6;
		socklen = sizeof(struct sockaddr_in6);
	}
	else{
		sockfmt = AF_INET;
		socklen = sizeof(struct sockaddr_in);
	}

	//
	SOCKET fd = WSASocket(
		AF_INET, SOCK_DGRAM, 0,
		0, 0, WSA_FLAG_OVERLAPPED
	);
	if(INVALID_SOCKET == fd){
		printf("errno=%d@socket\n",GetLastError());
		return 0;
	}

	//mem
	_obj* oo = getobjbysock(fd);
	logtoall("fd=%x,oo=%p\n", fd, oo);

	struct perfd* perfd = (void*)(oo->priv_256b);
	perfd->sock = fd;

	//bind
	int ret = bind(fd, (void*)my, socklen);
	if(SOCKET_ERROR == ret){
		printf("errno=%d@bind\n",GetLastError());
		closesocket(fd);
		return 0;
	}

	//self
	union addrv4v6* self = (void*)oo->sockinfo.self;
	memcpy((void*)self, my, socklen);

	iocp_add(fd, _UDP_);
	iocp_mod(fd, _UDP_);
	return oo;
}
_obj* createsocket_udpclient(union addrv4v6* my, union addrv4v6* to)
{
	int sockfmt,socklen;
	if(	(my && (AF_INET6 == my->sa.sa_family) ) |
		(to && (AF_INET6 == to->sa.sa_family) ) )
	{
		sockfmt = AF_INET6;
		socklen = sizeof(struct sockaddr_in6);
	}
	else{
		sockfmt = AF_INET;
		socklen = sizeof(struct sockaddr_in);
	}

	//
	SOCKET fd = WSASocket(
		AF_INET, SOCK_DGRAM, IPPROTO_UDP,
		0, 0, WSA_FLAG_OVERLAPPED
	);
	if(INVALID_SOCKET == fd){
		printf("errno=%d@socket\n",GetLastError());
		return 0;
	}

	//mem
	_obj* oo = getobjbysock(fd);
	logtoall("fd=%x,oo=%p\n", fd, oo);

	struct perfd* perfd = (void*)(oo->priv_256b);
	perfd->sock = fd;

	int ret;
if(my){
	ret = bind(fd, (void*)my, socklen);
	if(SOCKET_ERROR == ret){
		printf("errno=%d@bind\n",GetLastError());
		closesocket(fd);
		return 0;
	}
}

	//connect
	ret = connect(fd, (struct sockaddr*)to, socklen);
	if(ret < 0){
		printf("errno=%d@connect\n",GetLastError());
		return 0;
	}

	ret = setsockopt(fd, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, NULL, 0);

	//self
	union addrv4v6* self = (void*)oo->sockinfo.self;
	socklen_t len = socklen;
	getsockname(fd, (void*)(oo->sockinfo.self), &len);
	printmemory(self, 32);

	//peer
	union addrv4v6* peer = (void*)oo->sockinfo.peer;
	memcpy((void*)peer, to, socklen);

	iocp_add(fd, _udp_);
	iocp_mod(fd, _udp_);
	return oo;
}
_obj* createsocket_tcpserver(union addrv4v6* my)
{
	int sockfmt,socklen;
	if(AF_INET6 == my->sa.sa_family){
		sockfmt = AF_INET6;
		socklen = sizeof(struct sockaddr_in6);
	}
	else{
		sockfmt = AF_INET;
		socklen = sizeof(struct sockaddr_in);
	}

	//new
	SOCKET fd = WSASocket(
		AF_INET, SOCK_STREAM, IPPROTO_TCP,
		0, 0, WSA_FLAG_OVERLAPPED
	);
	if(INVALID_SOCKET == fd){
		printf("errno=%d@WSASocket\n",GetLastError());
		return 0;
	}

	//mem
	_obj* oo = getobjbysock(fd);
	struct perfd* parentperfd = (void*)(oo->priv_256b);
	logtoall("fd=%x,oo=%p\n", fd, oo);

	oo->sockinfo.fd = fd;
	parentperfd->sock = fd;

	//bind
	int ret = bind(fd, (SOCKADDR*)my, socklen);
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
	DWORD dwret = 0;

	LPFN_ACCEPTEX acceptex = NULL;
	GUID guid_AcceptEx = WSAID_ACCEPTEX;
	ret = WSAIoctl(
		fd,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guid_AcceptEx,
		sizeof(guid_AcceptEx),
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
/*
	LPFN_GETACCEPTEXSOCKADDRS getacceptexsockaddrs = NULL;
	GUID guid_GetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
	ret = WSAIoctl(
		listenperfd.sock,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guid_GetAcceptExSockaddrs,
		sizeof(guid_GetAcceptExSockaddrs),
		&getacceptexsockaddrs,
		sizeof(getacceptexsockaddrs),
		&dwret,
		NULL,
		NULL
	);
	if(ret != 0){
		printf("errno=%d@WSAIoctl WSAID_ACCEPTEX\n",GetLastError());
		return 0;
	}
*/
	//acceptex
	int j;
	SOCKET childsock;
	_obj* childobj;
	struct perfd* childperfd;
	struct perio* childperio;
	for(j=0;j<0x100;j++)
	{
		childsock = WSASocket(
			AF_INET, SOCK_STREAM, IPPROTO_TCP,
			0, 0, WSA_FLAG_OVERLAPPED
		);
		printf("%x\n", childsock);

		childobj = getobjbysock(childsock);

		childperfd = (void*)(childobj->priv_256b);
		childperfd->sock = childsock;

		childperio = &childperfd->perio[0];
		childperio->childsock = childsock;

		ret = acceptex(
			fd, childsock,
			(void*)(childobj->sockinfo.self), 0,
			0x20, 0x20,
			0, (void*)childperio
		);
	}

	//self
	union addrv4v6* self = (void*)oo->sockinfo.self;
	memcpy((void*)self, my, socklen);

	iocp_add(fd, _TCP_);
	return oo;
}
_obj* createsocket_tcpclient(union addrv4v6* my, union addrv4v6* to)
{
	int sockfmt,socklen;
	if(	(my && (AF_INET6 == my->sa.sa_family) ) |
		(to && (AF_INET6 == to->sa.sa_family) ) )
	{
		sockfmt = AF_INET6;
		socklen = sizeof(struct sockaddr_in6);
	}
	else{
		sockfmt = AF_INET;
		socklen = sizeof(struct sockaddr_in);
	}

	//
	SOCKET fd = WSASocket(
		sockfmt, SOCK_STREAM, IPPROTO_TCP,
		0, 0, WSA_FLAG_OVERLAPPED
	);
	if(INVALID_SOCKET == fd){
		printf("errno=@socket\n",GetLastError());
		return 0;
	}

	//mem
	_obj* oo = getobjbysock(fd);
	logtoall("fd=%x,oo=%p\n", fd, oo);

	struct perfd* perfd = (void*)(oo->priv_256b);
	perfd->sock = fd;

	//reuse
	int ret = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*)&ret, 4);
	if(ret < 0){
		printf("errno=%d@SO_REUSEADDR\n",GetLastError());
		return 0;
	}

if(my){
	ret = bind(fd, (void*)my, socklen);
	if(SOCKET_ERROR == ret){
		printf("errno=%d@bind\n",GetLastError());
		closesocket(fd);
		return 0;
	}
}

	//connect
	if(SOCKET_ERROR == connect(fd, (void*)to, socklen)){
		printf("errno=%d@connect\n",GetLastError());
		closesocket(fd);
		return 0;
	}

	ret = setsockopt(fd, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, NULL, 0);

	//self
	union addrv4v6* self = (void*)oo->sockinfo.self;
	socklen_t len = socklen;
	getsockname(fd, (void*)(oo->sockinfo.self), &len);
	printmemory(self, 32);

	//peer
	union addrv4v6* peer = (void*)oo->sockinfo.peer;
	memcpy((void*)peer, to, socklen);

	//
	iocp_add(fd, _tcp_);
	iocp_mod(fd, _tcp_);
	return oo;
}




_obj* socket_create(int fmt, char* arg)
{
	int j;
	char tmp[256];

	int myport = 0;
	char* myaddr = 0;
	int toport = 0;
	char* toaddr = 0;

	union addrv4v6 my = {};
	union addrv4v6 to = {};
	if(0 == arg)goto skip;

	//my->to
	for(j=0;j<256;j++){
		if(arg[j] <= 0x20)break;
		parsemyandto(arg, 256, tmp, 256, &myaddr, &myport, &toaddr, &toport);
	}

skip:
	//type
	switch(fmt){
	case _RAW_:{
		return createsocket_raw(myaddr, myport);
	}
	case _UDP_:{
		my.v4.sin_port = htons(myport);
		socket_str2sockaddr(myaddr, &my);
		return createsocket_udpserver(&my);
	}
	case _udp_:{
		if((0 == toaddr)&&(0 == toport)){
			toaddr = myaddr;
			toport = myport;
			socket_str2sockaddr(toaddr, &to);
			to.v4.sin_port = htons(toport);
			return createsocket_udpclient(0, &to);
		}
		else{
			socket_str2sockaddr(myaddr, &my);
			my.v4.sin_port = htons(myport);
			socket_str2sockaddr(toaddr, &to);
			to.v4.sin_port = htons(toport);
			return createsocket_udpclient(&my, &to);
		}
	}
	case _TCP_:{
		socket_str2sockaddr(myaddr, &my);
		my.v4.sin_port = htons(myport);
		return createsocket_tcpserver(&my);
	}
	case _tcp_:{
		if((0 == toaddr)&&(0 == toport)){
			toaddr = myaddr;
			toport = myport;
			socket_str2sockaddr(toaddr, &to);
			to.v4.sin_port = htons(toport);
			return createsocket_tcpclient(0, &to);
		}
		else{
			socket_str2sockaddr(myaddr, &my);
			my.v4.sin_port = htons(myport);
			socket_str2sockaddr(toaddr, &to);
			to.v4.sin_port = htons(toport);
			return createsocket_tcpclient(&my, &to);
		}
	}
	default:
		printf("error@type\n");
	}
	return 0;
}
int socket_delete(_obj* oo)
{
	SOCKET sock = getsockbyobj(oo);
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
int socket_reader(_obj* oo,int xx, p64 arg,int cmd, void* buf, int len)
{
	int j,ret;
	char* src;
	char* dst;

	SOCKET sock = getsockbyobj(oo);
	struct perfd* perfd = (void*)(oo->priv_256b);
	struct perio* perio = &perfd->perio[0];
/*
	//perio
	ret = perio->count;
	if(0 == ret)return 0;	//disconnect
*/
	//copy data
	src = perio->bufing.buf;
	dst = buf;
	for(j=0;j<ret;j++)dst[j] = src[j];

	//copy addr
	union addrv4v6* tmp;
	switch(oo->type){
	case _UDP_:
	case _udp_:
		if(arg){
			dst = (void*)arg;
			src = oo->sockinfo.peer;
			for(j=0;j<8;j++)dst[j] = src[j];
		}
		break;
	}

	//perio->count = 0;

	iocp_mod(sock, oo->type);
	return ret;
}
int socket_writer(_obj* oo,int xx, p64 arg,int cmd, void* buf, int len)
{
	if(0 == buf)return 0;
	if(0 == len){
		logtoall("%s: refuse to send 0 length packet\n", __func__);
		return 0;
	}

	SOCKET sock = getsockbyobj(oo);
	//struct perfd* perfd = (void*)(oo->priv_256b);
	//struct perio* perio = &perfd->perio[0];
	//printf("write:sock=%x,len=%x\n",sock,len);

	DWORD dwret;
	WSABUF wbuf;
	int ret, cnt, socklen;
	struct sockaddr_in* tmp;
	switch(oo->type){
	case _UDP_:
	case _udp_:
		if(arg)tmp = (void*)arg;
		else tmp = (void*)oo->sockinfo.peer;

		socklen = (AF_INET6 == tmp->sin_family) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in);
		//printf("@writesocket: ver=%x\n",socklen);
		//printmemory(tmp, socklen);

		wbuf.buf = buf;
		wbuf.len = len;
		ret = WSASendTo(sock,
			&wbuf, 1,
			&dwret, 0,
			(void*)tmp, socklen,
			0, 0
		);
		break;
	default:
		wbuf.buf = buf;
		wbuf.len = len;
		ret = WSASend(sock, &wbuf, 1, &dwret, 0, 0, 0);
		//printf("@send:len=%d,ret=%d,err=%d\n",len,ret,GetLastError());
	}
	return len;
}




int socket_take(_obj* oo,int xx, _syn* stack,int sp, p64 arg,int cmd, void* buf, int len)
{
	return 0;
}
int socket_give(_obj* oo,int xx, _syn* stack,int sp, p64 arg,int cmd, void* buf, int len)
{
	return 0;
}
int socket_attach()
{
	return 0;
}
int socket_detach()
{
	return 0;
}



/*
int socket_search(_obj* oo)
{
	return 0;
}
int socket_modify(_obj* oo)
{
	return 0;
}*/