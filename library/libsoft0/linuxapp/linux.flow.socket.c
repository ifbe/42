#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <netdb.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "libsoft.h"
#define MAXSIZE 4096
void epoll_add(int);
void epoll_del(int);
void epoll_mod(int);
int parseipv6addr(u8* buf, u8* out);
int parsemyandto(void*,int, void*,int, char**,int*, char**,int*);




union addrv4v6{
	struct sockaddr sa;		//just make compiler happy
	struct sockaddr_in v4;
	struct sockaddr_in6 v6;
};




static _obj* g_obj;
void initsocket(void* addr)
{
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &sa, 0);

	g_obj = addr;
}
void freesocket(int num)
{
}
static _obj* getobjbysock(int fd)
{
	return &g_obj[fd];
}
static int getsockbyobj(_obj* oo)
{
	return oo->sockinfo.fd;
}



/*
int selfname(u64 fd, u32* buf)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	getsockname(fd, (void*)&addr, &len);

	buf[0] = *(u32*)&addr.sin_addr;
	buf[1] = addr.sin_port;
	return 1;
}
int peername(u64 fd, u32* buf)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	getpeername(fd, (void*)&addr, &len);

	buf[0] = *(u32*)&addr.sin_addr;
	buf[1] = addr.sin_port;
	return 1;
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
		printf("ipadd:%s\n", inet_ntoa(*(struct in_addr*)ptr[j]));
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
int waitconnectwithselect(int sock)
{
	int ret;
	fd_set fds;
	struct timeval time;
	//printf("waiting\n");

	FD_ZERO(&fds);
	FD_SET(sock, &fds);

	time.tv_sec = 3;
	time.tv_usec = 0;

	ret = select(sock+1, NULL, &fds, NULL, &time);
	if(ret < 0){
		logtoall("error@select\n");
		return 0;
	}
	if(ret == 0){
		logtoall("timeout@select\n");
		return 0;
	}

	socklen_t retlen = sizeof(ret);
	if(getsockopt(sock, SOL_SOCKET, SO_ERROR, &ret, &retlen) < 0){
		logtoall("errno=%d@getsockopt\n",errno);
		return 0;
	}
	if(ret != 0){
		logtoall("errno=%d@waitconnect\n",ret);
		return 0;
	}
	return 1;
}




_obj* createsocket_bt(char* addr, int port)
{
	return 0;
}
_obj* createsocket_raw(char* addr, int port)
{
	int fd,ret,len;
	struct ifreq ifopts;

	//create
	fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(-1 == fd){
		printf("errno=%d@socket\n",errno);
		return 0;
	}

	//mem
	_obj* oo = getobjbysock(fd);
	logtoall("fd=%x,oo=%p\n", fd, oo);
	oo->sockinfo.fd = fd;

	//reuse
	ret = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
	if(-1 == ret){
		printf("errno=%d@setsockopt\n",errno);
		close(fd);
		return 0;
	}

	//setup
	for(ret=0;ret<IFNAMSIZ;ret++)
	{
		if(addr[ret] < 0x20){ifopts.ifr_name[ret] = 0;break;}
		else ifopts.ifr_name[ret] = addr[ret];
	}
	ioctl(fd, SIOCGIFFLAGS, &ifopts);
	ifopts.ifr_flags |= IFF_PROMISC;
	ioctl(fd, SIOCSIFFLAGS, &ifopts);

	//reuse
	len=0;
	ret = setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, addr, len);
	if(-1 == ret){
		printf("errno=%d@SO_BINDTODEVICE\n",errno);
		close(fd);
		return 0;
	}

	//done
	epoll_add(fd);
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

	int fd,ret,size;
	//create
	fd = socket(sockfmt, SOCK_DGRAM, 0);
	if(-1 == fd){
		printf("errno=%d@socket\n",errno);
		return 0;
	}
	if(fd > MAXSIZE)
	{
		printf("fd>4096\n");
		return 0;
	}

	//mem
	_obj* oo = getobjbysock(fd);
	logtoall("fd=%x,oo=%p\n", fd, oo);
	oo->sockinfo.fd = fd;

	//reuse
	size = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &size, 4);
	if(ret < 0){
		printf("errno=%d@setsockopt\n",errno);
		return 0;
	}

	//bind
	ret = bind(fd, (void*)my, socklen);
	if(-1 == ret){
		printf("errno=%d@bind\n",errno);
		close(fd);
		return 0;
	}

	//self
	union addrv4v6* self = (void*)oo->sockinfo.self;
	memcpy((void*)self, my, socklen);

	epoll_add(fd);
	return oo;
}
_obj* createsocket_udpclient(union addrv4v6* my, union addrv4v6* to)
{
	int sockfmt;
	int socklen;
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

	int fd,ret;
	//create
	fd = socket(sockfmt, SOCK_DGRAM, 0);
	if(-1 == fd){
		printf("errno=%d@socket\n",errno);
		return 0;
	}
	if(fd > MAXSIZE){
		printf("fd>4096\n");
		return 0;
	}

	//mem
	_obj* oo = getobjbysock(fd);
	logtoall("fd=%x,oo=%p\n", fd, oo);
	oo->sockinfo.fd = fd;

	//reuse
	ret = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
	if(ret < 0)
	{
		printf("errno=%d@setsockopt\n",errno);
		return 0;
	}

if(my){
	ret = bind(fd, (void*)my, socklen);
	if(-1 == ret){
		printf("errno=%d@bind\n",errno);
		close(fd);
		return 0;
	}
}

	//connect
	ret = connect(fd, (void*)to, socklen);
	if(ret < 0){
		printf("errno=%d@connect\n",errno);
		return 0;
	}

	//self
	union addrv4v6* self = (void*)oo->sockinfo.self;
	socklen_t len = socklen;
	getsockname(fd, (void*)(oo->sockinfo.self), &len);
	printmemory(self, 32);

	//peer
	union addrv4v6* peer = (void*)oo->sockinfo.self;
	memcpy((void*)peer, to, socklen);

	//done
	epoll_add(fd);
	return oo;
}
_obj* createsocket_tcpserver(union addrv4v6* my)
{
	//
	int sockfmt,socklen;
	if(AF_INET6 == my->sa.sa_family){
		sockfmt = AF_INET6;
		socklen = sizeof(struct sockaddr_in6);
	}
	else{
		sockfmt = AF_INET;
		socklen = sizeof(struct sockaddr_in);
	}

	int fd,ret,size;
	//create
	fd = socket(sockfmt, SOCK_STREAM, 0);
	if(-1 == fd){
		printf("errno=%d@socket\n",errno);
		return 0;
	}
	if(fd > MAXSIZE){
		printf("fd>4096\n");
		return 0;
	}

	//mem
	_obj* oo = getobjbysock(fd);
	logtoall("fd=%x,oo=%p\n", fd, oo);
	oo->sockinfo.fd = fd;

	//reuse
	size = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &size, 4);
	if(ret < 0){
		printf("errno=%d@SO_REUSEADDR\n",errno);
		return 0;
	}
#ifdef SO_REUSEPORT
	size = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &size, 4);
	if(ret < 0){
		printf("errno=%d@SO_REUSEPORT\n",errno);
		return 0;
	}
#endif

	//bind
	ret = bind(fd, (void*)my, socklen);
	if(-1 == ret){
		printf("errno=%d@bind\n",errno);
		close(fd);
		return 0;
	}

	//work
	listen(fd, 5);

	union addrv4v6* self = (void*)oo->sockinfo.self;
	memcpy((void*)self, my, socklen);

	//done
	epoll_add(fd);
	return oo;
}
_obj* createsocket_tcpclient(union addrv4v6* my, union addrv4v6* to)
{
	int sockfmt;
	int socklen;
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

	//create socket
	int fd = socket(sockfmt, SOCK_STREAM, 0);
	if(-1 == fd){
		printf("errno=%d@socket\n",errno);
		return 0;
	}

	//mem
	_obj* oo = getobjbysock(fd);
	logtoall("fd=%x,oo=%p\n", fd, oo);
	oo->sockinfo.fd = fd;

	//reuse
	int ret = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
	if(ret < 0){
		printf("errno=%d@SO_REUSEADDR\n",errno);
		return 0;
	}
#ifdef SO_REUSEPORT
	ret = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &ret, 4);
	if(ret < 0){
		printf("errno=%d@SO_REUSEPORT\n",errno);
		return 0;
	}
#endif

if(my){
	ret = bind(fd, (void*)my, socklen);
	if(-1 == ret){
		printf("errno=%d@bind\n",errno);
		close(fd);
		return 0;
	}
}

	//connect
	fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL, 0));
	//printf("@@@@@@@@before connect\n");
	ret = connect(fd, (void*)to, socklen);
	//printf("@@@@@@@@after connect\n");
	if(ret < 0){
		if(EINPROGRESS != errno){
			printf("errno=%d@connect\n",errno);
			return 0;
		}
		if(waitconnectwithselect(fd) <= 0){
			close(fd);
			return 0;
		}
	}

	//self
	union addrv4v6* self = (void*)oo->sockinfo.self;
	socklen_t len = sizeof(struct sockaddr_in);
	getsockname(fd, (void*)self, &len);
	printmemory(self, 32);

	//peer
	union addrv4v6* peer = (void*)oo->sockinfo.self;
	memcpy((void*)peer, to, socklen);

	//done
	epoll_add(fd);
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
	case _RAW_:return createsocket_raw(myaddr, myport);
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
	int fd = oo->sockinfo.fd;
	int ret = close(fd);
	printf("----fd=%d,ret=%d,errno=%d\n", fd, ret, errno);
	return ret;
	//epoll_del(x);
	//if(fd>0)epoll_del(fd);
}
int socket_writer(_obj* oo,int xx, p64 arg,int cmd, void* buf,int len)
{
	if(0 == buf)return 0;
	if(0 == len){
		logtoall("%s: refuse to send 0 length packet\n", __func__);
		return 0;
	}

	int fd = oo->sockinfo.fd;
	if(fd <= 0)return 0;
	//
	int ret;
	int pos;
	int socklen;
	struct sockaddr_in* tmp;
	//
	int dbgcnt = 0;
	u64 dbgtime_old = 0;
	u64 dbgtime_new = 0;
	//
	switch(oo->type){
	case _UDP_:
	case _udp_:
		if(arg)tmp = (void*)arg;
		else tmp = (void*)oo->sockinfo.peer;

		socklen = (AF_INET6 == tmp->sin_family) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in);
		printf("@writesocket: ver=%x\n",socklen);
		printmemory(tmp, socklen);

		ret = sendto(
			fd, buf, len, 0,
			(void*)tmp, socklen
		);
		len = ret;
		break;
	default:
		//must check, don't trust
		pos = 0;
		while(1){
			//logtoall("@write: pos=%x,len=%x\n", pos, len-pos);
			ret = write(fd, buf+pos, len-pos);
			if(ret < 0){
				//print log or shutup
				dbgcnt += 1;
				dbgtime_new = timeread_us();
				if( (EAGAIN != errno) | (dbgtime_new > dbgtime_old+1000*1000) ){
					logtoall("@writesocket: pos=%d,len=%d, ret=%d,errno=%d,dbgcnt=%d\n",
						pos, len-pos,
						ret, errno,  dbgcnt
					);
					dbgtime_old = dbgtime_new;
				}

				if(EAGAIN != errno)return -1;

				usleep(1000);
				continue;
			}

			pos += ret;
			if(pos == len){
				logtoall("write finish: dbgcnt=%d\n", dbgcnt);
				break;
			}
		}
	}
	return len;
}
int socket_reader(_obj* oo,int xx, p64 arg,int cmd, void* buf,int len)
{
	if(0 == buf)return 0;

	int fd = oo->sockinfo.fd;
	if(fd < 0)return 0;

	int ret, cnt=0;
	u64 type = oo->type;
	struct sockaddr* tmp;
	switch(type){
	case _UDP_:
	case _udp_:
		if(arg)tmp = (void*)arg;
		else tmp = (void*)oo->sockinfo.peer;
		cnt = sizeof(union addrv4v6);
		ret = recvfrom(
			fd, buf, len, 0,
			tmp, (void*)&cnt
		);
		//printf("socklen=%d\n",cnt);
		//printmemory(tmp, 32);
		break;
	default:
		while(1)
		{
			ret = read(fd, buf, len);
			if(ret > 0)break;
			if(ret == 0)return -1;		//disconnect
			if(errno != 11)return -2;	//errno
			if(cnt >= 100)return 0;

			usleep(1000);
			cnt++;
		}
	}
	epoll_mod(fd);
	return ret;
}




int socket_attach()
{
	return 0;
}
int socket_detach()
{
	return 0;
}
int socket_take(_obj* oo,int xx, _syn* stack,int sp, p64 arg,int cmd, void* buf, int len)
{
	return 0;
}
int socket_give(_obj* oo,int xx, _syn* stack,int sp, p64 arg,int cmd, void* buf, int len)
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
