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
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "libsoft.h"
#define MAXSIZE 4096
#define BUFFER_SIZE 0x100000
int readsocket(int, void*, void*, int);




struct object* obj;
static void* buf;
static int alive = 0;
static int epollfd = 0;




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
	int i;
	int fd, cc;
	int ret, cnt;
	struct epoll_event epollevent[16];

	while(alive)
	{
		ret = epoll_wait(epollfd, epollevent, 16, -1);	//start fetch
		if(ret <= 0)continue;

		//printf("epoll:%d\n",ret);
		for(i=0; i<ret; i++)
		{
			fd = epollevent[i].data.fd;

			if(epollevent[i].events & EPOLLRDHUP)
			{
				printf("rdhup!!!!!!!\n");
				eventwrite('-', _fd_, fd, timeread());
			}
			else if(epollevent[i].events & EPOLLIN)
			{
				//accept
				if(_TCP_ == obj[fd].type)
				{
					while(1)
					{
						struct sockaddr_in haha;
						socklen_t len = sizeof(struct sockaddr_in);

						cc = accept(fd, (struct sockaddr*)&haha, &len);
						if(cc == -1)break;
						if(cc >= MAXSIZE)
						{
							printf("fd>MAXSIZE\n");
							close(cc);
							continue;
						}

						memcpy(obj[cc].peer, &haha, 8);
						obj[cc].type = _Tcp_;
						obj[cc].name = 0;
						obj[cc].irel0 = obj[cc].ireln = 0;
						obj[cc].orel0 = obj[cc].oreln = 0;
						obj[cc].selffd = cc;
						obj[cc].thatfd = fd;
						epoll_add(cc);

						printf("++++ %d\n",cc);
						eventwrite('+', _fd_, cc, timeread());
					}//while

					//reset tcpfd
					epoll_mod(fd);
				}//accept

				//read
				else
				{
					//printf("#### %x\n", fd);
					//eventwrite('@', _fd_, fd, timeread());
					cnt = readsocket(fd, obj[fd].peer, buf, BUFFER_SIZE);
					if(cnt >= 0)
					{
						//printmemory(buf, cnt);
						cc = fd;
						if( (_Tcp_ == obj[fd].type) &&
							(0 == obj[fd].irel0) &&
							(0 == obj[fd].orel0) )
						{
							//TCP = Tcp.parent
							cc = obj[fd].thatfd;

							//Tcp = TCP.child
							obj[cc].thatfd = fd;
						}

						//say("@kqueuethread: %.4s\n", &obj[cc].type);
						relationwrite(&obj[cc], _dst_, 0, 0, buf, cnt);
					}
					if(cnt <= 0)
					{
						epoll_del(fd);
						close(fd);
						obj[fd].type = 0;
						obj[fd].name = 0;
						obj[fd].selffd = 0;
						obj[fd].thatfd = 0;
						continue;
					}
				}
			}//EPOLLIN
		}//for

		//wait for completion
		//usleep(100000);
	}//while

	printf("epoll die!!!\n");
	return 0;
}




void deletewatcher(int num)
{
	alive = 0;
}
void createwatcher(void* addr)
{
	obj = addr;
	buf = addr+0x300000;

	epollfd = epoll_create(MAXSIZE);
	if(epollfd <= 0)printf("error@epoll_create: %d,%d\n", epollfd, errno);

	alive = 1;
	threadcreate(epollthread, 0);
}
