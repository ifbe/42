#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "libsoft.h"
#define MAXSIZE 4096
#define _connect_ hex32('c','o','n', 0)
#define _listen_ hex32('l','i','s', 0)
void kqueue_add(int);
void kqueue_del(int);
void kqueue_mod(int);
int parsemyandto(void*,int, void*,int, char**,int*, char**,int*);




union addrv4v6{
	struct sockaddr sa;		//just make compiler happy
	struct sockaddr_in v4;
	struct sockaddr_in6 v6;
};
struct item* obj;




void initsocket(void* addr)
{
	struct sigaction sa;
	sa.sa_handler=SIG_IGN;
	sigaction(SIGPIPE, &sa, 0);

	obj = addr;
}
void freesocket()
{
}




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
		say("error@select\n");
		return 0;
	}
	if(ret == 0){
		say("timeout@select\n");
		return 0;
	}

	socklen_t retlen = sizeof(ret);
	if(getsockopt(sock, SOL_SOCKET, SO_ERROR, &ret, &retlen) < 0){
		say("errno=%d@getsockopt\n",errno);
		return 0;
	}
	if(ret != 0){
		say("errno=%d@waitconnect\n",ret);
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
	int fd,ret;
	say("macos doesnot support rawsocket!\n");

	//create
	fd = socket(AF_INET, SOCK_RAW, 0);
	if(-1 == fd){
		printf("errno=%d@socket\n", errno);
		return 0;
	}

	kqueue_add(fd);
	return &obj[fd];
}
_obj* createsocket_udpserver(char* addr, int port)
{
	int fd,ret,size;
	int sockfmt,socklen;
	union addrv4v6 sockbuf;

	memset(&sockbuf, 0, sizeof(sockbuf));
	if(1){
		sockfmt = AF_INET;
		socklen = sizeof(struct sockaddr_in);
		sockbuf.v4.sin_family = sockfmt;
		sockbuf.v4.sin_port = htons(port);
		sockbuf.v4.sin_addr.s_addr = htons(INADDR_ANY);
	}
	else{
		sockfmt = AF_INET6;
		socklen = sizeof(struct sockaddr_in6);
		sockbuf.v6.sin6_family = sockfmt;
		sockbuf.v6.sin6_port = htons(port);
		sockbuf.v6.sin6_addr = in6addr_any;
	}

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

	//reuse
	size = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &size, 4);
	if(ret < 0){
		printf("errno=%d@setsockopt\n",errno);
		return 0;
	}

	//bind
	ret = bind(fd, &sockbuf.sa, socklen);
	if(-1 == ret){
		printf("errno=%d@bind\n",errno);
		close(fd);
		return 0;
	}

	//done
	memcpy(obj[fd].sockinfo.self, &sockbuf, socklen);
	kqueue_add(fd);
	return &obj[fd];
}
_obj* createsocket_udpclient(char* myaddr, int myport, char* toaddr, int toport)
{
	int fd,ret;
	struct sockaddr_in* self;
	struct sockaddr_in* peer;

	//create
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == fd){
		printf("errno=%d@socket\n",errno);
		return 0;
	}
	if(fd > MAXSIZE){
		printf("fd>4096\n");
		return 0;
	}

	//reuse
	ret = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
	if(ret < 0){
		printf("errno=%d@setsockopt\n",errno);
		return 0;
	}

if((0 != myaddr) && (0 != myport)){
	//self
	self = (void*)obj[fd].sockinfo.self;
	memset(self, 0, sizeof(struct sockaddr_in));
	self->sin_family = AF_INET;
	self->sin_port = htons(myport);
	self->sin_addr.s_addr = inet_addr(myaddr);

	//bind
	ret = bind(fd, (void*)self, sizeof(struct sockaddr_in));
	if(-1 == ret){
		printf("errno=%d@bind\n",errno);
		close(fd);
		return 0;
	}
}

	//peer
	peer = (void*)obj[fd].sockinfo.peer;
	memset(peer, 0, sizeof(struct sockaddr_in));
	peer->sin_family = AF_INET;
	peer->sin_port = htons(toport);
	peer->sin_addr.s_addr = inet_addr(toaddr);

	//connect
	ret = connect(fd, (void*)peer, sizeof(struct sockaddr_in));
	if(ret < 0){
		printf("errno=%d@connect\n",errno);
		return 0;
	}

	//get the random port
	self = (void*)obj[fd].sockinfo.self;
	socklen_t len = sizeof(struct sockaddr_in);
	getsockname(fd, (void*)self, &len);
	printf("myaddr=%s:%d\n", inet_ntoa(self->sin_addr), ntohs(self->sin_port));

	//done
	kqueue_add(fd);
	return &obj[fd];
}
_obj* createsocket_tcpserver(char* addr, int port)
{
	int fd,ret,size;
	int sockfmt,socklen;
	union addrv4v6 sockbuf;

	memset(&sockbuf, 0, sizeof(sockbuf));
	if(0){
		sockfmt = AF_INET;
		socklen = sizeof(struct sockaddr_in);
		sockbuf.v4.sin_family = sockfmt;
		sockbuf.v4.sin_port = htons(port);
		sockbuf.v4.sin_addr.s_addr = htons(INADDR_ANY);
	}
	else{
		sockfmt = AF_INET6;
		socklen = sizeof(struct sockaddr_in6);
		sockbuf.v6.sin6_family = sockfmt;
		sockbuf.v6.sin6_port = htons(port);
		sockbuf.v6.sin6_addr = in6addr_any;
	}

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
	ret = bind(fd, &sockbuf.sa, socklen);
	if(-1 == ret){
		printf("errno=%d@bind\n",errno);
		close(fd);
		return 0;
	}

	//work
	listen(fd, 5);

	//done
	memcpy(obj[fd].sockinfo.self, &sockbuf, socklen);
	kqueue_add(fd);
	return &obj[fd];
}
_obj* createsocket_tcpclient(char* myaddr, int myport, char* toaddr, int toport)
{
	int fd,ret;
	struct sockaddr_in* self;
	struct sockaddr_in* peer;

	//create socket
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == fd){
		printf("errno=%d@socket\n",errno);
		return 0;
	}

	//reuse
	ret = 1;
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

if((0 != myaddr) && (0 != myport)){
	//self
	self = (void*)obj[fd].sockinfo.self;
	memset(self, 0, sizeof(struct sockaddr_in));
	self->sin_family = AF_INET;
	self->sin_port = htons(myport);
	self->sin_addr.s_addr = inet_addr(myaddr);

	//bind
	ret = bind(fd, (void*)self, sizeof(struct sockaddr_in));
	if(-1 == ret){
		printf("errno=%d@bind\n",errno);
		close(fd);
		return 0;
	}
}

	//peer
	peer = (void*)obj[fd].sockinfo.peer;
	memset(peer, 0, sizeof(struct sockaddr_in));
	peer->sin_family = AF_INET;
	peer->sin_port = htons(toport);
	peer->sin_addr.s_addr = inet_addr(toaddr);

	//connect
	fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL, 0));
	//printf("@@@@@@@@before connect\n");
	ret = connect(fd, (void*)peer, sizeof(struct sockaddr_in));
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

	//get the random port
	self = (void*)obj[fd].sockinfo.self;
	socklen_t len = sizeof(struct sockaddr_in);
	getsockname(fd, (void*)self, &len);
	printf("myaddr=%s:%d\n", inet_ntoa(self->sin_addr), ntohs(self->sin_port));

	//done
	kqueue_add(fd);
	return &obj[fd];
}




_obj* socket_create(int fmt, char* arg)
{
	int j;
	char tmp[256];

	int myport = 0;
	char* myaddr = 0;
	int toport = 0;
	char* toaddr = 0;
	if(0 == arg)goto skip;

	//my->to
	for(j=0;j<256;j++){
		if(arg[j] <= 0x20)break;
		parsemyandto(arg, 256, tmp, 256, &myaddr, &myport, &toaddr, &toport);
	}
	if(myaddr)socket_fixaddr(myaddr);
	if(toaddr)socket_fixaddr(toaddr);
	//printmemory(tmp,256);

skip:
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
int socket_delete(_obj* oo)
{
	int fd = oo->sockinfo.fd;
	int ret = close(fd);
	printf("----fd=%d,ret=%d,errno=%d\n", fd, ret, errno);
	return ret;
}
int socket_search(_obj* oo,int xx)
{
	return 0;
}
int socket_modify(_obj* oo,int xx, void* arg,int cmd, u8* addr,int port)
{
	if(_connect_ == cmd){
		u8* p = oo->sockinfo.self;
		u8* q = addr;
		say("reuse this for connect: %d.%d.%d.%d:%d -> %d.%d.%d.%d:%d\n",
			p[4],p[5],p[6],p[7], (p[2]<<8)+p[3],
			q[0],p[1],q[2],q[3], port
		);
	}
	if(_listen_ == cmd){
		say("reuse this for listen\n");
	}
	return 0;
}




int socket_link()
{
	return 0;
}
int socket_gone()
{
	return 0;
}
int socket_give(_obj* oo,int xx, struct sockaddr_in* tmp, void* buf, int len)
{
//say("@writesocket:%x,%llx,%llx,%x\n",fd,tmp,buf,len);
	if(buf == 0)return 0;

	int fd = oo->sockinfo.fd;
	if(fd < 0)return 0;

	int ret, cnt=0;
	u64 type = oo->type;
	if(_UDP_ == type){
		struct sockaddr_in out;
		if(0 == tmp)tmp = (void*)oo->sockinfo.peer;
		else{
			memset(&out, 0, sizeof(struct sockaddr_in));
			out.sin_family = AF_INET;
			out.sin_port = tmp->sin_port;
			out.sin_addr.s_addr = tmp->sin_addr.s_addr;
			tmp = &out;
		}

		ret = sendto(
			fd, buf, len, 0,
			(void*)tmp, sizeof(struct sockaddr_in)
		);
		return ret;
	}
	if(_udp_ == type){
		ret = 0;
		while(1){
			if(len-ret <= 1024){
				write(fd, buf+ret, len-ret);
				break;
			}
			else{
				write(fd, buf+ret, 1024);
				ret += 1024;
			}
		}
		return len;
	}

	cnt = 0;
	while(1){
		ret = write(fd, buf+cnt, len-cnt);
		if(ret < 0){
			say("@writesocket: ret=%d,errno=%d\n", ret, errno);
			if(EAGAIN != errno)return -1;

			usleep(1000);
			continue;
		}

		cnt += ret;
		if(cnt == len)break;
		say("@writesocket: %x/%x\n", cnt, len);
	}
	return ret;
}
int socket_take(_obj* oo,int xx, void* tmp,int cmd, void* buf,int len)
{
	if(0 == buf)return 0;

	int fd = oo->sockinfo.fd;
	if(fd < 0)return 0;

	int ret, cnt=0;
	u64 type = oo->type;
	if(_UDP_ == type)
	{
		cnt = sizeof(union addrv4v6);
		ret = recvfrom(
			fd, buf, len, 0,
			tmp, (void*)&cnt
		);
		//printf("socklen=%d\n",cnt);
		//printmemory(tmp, 32);
		return ret;
	}

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
	return ret;
}