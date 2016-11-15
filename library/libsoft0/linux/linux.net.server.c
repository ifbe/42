#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/types.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void eventwrite(u64,u64,u64,u64);
u64 gettime();
//
void printmemory(char*,int);
void say(char*,...);




//
#define MAXSIZE 1024
static struct epoll_event epollevent[MAXSIZE];
static int waiting[256];
static int enq = 0;
static int deq = 0;
//
static pthread_t id;
static int alive = 0;
//
static int listenfd=-1;
static int epollfd=-1;
//
static char IPADDRESS[32];
static int PORT;
static char DIRECTORY[64];
static int CONFIG;




void epoll_del(u32 fd)
{
	int j;
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);

	//
	close(fd);
	eventwrite(0, 0x2d6e, fd, 0);
}
void epoll_add(u32 fd)
{
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}
static void handle_accpet(int listenfd)
{
	int fd;
	struct sockaddr_in cliaddr;

	socklen_t cliaddrlen = sizeof(struct sockaddr_in);
	fd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddrlen);
	if (fd == -1)
	{
		say("accept error\n");
		return;
	}

	//
	snprintf(IPADDRESS, 32,
		"%s:%d",
		inet_ntoa(cliaddr.sin_addr),
		cliaddr.sin_port
	);
	eventwrite((u64)IPADDRESS, 0x2b6e, fd, 0);
	epoll_add(fd);
}
static void* newone(void* p)
{
	int i;
	int fd;
	int ret;
	struct sockaddr_in servaddr;
	snprintf(IPADDRESS, 32, "0.0.0.0");

	//socket
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, IPADDRESS, &servaddr.sin_addr);
	servaddr.sin_port = htons(PORT);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1)
	{
		say("error@socket\n");
		return 0;
	}

	//setopt
	ret = 1;
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
	if(ret<0)
	{
		say("error@setsockopet\n");
		return 0;
	}

	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
		say("error@bind\n");
		return 0;
	}
	listen(listenfd, 5);

	//epoll
	epollfd = epoll_create(MAXSIZE);
	epoll_add(listenfd);

	//
	enq = deq = 0;
	while(alive)
	{
		if(enq != deq)
		{
			usleep(10*1000);
			continue;
		}

		ret = epoll_wait(epollfd, epollevent, MAXSIZE, -1);
		for (i=0; i<ret; i++)
		{
			if (epollevent[i].events & EPOLLIN)
			{
				fd = epollevent[i].data.fd;

				if(fd == listenfd)
				{
					handle_accpet(listenfd);
				}
				else
				{
					waiting[enq] = fd;
					enq = (enq+1)%256;

					eventwrite(0, 0x406e, fd, gettime());
					//handle_read(fd);
				}
			}//EPOLLIN
		}//for
	}//while
}








int writeserver(u64 fd, u8* buf, u64 offset, u64 count)
{
	return write(fd, buf, count);
}
int readserver(u64 fd, u8* buf, u64 offset, u64 count)
{
	if(fd == waiting[deq])deq = (deq+1)%256;
	return read(fd, buf, count);
}
int listserver()
{
}
int chooseserver()
{
}
void stopserver()
{
	int j;
	alive = 0;

	if(listenfd>0)epoll_del(listenfd);
	if(epollfd>0)close(epollfd);
}
void startserver(char* addr, int port, char* dir, int opt)
{
	//snprintf();
	PORT = port;
	snprintf(DIRECTORY, 64, "%s", dir);

	//
	alive = 1;
	pthread_create(&id, NULL, newone, NULL);
}
void deleteserver(int num)
{
}
int createserver()
{
	struct sigaction sa;
	sa.sa_handler=SIG_IGN;
	sigaction(SIGPIPE,&sa,0);
}
