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

			//ret = cc/4;
			obj[cc/4].type = _Tcp_;
			obj[cc/4].name = 0;
			obj[cc/4].irel0 = obj[cc/4].ireln = 0;
			obj[cc/4].orel0 = obj[cc/4].oreln = 0;
			obj[cc/4].thatfd = fd;
			obj[cc/4].thatobj = &obj[fd/4];
			obj[cc/4].selffd = cc;
			//obj[cc/4].selfobj = &obj[cc/4];

			iocp_add(cc);
			iocp_mod(cc);
		}
		else if(tran == 0)
		{
			//ret = fd/4;
			//printf("[%x]----\n", ret);
			//eventwrite('-', _fd_, ret, 0);

			obj[fd/4].type = 0;
			obj[fd/4].name = 0;

			iocp_del(fd);
		}
		else
		{
			//pio->count = tran;
			//ret = fd/4;
			//printf("[%x]####\n", ret);
			//eventwrite('@', _fd_, ret, 0);

			cc = fd;
			if( (_Tcp_ == obj[fd/4].type) &&
				(0 == obj[fd/4].irel0) &&
				(0 == obj[fd/4].orel0) )
			{
				//TCP = Tcp.parent
				cc = obj[fd/4].thatfd;

				//Tcp = TCP.child
				obj[cc/4].thatfd = fd;
				obj[cc/4].thatobj = &obj[fd/4];
			}

			//say("@kqueuethread: %.4s\n", &obj[cc/4].type);
			relationwrite(&obj[cc/4], _dst_, 0, 0, pio->bufing.buf, tran);
			iocp_mod(fd);
		}
	}
	return 0;
}




void deletewatcher()
{
}
void createwatcher(void* addr)
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
