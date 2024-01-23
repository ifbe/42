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
int socket_reader(void*,int, p64,int, void*,int);




union addrv4v6{
	struct sockaddr sa;		//just make compiler happy
	struct sockaddr_in v4;
	struct sockaddr_in6 v6;
};
static struct item* obj;
static void* tmpbuf;
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
int kqueue_thread(int argc, const char * argv[])
{
	int ret,cnt;
	struct halfrel stack[0x80];
	struct kevent events[10];
	struct timespec timeout = {10,0};

	int fd,cc;
	struct item* here;
	struct item* child;
	struct item* parent;

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
			//logtoall("here=%llx,fd=%x\n", here, fd);

			switch(here->type){
			case _ptmx_:
			case _uart_:{
				cnt = read(fd, tmpbuf, BUFFER_SIZE);
				if(0 == cnt){
					logtoall("error@%.4s:len=%x\n", &here->type, cnt);
					system_delete(here);
					break;
				}
				give_data_into_peer(here,_dst_, stack,0, 0,0, tmpbuf,cnt);
				break;
			}//easy

			case _udp_:
			case _UDP_:
			case _tcp_:{
				union addrv4v6 addr;
				cnt = socket_reader(here,0, (p64)&addr,0, tmpbuf,BUFFER_SIZE);
				if(cnt >= 0)
				{
					//logtoall("@kqueuethread: %.4s\n", &obj[cc].type);
					if((0==here->irel0)&&(0==here->orel0))printmemory(tmpbuf, cnt);
					else give_data_into_peer(here,_dst_, stack,0, (p64)&addr,0, tmpbuf,cnt);
				}
				if(cnt <= 0)
				{
					kqueue_del(fd);
					system_delete(here);
					//close(fd);
					//here->type = 0;
				}
				break;
			}//tcp

			case _Tcp_:{
				cnt = socket_reader(here,0, (p64)here->sockinfo.peer,0, tmpbuf, BUFFER_SIZE);
				if(cnt >= 0)
				{
					//printmemory(buf, cnt);
					if( (0 == here->irel0) && (0 == here->orel0) )
					{
						parent = here->sockinfo.parent;
						memcpy(parent->sockinfo.peer, here->sockinfo.peer, 8);
						//parent->sockinfo.parent = 0;
						parent->sockinfo.child = here;

						//parent send
						here = parent;
					}

					//logtoall("@kqueuethread: %.4s\n", &here->type);
					give_data_into_peer(here,_dst_, stack,0, 0,0, tmpbuf,cnt);
				}
				if(cnt <= 0)
				{
					kqueue_del(fd);
					system_delete(here);
					//close(fd);
					//here->type = 0;
				}
				break;
			}//Tcp

			case _TCP_:{
				socklen_t len = sizeof(union addrv4v6);
				int cc = accept(fd, (struct sockaddr *)(here->sockinfo.peer), &len);
				if(cc <= 0)break;

				if(cc >= MAXSIZE){
					printf("fd>MAXSIZE\n");
					close(cc);
					break;
				}
				else{
					child = &obj[cc];
					printf("accept: fd=%x,cc=%x,here=%p,child=%p\n",fd,cc,here,child);

					child->type = _Tcp_;
					child->sockinfo.parent = here;
					child->sockinfo.child = 0;
					child->sockinfo.fd = cc;

					union addrv4v6* peeraddr = (void*)child->sockinfo.peer;
					memcpy(peeraddr, here->sockinfo.peer, 32);

					u8* pp = (u8*)&peeraddr->v4.sin_addr;
					printf("accept: port=%d,addr=%d.%d.%d.%d\n", peeraddr->v4.sin_port, pp[0],pp[1],pp[2],pp[3]);

					//here->sockinfo.parent = 0;
					here->sockinfo.child = child;
					kqueue_add(cc);
					//eventwrite('+', _fd_, cc, timeread_us());
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
	tmpbuf = addr+0x100000;

	alive = 1;
	threadcreate(kqueue_thread, 0);
}
