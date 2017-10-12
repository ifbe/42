#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ws2tcpip.h>	//IP_HDRINCL
#include<ws2bth.h>		//BTHPROTO_RFCOMM
#include<winsock2.h>
#include<mswsock.h>
#include<windows.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void stopsocket(SOCKET);
void startsocket();
u64 startthread(void*, void*);
//
void eventwrite(u64,u64,u64,u64);
void printmemory(void*, ...);
void say(void* , ...);




struct per_io_data
{
	OVERLAPPED overlap;
	SOCKET fd;
	int stage;
	WSABUF bufing;
	WSABUF bufdone;
};
struct object
{
	//[0x00,0x0f]
	u64 type_sock;  //raw, bt, udp, tcp?
	u64 stage0;
	u64 type_road;  //ssh, tls?
	u64 stage1;
	u64 type_app;   //http2, ws, rdp, vnc?
	u64 stage2;
	u64 type_data;  //html, rgb?
	u64 stage3;

	//[0x40,0x7f]
	u8 tempdat[0x40];	//(completion key) | (self and peer addr)

	//[0x80,0xff]
	u8 overlap[0x80];	//(only per io data)
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
				(ULONG_PTR)(obj[fd/4].tempdat),
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
	u32* p = (void*)(obj[handle/4].tempdat);
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