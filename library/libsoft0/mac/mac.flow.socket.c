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




struct object* obj;




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
	int fd,ret;

	//create
	fd = socket(AF_INET, SOCK_RAW, 0);
	if(-1 == fd){
		printf("errno=%d@socket\n", errno);
		return 0;
	}

	//reuse
	ret = 1;
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
	if(-1 == ret){
		printf("errno=%d@setsockopt\n", errno);
		close(fd);
		return 0;
	}

	kqueue_add(fd);
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
	kqueue_add(fd);
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
	if(ret < 0){
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
	kqueue_add(fd);
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
	kqueue_add(fd);
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
	kqueue_add(fd);
	return fd;
}




int writesocket(int fd, struct sockaddr_in* tmp, void* buf, int len)
{
	int cnt,ret;
	u64 type;
	if(fd == 0)return 0;
	if(buf == 0)return 0;
//say("@writesocket:%x,%llx,%llx,%x\n",fd,tmp,buf,len);

	type = obj[fd].type;
	if(_UDP_ == type){
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
	return ret;
}




int searchsocket(int fd)
{
	return 0;
}
int modifysocket(int fd, int arg, u8* addr, int port)
{
	if(_connect_ == arg){
		u8* p = obj[fd].self;
		u8* q = addr;
		say("reuse this for connect: %d.%d.%d.%d:%d -> %d.%d.%d.%d:%d\n",
			p[4],p[5],p[6],p[7], (p[2]<<8)+p[3],
			q[0],p[1],q[2],q[3], port
		);
	}
	if(_listen_ == arg){
		say("reuse this for listen\n");
	}
	return 0;
}
int deletesocket(int fd)
{
	int ret = close(fd);
	printf("----fd=%d,ret=%d,errno=%d\n", fd, ret, errno);
	return ret;
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
			say("ipv4=%x\n",ipv4);
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




void freesocket()
{
}
void initsocket(void* addr)
{
	struct sigaction sa;
	sa.sa_handler=SIG_IGN;
	sigaction(SIGPIPE, &sa, 0);

	obj = addr;
}
