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
#define BUFFER_SIZE 1024




static struct object* obj;
static void* buf;
static int kqfd = 0;
static int alive = 0;




int kqueue_add(int fd)
{
	struct kevent ke;
	EV_SET(&ke, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	kevent(kqfd, &ke, 1, NULL, 0, NULL);
	return 0;
}
int kqueue_del(int fd)
{
	struct kevent ke;
	EV_SET(&ke, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
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
	int fd,cc;
	int ret,cnt;
	struct kevent events[10];
	struct timespec timeout = {10,0};

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
			struct kevent current_event = events[j];
			fd = current_event.ident;
			if(_TCP_ == obj[fd].type)
			{
				socklen_t size;
				struct sockaddr_in addr;
				int cc = accept(fd, (struct sockaddr *)&addr, &size);
				if(cc > 0)
				{
					obj[cc].type = _Tcp_;
					obj[cc].name = 0;
					obj[cc].selffd = cc;
					obj[cc].thatfd = fd;
					kqueue_add(cc);
					eventwrite('+', _fd_, cc, timeread());
				}
				continue;
			}

			cnt = readsocket(fd, obj[fd].peer, buf, BUFFER_SIZE);
			if(cnt <= 0)
			{
				kqueue_del(fd);
				close(fd);
				obj[fd].type = 0;
				obj[fd].name = 0;
				obj[fd].selffd = 0;
				obj[fd].thatfd = 0;
				continue;
			}

			systemwrite(&obj[fd], obj[fd].peer, 0, 0, buf, cnt);
		}//for
	}//while1
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

	alive = 1;
	threadcreate(kqueuethread, 0);
}
