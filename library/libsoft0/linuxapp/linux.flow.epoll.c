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
int socket_take(int, void*, void*, int);




union addrv4v6{
	struct sockaddr sa;		//just make compiler happy
	struct sockaddr_in v4;
	struct sockaddr_in6 v6;
};
static _obj* obj;
static void* tmpbuf;
static int alive = 0;
static int epollfd = 0;




void epoll_add(u64 fd)
{
	int flag;
	struct epoll_event ev;
	flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);

	ev.events = EPOLLIN | EPOLLET;
	ev.data.u64 = 0;
	ev.data.ptr = &obj[fd];
	//printf("%x,%x\n", &obj[fd], ev.data.ptr);
	//printf("%llx,%llx\n", &obj[fd], ev.data.ptr);
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}
void epoll_del(u64 fd)
{
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.u64 = 0;
	ev.data.ptr = &obj[fd];
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
	close(fd);
}
void epoll_mod(u64 fd)
{
	struct epoll_event ev;

	ev.events = EPOLLIN | EPOLLET;
	ev.data.u64 = 0;
	ev.data.ptr = &obj[fd];
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}
static void* epollthread(void* p)
{
	int j;
	int ret, cnt;
	struct epoll_event ev[16];
	struct halfrel stack[0x80];

	int fd, cc;
	_obj* here;
	_obj* child;
	_obj* parent;

	while(alive)
	{
		ret = epoll_wait(epollfd, ev, 16, -1);
		if(ret <= 0)continue;

		//printf("epoll:%d\n",ret);
		for(j=0; j<ret; j++)
		{
			here = ev[j].data.ptr;
			fd = here->sockinfo.fd;
			//printmemory(&ev[j], sizeof(struct epoll_event));
			//say("fd=%x, here=%llx\n", fd, here);
			//printf("fd=%x, here=%llx\n", fd, here);
			//printf("fd=%x, here=%x\n", fd, here);

			if(ev[j].events & EPOLLRDHUP)
			{
				printf("rdhup!!!!!!!\n");
				eventwrite('-', _sys_, fd, timeread_us());
			}
			else if(ev[j].events & EPOLLIN)
			{
				switch(here->type){
				case _ptmx_:
				case _uart_:{
					cnt = read(fd, tmpbuf, BUFFER_SIZE);
					if(0 == cnt){
						say("error@%.4s:len=%x\n", &here->type, cnt);
						system_delete(here);
						break;
					}
					give_data_into_peer(here,_dst_, stack,0, 0,0, tmpbuf,cnt);
					break;
				}//easy

				case _udp_:
				case _UDP_:
				case _tcp_:{
					cnt = socket_take(fd, here->sockinfo.peer, tmpbuf, BUFFER_SIZE);
					if(cnt >= 0)
					{
						//say("@epollthread: %.4s\n", &obj[cc].type);
						if((0==here->irel0)&&(0==here->orel0))printmemory(tmpbuf, cnt);
						else give_data_into_peer(here,_dst_, stack,0, here->sockinfo.peer,0, tmpbuf,cnt);
					}
					if(cnt <= 0)
					{
						epoll_del(fd);
						system_delete(here);
						//close(fd);
						//here->type = 0;
						continue;
					}
					break;
				}//tcp

				case _Tcp_:{
					cnt = socket_take(fd, here->sockinfo.peer, tmpbuf, BUFFER_SIZE);
					if(cnt >= 0)
					{
						//printmemory(tmpbuf, cnt);
						if( (0 == here->irel0) && (0 == here->orel0) )
						{
							parent = here->sockinfo.parent;
							memcpy(parent->sockinfo.peer, here->sockinfo.peer, 8);
							//parent->sockinfo.parent = 0;
							parent->sockinfo.child = here;

							//parent send
							here = parent;
						}

						//say("@kqueuethread: %.4s\n", &obj[cc].type);
						give_data_into_peer(here,_dst_, stack,0, 0,0, tmpbuf,cnt);
					}
					if(cnt <= 0)
					{
						epoll_del(fd);
						system_delete(here);
						//close(fd);
						//here->type = 0;
						continue;
					}
					break;
				}//Tcp

				case _TCP_:{
					while(1)
					{
						socklen_t len = sizeof(union addrv4v6);
						cc = accept(fd, (struct sockaddr*)(here->sockinfo.peer), &len);
						printf("cc=%x,errno=%d\n",cc,errno);
						if(cc <= 0)break;

						if(cc >= MAXSIZE){
							printf("fd>MAXSIZE\n");
							close(cc);
						}
						else{
							//printf("fd=%x,cc=%x,here=%llx,child=%llx\n",fd,cc,here,child);

							child = &obj[cc];
							child->type = _Tcp_;
							memcpy(child->sockinfo.peer, here->sockinfo.peer, 32);
							child->sockinfo.parent = here;
							child->sockinfo.child = 0;
							child->sockinfo.fd = cc;

							//here->sockinfo.parent = 0;
							here->sockinfo.child = child;
							epoll_add(cc);
						}
						continue;
					}//while

					//reset tcpfd
					epoll_mod(fd);
					break;
				}//TCP

				default:{
					break;
				}
				}//switch
			}//EPOLLIN
		}//for

		//wait for completion
		//usleep(100000);
	}//while

	printf("epoll die!!!\n");
	return 0;
}




void freewatcher()
{
	alive = 0;
}
void initwatcher(void* addr)
{
	obj = addr;
	tmpbuf = addr + 0x100000;

	epollfd = epoll_create(MAXSIZE);
	if(epollfd <= 0)printf("error@epoll_create: %d,%d\n", epollfd, errno);

	alive = 1;
	threadcreate(epollthread, 0);
}
