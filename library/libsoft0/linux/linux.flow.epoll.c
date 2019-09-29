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
int uart_read(int, int, void*, int);
int readsocket(int, void*, void*, int);




struct object* obj;
static void* buf;
static int alive = 0;
static int epollfd = 0;




void epoll_add(u64 fd)
{
	int flag;
	struct epoll_event ev;
	flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);

	ev.events = EPOLLIN | EPOLLET;
	ev.data.ptr = &obj[fd];
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}
void epoll_del(u64 fd)
{
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.ptr = &obj[fd];
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
	close(fd);
}
void epoll_mod(u64 fd)
{
	struct epoll_event ev;

	ev.events = EPOLLIN | EPOLLET;
	ev.data.ptr = &obj[fd];
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}
static void* epollthread(void* p)
{
	int j;
	int fd, cc;
	int ret, cnt;
	struct object* here;
	struct object* child;
	struct object* parent;
	struct epoll_event epollevent[16];

	while(alive)
	{
		ret = epoll_wait(epollfd, epollevent, 16, -1);	//start fetch
		if(ret <= 0)continue;

		//printf("epoll:%d\n",ret);
		for(j=0; j<ret; j++)
		{
			here = events[j].data.ptr;
			fd = here->selffd;

			if(epollevent[j].events & EPOLLRDHUP)
			{
				printf("rdhup!!!!!!!\n");
				eventwrite('-', _fd_, fd, timeread());
			}
			else if(epollevent[j].events & EPOLLIN)
			{
				switch(here->type){
				case _TCP_:{
					while(1)
					{
						struct sockaddr_in haha;
						socklen_t len = sizeof(struct sockaddr_in);

						cc = accept(fd, (struct sockaddr*)&haha, &len);
						if(cc <= 0)break;

						if(cc >= MAXSIZE){
							printf("fd>MAXSIZE\n");
							close(cc);
						}
						else{
							child = &obj[cc];
							child->type = _Tcp_;
							child->name = 0;
							child->selffd = cc;
							child->selfobj = child;
							child->tempfd = fd;
							child->tempobj = here;

							memcpy(child->peer, &haha, 8);
							epoll_add(cc);
						}
						continue;
					}//while

					//reset tcpfd
					epoll_mod(fd);
					break;
				}//TCP
				case _Tcp_:{
					cnt = readsocket(fd, here->peer, buf, BUFFER_SIZE);
					if(cnt >= 0)
					{
						//printmemory(buf, cnt);
						if( (0 == here->irel0) && (0 == here->orel0) )
						{
							//tell parent, its me
							parent = here->tempobj;
							parent->tempfd = fd;
							parent->tempobj = here;

							//parent send
							here = parent;
						}

						//say("@kqueuethread: %.4s\n", &obj[cc].type);
						relationwrite(here, _dst_, 0, 0, buf, cnt);
					}
					if(cnt <= 0)
					{
						epoll_del(fd);
						close(fd);
						here->type = 0;
						continue;
					}
				}//Tcp
				case _uart_:{
					cnt = read(fd, buf, BUFFER_SIZE);
					relationwrite(here, _dst_, 0, 0, buf, cnt);
					break;
				}//uart
				default:{
					cnt = readsocket(fd, here->peer, buf, BUFFER_SIZE);
					if(cnt >= 0)
					{
						printmemory(buf, cnt);
						//say("@kqueuethread: %.4s\n", &obj[cc].type);
						relationwrite(here, _dst_, 0, 0, buf, cnt);
					}
					if(cnt <= 0)
					{
						epoll_del(fd);
						close(fd);
						here->type = 0;
						continue;
					}
					break;
				}//default
				}//switch
			}//EPOLLIN
		}//for

		//wait for completion
		//usleep(100000);
	}//while

	printf("epoll die!!!\n");
	return 0;
}




void freeewatcher()
{
	alive = 0;
}
void initwatcher(void* addr)
{
	obj = addr;
	buf = addr+0x300000;

	epollfd = epoll_create(MAXSIZE);
	if(epollfd <= 0)printf("error@epoll_create: %d,%d\n", epollfd, errno);

	alive = 1;
	threadcreate(epollthread, 0);
}
