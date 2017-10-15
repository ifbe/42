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
void stopsocket(SOCKET);
void startsocket();
void eventwrite(u64,u64,u64,u64);




struct per_io_data
{
	OVERLAPPED overlap;
	SOCKET fd;
	int stage;
	WSABUF bufing;
	WSABUF bufdone;
};
static struct object* obj;
static HANDLE iocpfd;




DWORD WINAPI iocpthread(LPVOID pM)
{
	struct per_io_data* pov = NULL;
	u32* key = 0;
	char temp[4096];
	int th;
	int ret;

	HANDLE hh;
	SOCKET fd;
	DWORD trans = 0;
	DWORD flag = 0;

	th = GetCurrentThreadId();
	while(1)
	{
		ret = GetQueuedCompletionStatus(
			iocpfd,
			&trans,
			(void*)&key,
			(void*)&pov,
			INFINITE
		);
		if(ret == 0)continue;

		fd = pov->fd;
		printf("th=%d,ret=%d,trans=%d,listen=%d,this=%d\n",
			th, ret, trans, *key, fd
		);

		//accept
		if(pov->stage == 0)
		{
			hh = CreateIoCompletionPort(
				(void*)fd,
				iocpfd,
				(ULONG_PTR)(obj[fd/4].self),
				0
			);

			pov->stage = 1;
			obj[fd/4].type_sock = 't';
			eventwrite(0, 0x2b6e, fd/4, 0);
			printf("[%x]++++,hh=%llx\n",fd/4,hh);
		}
		else if(trans == 0)
		{
			printf("[%x]----\n",fd/4);
			stopsocket(fd);
			continue;
		}
		else
		{
			pov->stage = 1;
			pov->bufdone.buf = pov->bufing.buf;
			pov->bufdone.len = trans;
			eventwrite(0, 0x406e, fd/4, 0);
			
			printf("[%x]####\n",fd/4);
		}

		//all
		pov->bufing.buf = malloc(4096);
		pov->bufing.len = 4096;
		ret = WSARecv(fd, &(pov->bufing), 1, &trans, &flag, (void*)pov, NULL);
		printf("(recv)ret=%d,err=%d\n", ret, WSAGetLastError());
	}
	return 0;
}
void stopwatcher()
{
}
void startwatcher(SOCKET handle)
{
	u32* p = (void*)(obj[handle/4].self);
	*p = handle;
	CreateIoCompletionPort(
		(void*)handle,
		iocpfd,
		(ULONG_PTR)p,
		0
	);
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