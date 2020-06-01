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
	SOCKET sock;
};
static HANDLE iocpfd;
//
static struct sysobj* obj = 0;
static struct sysobj* getobjbysock(SOCKET sock)
{
	return &obj[sock/4];
}
static struct sysobj* getobjbid(int idx)
{
	return &obj[idx];
}
static int getidbysock(SOCKET sock)
{
	return sock/4;
}
static SOCKET getsockbyid(int idx)
{
	return idx*4;
}




void iocp_add(SOCKET sock, int type)
{
	struct sysobj* tmp = getobjbysock(sock);
	CreateIoCompletionPort(
		(void*)sock,
		iocpfd,
		(ULONG_PTR)tmp,
		0
	);
	if(_TCP_ == type)return;

	struct per_io_data* perio = (void*)(tmp->data);
	perio->bufing.buf = malloc(0x100000);
	perio->bufing.len = 0x100000;
}
void iocp_del(SOCKET sock, int type)
{
	struct sysobj* tmp = getobjbysock(sock);
	struct per_io_data* perio = (void*)(tmp->data);
	if(perio->bufing.buf)
	{
		free(perio->bufing.buf);
		perio->bufing.buf = 0;
	}
}
void iocp_mod(SOCKET sock, int type)
{
	int ret;
	DWORD tran = 0;
	DWORD flag = 0;

	struct sysobj* perfd = getobjbysock(sock);
	struct per_io_data* perio = (void*)(perfd->data);

	switch(type){
	case _UDP_:
	case _udp_:{
		ret = sizeof(struct sockaddr_in);
		ret = WSARecvFrom(sock,
			&(perio->bufing), 1,
			&tran, &flag,
			(void*)(perfd->peer), &ret,
			(void*)perio, NULL
		);
		break;
	}
	default:{
		ret = WSARecv(sock,
			&(perio->bufing), 1,
			&tran, &flag,
			(void*)perio, NULL
		);
		break;
	}
	}
}
DWORD WINAPI iocpthread(LPVOID pM)
{
	int ret;
	DWORD tran;
	struct halfrel stack[0x80];
	struct sysobj* perfd = NULL;
	struct per_io_data* perio = NULL;

	SOCKET fd;
	SOCKET cc;
	struct sysobj* parent;
	struct sysobj* child;

	ret = GetCurrentThreadId();
	//say("threadid = %x\n", ret);

	while(1)
	{
		tran = 0;
		ret = GetQueuedCompletionStatus(
			iocpfd,
			&tran,
			(void*)&perfd,
			(void*)&perio,
			INFINITE
		);
		if(0 == ret)continue;

		fd = getsockbyid(perfd->selffd);
		if(_TCP_ == perfd->type)
		{
			cc = perio->sock;
			child = getobjbysock(cc);
			printf("parent=%x, child=%x\n", fd, cc);

			child->type = _Tcp_;
			child->name = 0;
			child->irel0 = child->ireln = 0;
			child->orel0 = child->oreln = 0;
			child->selffd = getidbysock(cc);
			child->selfobj = child;
			child->tempfd = getidbysock(fd);
			child->tempobj = perfd;

			//peername
			iocp_add(cc, _Tcp_);
			iocp_mod(cc, _Tcp_);
			continue;
		}
		if(0 == tran)
		{
			iocp_del(fd, 0);
			systemdelete(perfd);
			continue;
		}

		switch(perfd->type){
		case _Tcp_:{
			if(	(0 == perfd->irel0) && (0 == perfd->orel0) ){
				parent = perfd->tempobj;
				memcpy(parent->peer, perfd->peer, 8);

				//tell parent, its me
				parent->tempfd = perfd->selffd;
				parent->tempobj = perfd;

				//parent send
				perfd = parent;
			}

			give_data_into_peer(perfd,_dst_, stack,0, 0,0, perio->bufing.buf,tran);
			iocp_mod(fd, perfd->type);
			break;
		}//Tcp
		case _UDP_:{
			give_data_into_peer(perfd,_dst_, stack,0, perfd->peer,0, perio->bufing.buf,tran);
			iocp_mod(fd, perfd->type);
			break;
		}
		default:{
			give_data_into_peer(perfd,_dst_, stack,0, 0,0, perio->bufing.buf,tran);
			iocp_mod(fd, perfd->type);
			break;
		}//default
		}//switch
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
