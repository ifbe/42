#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>	//IP_HDRINCL
#include <ws2bth.h>		//BTHPROTO_RFCOMM
#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include "libsoft.h"




struct per_io_data
{
	OVERLAPPED overlap;
	WSABUF bufing;
	int count;
	int stage;
	SOCKET fd;
};
static struct object* obj;
static HANDLE iocpfd;




void iocp_add(SOCKET fd)
{
	u32* pfd = (void*)(obj[fd/4].self);
	struct per_io_data* pio = (void*)(obj[fd/4].data);

	*pfd = fd;
	CreateIoCompletionPort(
		(void*)fd,
		iocpfd,
		(ULONG_PTR)pfd,
		0
	);
	if(_TCP_ == obj[fd/4].type)return;

	pio->stage = 1;
	pio->bufing.buf = malloc(4096);
	pio->bufing.len = 4096;
}
void iocp_del(SOCKET fd)
{
	struct per_io_data* pio = (void*)(obj[fd/4].data);
	if(pio->bufing.buf)
	{
		free(pio->bufing.buf);
		pio->bufing.buf = 0;
	}
}
void iocp_mod(SOCKET fd)
{
	int ret;
	DWORD tran = 0;
	DWORD flag = 0;
	struct per_io_data* pio;
	void* peer;

	ret = obj[fd/4].type;
	pio = (void*)(obj[fd/4].data);
	peer = (void*)(obj[fd/4].peer);

	if((_UDP_ == ret) | (_udp_ == ret))
	{
		ret = sizeof(struct sockaddr_in);
		ret = WSARecvFrom(fd,
			&(pio->bufing), 1,
			&tran, &flag,
			peer, &ret,
			(void*)pio, NULL
		);
	}
	else
	{
		ret = WSARecv(fd,
			&(pio->bufing), 1,
			&tran, &flag,
			(void*)pio, NULL
		);
	}
}
DWORD WINAPI iocpthread(LPVOID pM)
{
	u32* pfd = 0;
	struct per_io_data* pio = NULL;
	int tmp, ret;
	SOCKET fd;
	SOCKET cc;
	DWORD tran = 0;
	DWORD flag = 0;
	struct object* here;
	struct object* parent;
	struct object* child;

	tmp = GetCurrentThreadId();
	while(1)
	{
		ret = GetQueuedCompletionStatus(
			iocpfd,
			&tran,
			(void*)&pfd,
			(void*)&pio,
			INFINITE
		);
		if(ret == 0)continue;

		fd = *pfd;
		cc = pio->fd;
		//printf("tmp=%x,tran=%x,listen=%x,this=%x\n", tmp, tran, fd, cc);

		//accept
		if(pio->stage == 0)
		{
			//ret = cc/4;
			//printf("[%x,%x]++++\n", tmp, ret);
			//eventwrite('+', _fd_, ret, 0);
			printf("parent=%x, child=%x\n", fd, cc);

			parent = &obj[fd/4];
			child = &obj[cc/4];

			child->type = _Tcp_;
			child->name = 0;
			child->irel0 = child->ireln = 0;
			child->orel0 = child->oreln = 0;
			child->selffd = cc/4;
			child->selfobj = child;
			child->tempfd = fd/4;
			child->tempobj = parent;

			//todo: getpeername

			iocp_add(cc);
			iocp_mod(cc);
		}
		else if(tran == 0)
		{
			//ret = fd/4;
			//printf("[%x]----\n", ret);
			//eventwrite('-', _fd_, ret, 0);

			here = &obj[fd/4];
			iocp_del(fd);
			systemdelete(here);
		}
		else
		{
			//pio->count = tran;
			//ret = fd/4;
			//printf("[%x]####\n", ret);
			//eventwrite('@', _fd_, ret, 0);

			here = &obj[fd/4];
			switch(here->type){
			case _Tcp_:{
				if(	(0 == here->irel0) && (0 == here->orel0) ){
					parent = here->tempobj;
					memcpy(parent->peer, here->peer, 8);

					//tell parent, its me
					parent->tempfd = fd;
					parent->tempobj = here;

					//parent send
					here = parent;
				}

				relationwrite(here, _dst_, 0, 0, pio->bufing.buf, tran);
				iocp_mod(fd);
				break;
			}//Tcp
			default:{
				relationwrite(here, _dst_, 0, 0, pio->bufing.buf, tran);
				iocp_mod(fd);
			}//default
			}//switch
		}//else
	}//while
	return 0;
}




void freewatcher()
{
}
void initwatcher(void* addr)
{
	obj = addr;

	//iocp
	iocpfd = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL,
		0,
		4
	);

	int j;
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	for(j=0;j<info.dwNumberOfProcessors*2;j++)
	{
		u64 thread = threadcreate(iocpthread, iocpfd);
	}
}
