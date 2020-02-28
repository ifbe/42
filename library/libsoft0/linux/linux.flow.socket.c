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




struct object* obj;



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
int waitconnectwithselect(int sock)
{
	int ret;
	fd_set fds;
	struct timeval time;
	printf("waiting\n");

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

	return 1;
}




int createsocket_raw(char* addr, int port)
{
	int fd,ret,len;
	struct ifreq ifopts;

	//create
	fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(-1 == fd){
		printf("errno=%d@socket\n",errno);
		return 0;
	}

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
	ret = setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, addr, len);
	if(-1 == ret){
		printf("errno=%d@SO_BINDTODEVICE\n",errno);
		close(fd);
		return 0;
	}

	//done
	epoll_add(fd);
	return fd;
}
int createsocket_udpserver(char* addr, int port)
{
	int fd,ret;
	struct sockaddr_in* self;

	//create
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == fd){
		printf("errno=%d@socket\n",errno);
		return 0;
	}
	if(fd > MAXSIZE)
	{
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

	//self
	self = (void*)obj[fd].self;
	memset(self, 0, sizeof(struct sockaddr_in));
	self->sin_family = AF_INET;
	self->sin_port = htons(port);
	self->sin_addr.s_addr = htons(INADDR_ANY);

	//bind
	ret = bind(fd, (void*)self, sizeof(struct sockaddr_in));
	if(-1 == ret){
		printf("errno=%d@bind\n",errno);
		close(fd);
		return 0;
	}

	//done
	epoll_add(fd);
	return fd;
}
int createsocket_udpclient(char* myaddr, int myport, char* toaddr, int toport)
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
	if(ret < 0)
	{
		printf("errno=%d@setsockopt\n",errno);
		return 0;
	}

if((0 != myaddr) && (0 != myport)){
	//self
	self = (void*)obj[fd].self;
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
	peer = (void*)obj[fd].peer;
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
	self = (void*)obj[fd].self;
	socklen_t len = sizeof(struct sockaddr_in);
	getsockname(fd, (void*)self, &len);
	printf("myaddr=%s:%d\n", inet_ntoa(self->sin_addr), ntohs(self->sin_port));

	//done
	epoll_add(fd);
	return fd;
}
int createsocket_tcpserver(char* addr, int port)
{
	int fd,ret;
	struct sockaddr_in* self;

	//create
	fd = socket(AF_INET, SOCK_STREAM, 0);
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

	//self
	self = (void*)obj[fd].self;
	memset(self, 0, sizeof(struct sockaddr_in));
	self->sin_family = AF_INET;
	self->sin_port = htons(port);
	self->sin_addr.s_addr = htons(INADDR_ANY);

	//bind
	ret = bind(fd, (void*)self, sizeof(struct sockaddr_in));
	if(-1 == ret){
		printf("errno=%d@bind\n",errno);
		close(fd);
		return 0;
	}

	//work
	listen(fd, 5);

	//done
	epoll_add(fd);
	return fd;
}
int createsocket_tcpclient(char* myaddr, int myport, char* toaddr, int toport)
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
	self = (void*)obj[fd].self;
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
	peer = (void*)obj[fd].peer;
	memset(peer, 0, sizeof(struct sockaddr_in));
	peer->sin_family = AF_INET;
	peer->sin_port = htons(toport);
	peer->sin_addr.s_addr = inet_addr(toaddr);

	//connect
	fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL, 0));
	printf("@@@@@@@@before connect\n");
	ret = connect(fd, (void*)peer, sizeof(struct sockaddr_in));
	printf("@@@@@@@@after connect\n");
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
	self = (void*)obj[fd].self;
	socklen_t len = sizeof(struct sockaddr_in);
	getsockname(fd, (void*)self, &len);
	printf("myaddr=%s:%d\n", inet_ntoa(self->sin_addr), ntohs(self->sin_port));

	//done
	epoll_add(fd);
	return fd;
}




int writesocket(int fd, struct sockaddr_in* tmp, void* buf, int len)
{
	int ret,cnt;
	u64 type;
	if(fd == 0)return 0;
	if(buf == 0)return 0;

	type = obj[fd].type;
	if(_UDP_ == type)
	{
		struct sockaddr_in out;
		if(0 == tmp)tmp = (void*)obj[fd].peer;
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
		cnt = 0;
		while(1){
			if(len-cnt <= 1024){
				ret = write(fd, buf+cnt, len-cnt);
				break;
			}
			else{
				ret = write(fd, buf+cnt, 1024);
				cnt += 1024;
			}
		}
		return len;
	}

	//must check, don't trust
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
	return len;
}
int readsocket(int fd, void* tmp, void* buf, int len)
{
	u64 type;
	int ret, cnt=0;
	if(fd == 0)return 0;
	if(buf == 0)return 0;

	type = obj[fd].type;
	if(_UDP_ == type)
	{
		ret = sizeof(struct sockaddr_in);
		ret = recvfrom(
			fd, buf, len, 0,
			tmp, (void*)&ret
		);
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
	epoll_mod(fd);
	return ret;
}




int searchsocket(int fd)
{
	return 0;
}
int modifysocket(int fd)
{
	return 0;
}
int deletesocket(int fd)
{
	int ret = close(fd);
	printf("----fd=%d,ret=%d,errno=%d\n", fd, ret, errno);
	return ret;
	//epoll_del(x);
	//if(fd>0)epoll_del(fd);
}
int createsocket(char* addr, int port, int type)
{
	int j,fd,ret;
	u32 ipv4;

	//dns thing
	for(j=0;j<256;j++)
	{
		if(addr[j] <= 0x20)break;
		if((addr[j]>='a')&&(addr[j]<='z'))
		{
			ipv4 = resolvehostname(addr);
			if(0 == ipv4)return 0;

			addr = inet_ntoa(*(struct in_addr*)&ipv4);
			break;
		}
	}

	//type
	switch(type){
	case _RAW_:return createsocket_raw(addr,port);
	case _UDP_:return createsocket_udpserver(addr,port);
	case _udp_:return createsocket_udpclient(0, 0, addr,port);
	case _TCP_:return createsocket_tcpserver(addr,port);
	case _tcp_:return createsocket_tcpclient(0, 0, addr,port);
	default:printf("error@type\n");
	}
	return 0;
}




void freesocket(int num)
{
}
void initsocket(void* addr)
{
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &sa, 0);

	obj = addr;
}
