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
#include "system.h"
#define MAXSIZE 4096
void epoll_add(int);
void epoll_del(int);
void epoll_mod(int);




struct object* obj;
//
static int btfd = 0;
static int rawfd = 0;
static int tcpfd = 0;
static int udpfd = 0;




void selfname(u64 fd, u32* buf)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	getsockname(fd, (void*)&addr, &len);

	buf[0] = *(u32*)&addr.sin_addr;
	buf[1] = addr.sin_port;
}
void peername(u64 fd, u32* buf)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	getpeername(fd, (void*)&addr, &len);

	buf[0] = *(u32*)&addr.sin_addr;
	buf[1] = addr.sin_port;
}




int writesocket(int fd, int off, void* buf, int len)
{
	u64 type;
	int ret;
	if(fd == 0)return 0;
	if(buf == 0)return 0;

	type = obj[fd].type;
	if( (type == 'U') | (type == 'u') )
	{
		ret = sendto(
			fd, buf, len, 0,
			(void*)(obj[fd].peer), sizeof(struct sockaddr_in)
		);
		return ret;
	}

	ret = write(fd, buf, len);
	return ret;
}
int readsocket(int fd, int off, void* buf, int len)
{
	u64 type;
	int ret, cnt=0;
	if(fd == 0)return 0;
	if(buf == 0)return 0;

	type = obj[fd].type;
	if( (type == 'U') | (type == 'u') )
	{
		ret = sizeof(struct sockaddr_in);
		ret = recvfrom(
			fd, buf, len, 0,
			(void*)(obj[fd].peer), (void*)&ret
		);
		return ret;
	}
/*
	cnt = read(fd, buf, len);
	if(cnt <= 0)return 0;

	while(1)
	{
		ret = read(fd, buf+cnt, len-cnt);
		if(ret == -1)
		{
			if(errno != EAGAIN)return 0;
			else break;
		}
		else if(ret == 0)return 0;
		else cnt += ret;
	}
*/
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
int listsocket()
{
	return 0;
}
int choosesocket()
{
	return 0;
}
void stopsocket(int x)
{
	int ret = close(x);
	obj[x].type = 0;
	obj[x].name = 0;
	printf("---- %d %d, %d\n", x, ret, errno);

	//epoll_del(x);
	//if(tcpfd>0)epoll_del(tcpfd);
}
int startsocket(char* addr, int port, int type)
{
	int fd;
	int ret;
	struct hostent* host;
	for(ret=0;ret<256;ret++)
	{
		if((addr[ret]>='a')&&(addr[ret]<='z'))
		{
			host = gethostbyname(addr);
			addr = inet_ntoa(*(struct in_addr*)host->h_addr_list[0]);
			break;
		}
	}
	if(type == 'R')		//RAW
	{
		int ret;
		int len;
		struct ifreq ifopts;

		//create
		rawfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
		if(rawfd == -1)
		{
			printf("error%d@socket:%d\n",errno,rawfd);
			return 0;
		}

		//reuse
		ret = 1;
		ret = setsockopt(rawfd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
		if(ret == -1)
		{
			perror("setsockopt");
			close(rawfd);
			return 0;
		}

		//setup
		len = strlen(addr);
		strncpy(ifopts.ifr_name, addr, len);
		ioctl(rawfd, SIOCGIFFLAGS, &ifopts);
		ifopts.ifr_flags |= IFF_PROMISC;
		ioctl(rawfd, SIOCSIFFLAGS, &ifopts);

		//bind
		ret = setsockopt(rawfd, SOL_SOCKET, SO_BINDTODEVICE, addr, len);
		if(ret == -1)
		{
			perror("SO_BINDTODEVICE");
			close(rawfd);
			return 0;
		}

		//done
		obj[rawfd].type = type;
		obj[rawfd].name = 0;
		epoll_add(rawfd);
		return rawfd;
	}
	if(type == 'r')		//raw
	{
		return 0;
	}
	else if(type == 'U')	//UDP
	{
		int ret;
		struct sockaddr_in* self;

		//create
		udpfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (udpfd == -1)
		{
			printf("error@socket\n");
			return 0;
		}
		if(udpfd > MAXSIZE)
		{
			printf("udpfd>4096\n");
			return 0;
		}

		//reuse
		ret = 1;
		ret = setsockopt(udpfd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
		if(ret<0)
		{
			printf("error@setsockopet\n");
			return 0;
		}

		//self
		self = (void*)obj[udpfd].self;
		memset(self, 0, sizeof(struct sockaddr_in));
		self->sin_family = AF_INET;
		self->sin_port = htons(port);
		self->sin_addr.s_addr = htons(INADDR_ANY);
		ret = bind(udpfd, (void*)self, sizeof(struct sockaddr_in));
		if(ret == -1)
		{
			printf("error@bind\n");
			close(udpfd);
			return 0;
		}

		//done
		obj[udpfd].type = type;
		obj[udpfd].name = 0;
		epoll_add(udpfd);
		return udpfd;
	}
	else if(type == 'u')	//udp
	{
		struct sockaddr_in* self;
		struct sockaddr_in* peer;
		if(udpfd > 0)goto udpnext;

		//create
		udpfd = socket(AF_INET, SOCK_DGRAM, 0);
		if(udpfd == -1)
		{
			printf("error%d@socket:%d\n",errno,udpfd);
			return 0;
		}
		if(udpfd > MAXSIZE)
		{
			printf("udpfd>4096\n");
			return 0;
		}

		//reuse
		ret = 1;
		ret = setsockopt(udpfd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
		if(ret<0)
		{
			printf("error@setsockopet\n");
			return 0;
		}

		//self
		self = (void*)obj[udpfd].self;
		memset(self, 0, sizeof(struct sockaddr_in));
		self->sin_family = AF_INET;
		self->sin_port = htons(2222);
		self->sin_addr.s_addr = htons(INADDR_ANY);
		ret = bind(udpfd, (void*)self, sizeof(struct sockaddr_in));
		if(ret == -1)
		{
			printf("error@bind\n");
			close(udpfd);
			return 0;
		}

udpnext:
		//peer
		peer = (void*)obj[udpfd].peer;
		memset(peer, 0, sizeof(struct sockaddr_in));
		peer->sin_family = AF_INET;
		peer->sin_port = htons(port);
		peer->sin_addr.s_addr = inet_addr(addr);

		//done
		obj[udpfd].type = type;
		obj[udpfd].name = 0;
		epoll_add(udpfd);
		return udpfd;
	}
	else if(type == 'T')	//TCP
	{
		int ret;
		struct sockaddr_in* self;

		//create
		tcpfd = socket(AF_INET, SOCK_STREAM, 0);
		if (tcpfd == -1)
		{
			printf("error@socket\n");
			return 0;
		}
		if(tcpfd > MAXSIZE)
		{
			printf("tcpfd>4096\n");
			return 0;
		}

		//reuse
		ret = 1;
		ret = setsockopt(tcpfd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
		if(ret<0)
		{
			printf("error@setsockopet\n");
			return 0;
		}

		//self
		self = (void*)obj[tcpfd].self;
		memset(self, 0, sizeof(struct sockaddr_in));
		self->sin_family = AF_INET;
		self->sin_port = htons(port);
		self->sin_addr.s_addr = htons(INADDR_ANY);
		ret = bind(tcpfd, (void*)self, sizeof(struct sockaddr_in));
		if(ret == -1)
		{
			printf("error@bind\n");
			close(tcpfd);
			return 0;
		}

		//work
		listen(tcpfd, 5);

		//done
		obj[tcpfd].type = type;
		obj[tcpfd].name = 0;
		epoll_add(tcpfd);
		return tcpfd;
	}
	else if(type == 't')	//tcp
	{
		int ret;
		struct sockaddr_in* peer;

		//create socket
		fd = socket(AF_INET, SOCK_STREAM, 0);
		if(fd == -1)
		{
			printf("error%d@socket:%d\n",errno,fd);
			return 0;
		}

		//reuse
		ret = 1;
		ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
		if(ret<0)
		{
			printf("error@setsockopet\n");
			return 0;
		}

		//peer
		peer = (void*)obj[fd].peer;
		memset(peer, 0, sizeof(struct sockaddr_in));
		peer->sin_family = AF_INET;
		peer->sin_addr.s_addr = inet_addr(addr);
		peer->sin_port = htons(port);

		//work
		ret = connect(fd, (void*)peer, sizeof(struct sockaddr_in));
		if(ret < 0)
		{
			printf("connect error\n");
			return 0;
		}

		//done
		obj[fd].type = type;
		obj[fd].name = 0;
		epoll_add(fd);
		return fd;
	}
	else if(type == 'B')	//BT
	{
		return 0;
	}
	else if(type == 'b')	//bt
	{
		return 0;
	}
	else printf("error@type\n");
	return 0;
}
void deletesocket(int num)
{
}
void createsocket(void* addr)
{
	struct sigaction sa;
	sa.sa_handler=SIG_IGN;
	sigaction(SIGPIPE, &sa, 0);

	obj = addr;
}
