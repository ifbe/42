#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "libsoft.h"
#define MAXSIZE 4096
#define BUFFER_SIZE 0x100000
int readsocket(int, void*, void*, int);




union addrv4v6{
	struct sockaddr sa;		//just make compiler happy
	struct sockaddr_in v4;
	struct sockaddr_in6 v6;
};
static struct sysobj* obj;
static void* buf;
static int kqfd = 0;
static int alive = 0;




int kqueue_add(int fd)
{
	struct kevent ke;
	EV_SET(&ke, fd, EVFILT_READ, EV_ADD, 0, 0, &obj[fd]);
	kevent(kqfd, &ke, 1, NULL, 0, NULL);
	return 0;
}
int kqueue_del(int fd)
{
	struct kevent ke;
	EV_SET(&ke, fd, EVFILT_READ, EV_DELETE, 0, 0, &obj[fd]);
	kevent(kqfd, &ke, 1, NULL, 0, NULL);
	return 0;
}
int kqueue_mod(int fd)
{
	struct kevent ke;
	return 0;
}
int kqueuethread(int argc, const char * argv[])
{
	int ret,cnt;
	struct halfrel stack[0x80];
	struct kevent events[10];
	struct timespec timeout = {10,0};

	int fd,cc;
	struct sysobj* here;
	struct sysobj* child;
	struct sysobj* parent;

	kqfd = kqueue();
	if(kqfd <= 0)
	{
		perror("error@kqueue()\n");
		exit(1);
	}

	while (1)
	{
		int ret = kevent(kqfd, NULL, 0, events, 10, &timeout);
		if (ret < 0)
		{
			printf("kevent error\n");
			continue;
		}
		if(ret == 0)
		{
			//printf("kenvent timeout\n");
			continue;
		}

		for (int j=0;j<ret;j++)
		{
			here = events[j].udata;
			fd = events[j].ident;
			//say("here=%llx,fd=%x\n", here, fd);

			switch(here->type){
			case _ptmx_:
			case _uart_:{
				cnt = read(fd, buf, BUFFER_SIZE);
				if(0 == cnt){
					say("error@%.4s:len=%x\n", &here->type, cnt);
					systemdelete(here);
					break;
				}
				give_data_into_peer(here,_dst_, stack,0, 0,0, buf,cnt);
				break;
			}//easy

			case _udp_:
			case _UDP_:
			case _tcp_:{
				cnt = readsocket(fd, here->peer, buf, BUFFER_SIZE);
				if(cnt >= 0)
				{
					//say("@kqueuethread: %.4s\n", &obj[cc].type);
					if((0==here->irel0)&&(0==here->orel0))printmemory(buf, cnt);
					else give_data_into_peer(here,_dst_, stack,0, here->peer,0, buf,cnt);
				}
				if(cnt <= 0)
				{
					kqueue_del(fd);
					systemdelete(here);
					//close(fd);
					//here->type = 0;
				}
				break;
			}//tcp

			case _Tcp_:{
				cnt = readsocket(fd, here->peer, buf, BUFFER_SIZE);
				if(cnt >= 0)
				{
					//printmemory(buf, cnt);
					if( (0 == here->irel0) && (0 == here->orel0) )
					{
						parent = here->tempobj;
						memcpy(parent->peer, here->peer, 8);

						//tell parent, its me
						parent->tempfd = fd;
						parent->tempobj = here;

						//parent send
						here = parent;
					}

					//say("@kqueuethread: %.4s\n", &here->type);
					give_data_into_peer(here,_dst_, stack,0, 0,0, buf,cnt);
				}
				if(cnt <= 0)
				{
					kqueue_del(fd);
					systemdelete(here);
					//close(fd);
					//here->type = 0;
				}
				break;
			}//Tcp

			case _TCP_:{
				socklen_t len = sizeof(union addrv4v6);
				int cc = accept(fd, (struct sockaddr *)(here->peer), &len);
				if(cc <= 0)break;

				if(cc >= MAXSIZE){
					printf("fd>MAXSIZE\n");
					close(cc);
					break;
				}
				else{
					child = &obj[cc];
					child->type = _Tcp_;
					child->name = 0;
					child->selffd = cc;
					child->selfobj = child;
					child->tempfd = fd;
					child->tempobj = here;
					say("accept:listen=%d,incoming=%d\n", fd, cc);
					printmemory(here->peer, sizeof(union addrv4v6));
					memcpy(child->peer, here->peer, 32);
					kqueue_add(cc);
					//eventwrite('+', _fd_, cc, timeread());
				}
				break;
			}//TCP

			default:{
				break;
			}
			}//switch
		}//for
	}//while1
	return 0;
}




void freewatcher()
{
	alive = 0;
}
void initwatcher(void* addr)
{
	obj = addr;
	buf = addr+0x100000;

	alive = 1;
	threadcreate(kqueuethread, 0);
}
