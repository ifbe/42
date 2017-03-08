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
	//0x20 = 4 * 8
	u64 type0;	//raw, bt, udp, tcp?
	u64 type1;	//ssh, tls?
	u64 type2;	//https, wss?
	u64 type3;

	//0x20 = 4 * 8
	u64 zero;
	u64 port_src;
	u64 port_xxx;
	u64 port_dst;

	//0xc0 = 3 * 0x40
	u8 addr_src[0x40];
	u8 addr_xxx[0x40];
	u8 addr_dst[0x40];
};
struct object* obj;
static struct epoll_event epollevent[16];
//static int enqueue=0;
//static int dequeue=0;
//
static u64 thread;
static int alive = 0;
static int epollfd = 0;
static int listenfd = 0;




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
epollfd = epoll_create(MAXSIZE);

while(alive)
{
	ret = epoll_wait(epollfd, epollevent, 16, -1);	//start fetch
	if(ret <= 0)continue;

	//printf("epoll:%d\n",ret);
	for(i=0; i<ret; i++)
	{
	if(epollevent[i].events & EPOLLRDHUP)
	{
		printf("rdhup!!!!!!!\n");
		eventwrite(fd, 0x2d6e, 0, gettime());
	}
	else if(epollevent[i].events & EPOLLIN)
	{
		//ev.fd
		fd = epollevent[i].data.fd;

		//accept
		if(fd == listenfd)
		{
		int fd;
		struct sockaddr_in cliaddr;
		socklen_t cliaddrlen = sizeof(struct sockaddr_in);
		while(1)
		{
			fd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddrlen);
			if(fd == -1)break;
			if(fd >= MAXSIZE)
			{
				printf("fd>MAXSIZE\n");
				close(fd);
				continue;
			}

			printf("++++ %d\n",fd);
			obj[fd].type0 = 't';
			obj[fd].type1 = 0;
			epoll_add(fd);
			eventwrite(fd, 0x2b6e, 0, gettime());
		}//while
		}//accept

		//read
		else
		{
			//enqueue++;
			printf("#### %x\n", fd);
			eventwrite(fd, 0x406e, 0, gettime());
		}
	}//EPOLLIN
	}//for

	//wait for completion
	//usleep(100000);
}//while

printf("epoll die!!!\n");
return 0;
}








int writesocket(u64 fd, u8* buf, u64 off, u64 len)
{
/*
	socklen_t serlen = 0;
	struct sockaddr_in server = {0};
	if(st == SOCK_DGRAM)
	{
		ret=sendto(fd, buf, len, 0, (void*)&server, serlen);
		//printf("err:%d@sendto:%d\n",errno,ret);
	}
	else
	{
		ret=write(fd, buf, len);
		//printf("err:%d@write:%d\n",errno,ret);
	}
*/
	int ret;
	if(fd == 0)return 0;
	if(buf == 0)return 0;
	ret = write(fd, buf, len);
	return ret;
}
int readsocket(u64 fd, u8* buf, u64 off, u64 len)
{
/*
	socklen_t serlen = 0;
	struct sockaddr_in server = {0};
	if(st == SOCK_RAW)
	{
		while(alive == 1)
		{
			ret=recvfrom(fd, buf, 0x1000, 0, NULL, NULL);
			printmemory(buf,ret);
		}
	}
	else if(st == SOCK_DGRAM)
	{
		while(alive == 1)
		{
			ret=recvfrom(fd, buf, 0x1000, 0, (void*)&server, (void*)&serlen);
			if(ret<0)break;

			buf[ret] = 0;
			printf("%s", buf);
			fflush(stdout);
		}
	}
	else
	{
		while(alive == 1)
		{
			ret=read(fd, buf, 0x1000);
			if(ret<0)break;

			buf[ret] = 0;
			printf("%s", buf);
			fflush(stdout);
		}
	}
*/
/*
	if(buf == 0)
	{
		epoll_mod(fd);
		return 0;
	}
*/
	int ret, cnt;
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

	if(cnt < len)epoll_mod(fd);
	return cnt;
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
	printf("---- %d %d, %d\n", x, ret, errno);

	//epoll_del(x);
	//if(listenfd>0)epoll_del(listenfd);
}
int startsocket(char* addr, int port, int type)
{
	int fd;
	int ret;
	if(type == 'r')		//raw
	{
		int sockopt;
		struct ifreq ifopts;
		char* ifName = "eth0";

		fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
		if(fd == -1)
		{
			printf("error%d@socket:%d\n",errno,fd);
			return 0;
		}

		strncpy(ifopts.ifr_name, ifName, 5);
		ioctl(fd, SIOCGIFFLAGS, &ifopts);
		ifopts.ifr_flags |= IFF_PROMISC;
		ioctl(fd, SIOCSIFFLAGS, &ifopts);

		ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt);
		if(ret == -1)
		{
			perror("setsockopt");
			close(fd);
			exit(EXIT_FAILURE);
		}

		ret = setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, ifName, 5);
		if(ret == -1)
		{
			perror("SO_BINDTODEVICE");
			close(fd);
			exit(EXIT_FAILURE);
		}
		epoll_add(fd);

		obj[listenfd].type0 = type;
		eventwrite(fd, 0x2b6e, 0, gettime());
		return fd;
	}
	else if(type == 'u')	//udp client
	{
		//create struct
		struct sockaddr_in server = {0};
		socklen_t serlen = 0;
		u32 temp[4];

		memset(&server, 0, sizeof(struct sockaddr_in));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = inet_addr(addr);
		server.sin_port = htons(port);
		serlen = sizeof(struct sockaddr_in);

		//create socket
		fd = socket(AF_INET, SOCK_DGRAM, 0);
		if(fd == -1)
		{
			printf("error%d@socket:%d\n",errno,fd);
			return 0;
		}
		epoll_add(fd);

		obj[listenfd].type0 = type;
		obj[listenfd].type1 = 0;
		obj[listenfd].port_src = port;
		eventwrite(fd, 0x2b6e, 0, gettime());
		return fd;
	}
	else if(type == 't')	//tcp client
	{
		//create struct
		struct sockaddr_in server = {0};
		socklen_t serlen = 0;
		u32 temp[4];

		memset(&server, 0, sizeof(struct sockaddr_in));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = inet_addr(addr);
		server.sin_port = htons(port);
		serlen = sizeof(struct sockaddr_in);

		//create socket
		fd = socket(AF_INET, SOCK_STREAM, 0);
		if(fd == -1)
		{
			printf("error%d@socket:%d\n",errno,fd);
			return 0;
		}

		//connect
		ret = connect(fd, (struct sockaddr*)&server, sizeof(server));
		if(ret < 0)
		{
			printf("connect error\n");
			return 0;
		}
		epoll_add(fd);

		//
		obj[listenfd].type0 = type;
		obj[listenfd].type1 = 0;
		obj[listenfd].port_src = port;
		eventwrite(fd, 0x2b6e, 0, gettime());
		return fd;
	}
	else if(type == 'b')	//bluetooth client
	{
	}
	else if(type == 'U')	//udp server
	{
	}
	else if(type == 'T')	//tcp server
	{
		int i;
		int fd;
		int ret;
		struct sockaddr_in servaddr;
		char IPADDRESS[32];
		snprintf(IPADDRESS, 32, "0.0.0.0");

		//socket
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		inet_pton(AF_INET, IPADDRESS, &servaddr.sin_addr);
		servaddr.sin_port = htons(port);

		listenfd = socket(AF_INET, SOCK_STREAM, 0);
		if (listenfd == -1)
		{
			printf("error@socket\n");
			return 0;
		}
		if(listenfd > MAXSIZE)
		{
			printf("listenfd>4096\n");
			return 0;
		}

		//setopt
		ret = 1;
		ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
		if(ret<0)
		{
			printf("error@setsockopet\n");
			return 0;
		}

		if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
		{
			printf("error@bind\n");
			return 0;
		}
		listen(listenfd, 5);
		epoll_add(listenfd);	

		//
		obj[listenfd].type0 = type;
		obj[listenfd].port_src = port;
		return listenfd;
	}
	else if(type == 'B')	//bluetooth server
	{
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

	alive = 1;
	thread = startthread(epollthread, 0);
}
