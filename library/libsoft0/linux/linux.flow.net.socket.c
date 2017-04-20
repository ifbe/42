#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#include<unistd.h>
#include<signal.h>
#include<arpa/inet.h>
#include<linux/if_ether.h>
#include<net/if.h>
#include<netinet/in.h>
#include<netinet/ether.h>
#include<sys/epoll.h>
#include<sys/ioctl.h>
#include<sys/socket.h>
#include<sys/types.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
#define MAXSIZE 4096
//
void eventwrite(u64,u64,u64,u64);
//
void stopthread();
u64 startthread(void*, void*);
u64 gettime();
u32 getrandom();
//
void printmemory(void*, int);
void say(void*, ...);




struct object
{
	//[0x00,0x3f]
	u64 type_sock;	//raw, bt, udp, tcp?
	u64 stage0;
	u64 type_road;	//ssh, tls?
	u64 stage1;
	u64 type_app;	//http2, ws, rdp, vnc?
	u64 stage2;
	u64 type_data;	//html, rgb?
	u64 stage3;

	//[0x40,0x7f]
	u8 addr_src[0x20];
	u8 addr_dst[0x20];

	//[0x80,0xff]
	u8 data[0x80];
};
struct object* obj;
//
static u64 thread;
static int alive = 0;
static int epollfd = 0;
//
static int btlisten = 0;
static int rawlisten = 0;
static int tcplisten = 0;
static int udplisten = 0;




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
void epoll_del(u64 fd)
{
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
	close(fd);
}
void epoll_mod(u64 fd)
{
	struct epoll_event ev;

	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}
void epoll_add(u64 fd)
{
	int flag;
	struct epoll_event ev;
	flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);

	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}
static void* epollthread(void* p)
{
int i, fd, ret;
struct epoll_event epollevent[16];

while(alive)
{
	epoll_mod(tcplisten);
	ret = epoll_wait(epollfd, epollevent, 16, -1);	//start fetch
	if(ret <= 0)continue;

	//printf("epoll:%d\n",ret);
	for(i=0; i<ret; i++)
	{
	if(epollevent[i].events & EPOLLRDHUP)
	{
		printf("rdhup!!!!!!!\n");
		eventwrite(0, 0x2d6e, fd, gettime());
	}
	else if(epollevent[i].events & EPOLLIN)
	{
		//ev.fd
		fd = epollevent[i].data.fd;

		//accept
		if(fd == tcplisten)
		{
		struct sockaddr_in cliaddr;
		socklen_t cliaddrlen = sizeof(struct sockaddr_in);
		while(1)
		{
			fd = accept(tcplisten, (struct sockaddr*)&cliaddr, &cliaddrlen);
			if(fd == -1)break;
			if(fd >= MAXSIZE)
			{
				printf("fd>MAXSIZE\n");
				close(fd);
				continue;
			}

			printf("++++ %d\n",fd);
			obj[fd].type_sock = 't';
			obj[fd].type_road = 0;
			epoll_add(fd);
			eventwrite(0, 0x2b6e, fd, gettime());
		}//while
		}//accept

		//read
		else
		{
			//printf("#### %x\n", fd);
			eventwrite(0, 0x406e, fd, gettime());
		}
	}//EPOLLIN
	}//for

	//wait for completion
	//usleep(100000);
}//while

printf("epoll die!!!\n");
return 0;
}








int writesocket(int fd, u8* buf, int off, int len)
{
	u64 type;
	int ret;
	if(fd == 0)return 0;
	if(buf == 0)return 0;

	type = obj[fd].type_sock;
	if( (type == 'U') | (type == 'u') )
	{
		ret = sendto(
			fd, buf, len, 0,
			(void*)obj[fd].addr_src, sizeof(struct sockaddr_in)
		);
		return ret;
	}

	ret = write(fd, buf, len);
	return ret;
}
int readsocket(int fd, u8* buf, int off, int len)
{
	u64 type;
	int ret, cnt=0;
	if(fd == 0)return 0;
	if(buf == 0)return 0;

	type = obj[fd].type_sock;
	if( (type == 'U') | (type == 'u') )
	{
		ret = sizeof(struct sockaddr_in);
		ret = recvfrom(
			fd, buf, len, 0,
			(void*)obj[fd].addr_src, (void*)&ret
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
	obj[x].type_sock = 0;
	obj[x].type_road = 0;
	printf("---- %d %d, %d\n", x, ret, errno);

	//epoll_del(x);
	//if(tcplisten>0)epoll_del(tcplisten);
}
int startsocket(char* addr, int port, int type)
{
	int fd;
	int ret;
	if(type == 'R')		//RAW
	{
		int sockopt;
		struct ifreq ifopts;
		char* ifName = "eth0";

		rawlisten = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
		if(rawlisten == -1)
		{
			printf("error%d@socket:%d\n",errno,rawlisten);
			return 0;
		}

		strncpy(ifopts.ifr_name, ifName, 5);
		ioctl(rawlisten, SIOCGIFFLAGS, &ifopts);
		ifopts.ifr_flags |= IFF_PROMISC;
		ioctl(rawlisten, SIOCSIFFLAGS, &ifopts);

		ret = setsockopt(rawlisten, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt);
		if(ret == -1)
		{
			perror("setsockopt");
			close(rawlisten);
			return 0;
		}

		ret = setsockopt(rawlisten, SOL_SOCKET, SO_BINDTODEVICE, ifName, 5);
		if(ret == -1)
		{
			perror("SO_BINDTODEVICE");
			close(rawlisten);
			return 0;
		}
		epoll_add(rawlisten);

		obj[rawlisten].type_sock = type;
		obj[rawlisten].type_road = 0;
		return rawlisten;
	}
	if(type == 'r')		//raw
	{
		return fd;
	}
	else if(type == 'U')	//UDP
	{
		int ret;
		struct sockaddr_in servaddr;

		//socket
		udplisten = socket(AF_INET, SOCK_DGRAM, 0);
		if (udplisten == -1)
		{
			printf("error@socket\n");
			return 0;
		}
		if(udplisten > MAXSIZE)
		{
			printf("udplisten>4096\n");
			return 0;
		}

		//
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port);
		servaddr.sin_addr.s_addr = htons(INADDR_ANY);

		//
		ret = bind(udplisten, (void*)&servaddr, sizeof(servaddr));
		if(ret == -1)
		{
			printf("error@bind\n");
			close(udplisten);
			return 0;
		}

		obj[udplisten].type_sock = type;
		obj[udplisten].type_road = 0;
		epoll_add(udplisten);
		return udplisten;
	}
	else if(type == 'u')	//udp
	{
		struct sockaddr_in* haha;
		fd = socket(AF_INET, SOCK_DGRAM, 0);
		if(fd == -1)
		{
			printf("error%d@socket:%d\n",errno,fd);
			return 0;
		}

		haha = (void*)obj[fd].addr_src;
		memset(haha, 0, sizeof(struct sockaddr_in));
		haha->sin_family = AF_INET;
		haha->sin_addr.s_addr = inet_addr(addr);
		haha->sin_port = htons(port);

		//
		ret = connect(fd, (void*)haha, sizeof(struct sockaddr_in) );
		if(ret < 0)
		{
			printf("connect error\n");
			return 0;
		}

		//
		obj[fd].type_sock = type;
		obj[fd].type_road = 0;
		epoll_add(fd);
		return fd;
	}
	else if(type == 'T')	//TCP
	{
		int ret;
		struct sockaddr_in servaddr;

		//socket
		tcplisten = socket(AF_INET, SOCK_STREAM, 0);
		if (tcplisten == -1)
		{
			printf("error@socket\n");
			return 0;
		}
		if(tcplisten > MAXSIZE)
		{
			printf("tcplisten>4096\n");
			return 0;
		}

		//setopt
		ret = 1;
		ret = setsockopt(tcplisten, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
		if(ret<0)
		{
			printf("error@setsockopet\n");
			return 0;
		}

		//
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port);
		servaddr.sin_addr.s_addr = htons(INADDR_ANY);

		//
		ret = bind(tcplisten, (void*)&servaddr, sizeof(servaddr));
		if(ret == -1)
		{
			printf("error@bind\n");
			close(tcplisten);
			return 0;
		}

		//
		listen(tcplisten, 5);

		//
		obj[tcplisten].type_sock = type;
		obj[tcplisten].type_road = 0;
		epoll_add(tcplisten);
		return tcplisten;
	}
	else if(type == 't')	//tcp
	{
		//create struct
		struct sockaddr_in* haha;

		//create socket
		fd = socket(AF_INET, SOCK_STREAM, 0);
		if(fd == -1)
		{
			printf("error%d@socket:%d\n",errno,fd);
			return 0;
		}

		//
		haha = (void*)obj[fd].addr_src;
		memset(haha, 0, sizeof(struct sockaddr_in));
		haha->sin_family = AF_INET;
		haha->sin_addr.s_addr = inet_addr(addr);
		haha->sin_port = htons(port);

		//connect
		ret = connect(fd, (void*)haha, sizeof(struct sockaddr_in));
		if(ret < 0)
		{
			printf("connect error\n");
			return 0;
		}

		//
		obj[fd].type_sock = type;
		obj[fd].type_road = 0;
		epoll_add(fd);
		return fd;
	}
	else if(type == 'B')	//BT
	{
		return btlisten;
	}
	else if(type == 'b')	//bt
	{
		return fd;
	}
	else printf("error@type\n");
	return 0;
}
void deletesocket(int num)
{
	alive = 0;
	if(epollfd>0)close(epollfd);
}
void createsocket(void* addr)
{
	struct sigaction sa;
	obj = addr;

	sa.sa_handler=SIG_IGN;
	sigaction(SIGPIPE, &sa, 0);

	epollfd = epoll_create(MAXSIZE);
	if(epollfd <= 0)printf("%d,%d@epoll_create\n", epollfd, errno);

	alive = 1;
	thread = startthread(epollthread, 0);
}
