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
u64 gettime();
u64 startthread(void*, void*);
void stopthread();
//
void eventwrite(u64,u64,u64,u64);
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
	u8 self[0x20];
	u8 peer[0x20];

	//[0x80,0xff]
	u8 data[0x80];
};
struct object* obj;
//
static u64 thread;
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
	int i, ret;
	int fd, cc;
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
				eventwrite(0, 0x2d6e, fd, gettime());
			}
			else if(epollevent[i].events & EPOLLIN)
			{
				//accept
				if(obj[fd].type_sock == 'T')
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
						obj[cc].type_sock = 't';
						obj[cc].type_road = 0;
						epoll_add(cc);

						printf("++++ %d\n",cc);
						eventwrite(0, 0x2b6e, cc, gettime());
					}//while

					//reset tcpfd
					epoll_mod(fd);
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




void deletewatcher(int num)
{
	alive = 0;
}
void createwatcher(void* addr)
{
	obj = addr;

	epollfd = epoll_create(MAXSIZE);
	if(epollfd <= 0)printf("%d,%d@epoll_create\n", epollfd, errno);

	alive = 1;
	thread = startthread(epollthread, 0);
}
