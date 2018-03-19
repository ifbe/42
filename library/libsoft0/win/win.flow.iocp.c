#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ws2tcpip.h>	//IP_HDRINCL
#include<ws2bth.h>		//BTHPROTO_RFCOMM
#include<winsock2.h>
#include<mswsock.h>
#include<windows.h>
#include"system.h"
u64 startthread(void*, void*);
void eventwrite(u64,u64,u64,u64);




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
	if('T' == obj[fd/4].type)return;

	pio->stage = 1;
	pio->bufing.buf = malloc(4096);
	pio->bufing.len = 4096;
}
void iocp_del(SOCKET fd)
{
	struct per_io_data* pio = (void*)(obj[fd/4].data);
	if(pio->bufing.buf)free(pio->bufing.buf);
}
void iocp_mod(SOCKET fd)
{
	int ret;
	DWORD tran = 0;
	DWORD flag = 0;
	struct per_io_data* pio = (void*)(obj[fd/4].data);
	ret = WSARecv(fd, &(pio->bufing), 1, &tran, &flag, (void*)pio, NULL);
}
DWORD WINAPI iocpthread(LPVOID pM)
{
	u32* pfd = 0;
	struct per_io_data* pio = NULL;
	int th;
	int ret;
	SOCKET fd;
	SOCKET cc;
	DWORD tran = 0;
	DWORD flag = 0;

	th = GetCurrentThreadId();
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
		//printf("th=%x,tran=%x,listen=%x,this=%x\n", th, tran, fd, cc);

		//accept
		if(pio->stage == 0)
		{
			printf("[%x,%x]++++\n", fd, cc);
			eventwrite('+', __fd__, cc/4, 0);

			obj[cc/4].type = 't';
			obj[cc/4].name = 0;

			iocp_add(cc);
			iocp_mod(cc);
		}
		else if(tran == 0)
		{
			printf("[%x]----\n",fd);
			eventwrite('-', __fd__, fd/4, 0);

			obj[cc/4].type = 0;
			obj[cc/4].name = 0;

			iocp_del(fd);
		}
		else
		{
			printf("[%x]####\n",fd);
			pio->count = tran;
			eventwrite('@', __fd__, fd/4, 0);

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
		u64 thread = startthread(iocpthread, iocpfd);
	}
}
