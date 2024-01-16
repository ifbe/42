#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>	//IP_HDRINCL
#include <ws2bth.h>		//BTHPROTO_RFCOMM
#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include "libsoft.h"




struct perio
{
	OVERLAPPED overlap;		//start with struct overlap
	WSABUF bufing;		//follow by own data
	SOCKET childsock;
};
struct perfd{
	struct perio perio[2];
	SOCKET sock;
};
static HANDLE iocpfd;
static _obj* g_obj = 0;
//
static _obj* getobjbysock(SOCKET sock)
{
	return &g_obj[sock/4];
}
static SOCKET getsockbyobj(_obj* oo)
{
	struct perfd* perfd = (void*)(oo->priv_256b);
	return perfd->sock;
}




void iocp_add(SOCKET sock, int type)
{
	_obj* tmp = getobjbysock(sock);
	CreateIoCompletionPort(
		(void*)sock,
		iocpfd,
		(ULONG_PTR)tmp,
		0
	);
	if(_TCP_ == type)return;

	struct perfd* perfd = (void*)(tmp->priv_256b);
	struct perio* perio = &perfd->perio[0];
	perio->bufing.buf = malloc(0x100000);
	perio->bufing.len = 0x100000;
}
void iocp_del(SOCKET sock, int type)
{
	_obj* tmp = getobjbysock(sock);
	struct perfd* perfd = (void*)(tmp->priv_256b);
	struct perio* perio = &perfd->perio[0];
	if(perio->bufing.buf){
		free(perio->bufing.buf);
		perio->bufing.buf = 0;
	}
}
void iocp_mod(SOCKET sock, int type)
{
	int ret;
	DWORD tran = 0;
	DWORD flag = 0;

	_obj* oo = getobjbysock(sock);
	struct perfd* perfd = (void*)(oo->priv_256b);
	struct perio* perio = &perfd->perio[0];

	switch(type){
	case _UDP_:
	case _udp_:{
		ret = sizeof(struct sockaddr_in);
		ret = WSARecvFrom(sock,
			&(perio->bufing), 1,
			&tran, &flag,
			(void*)(oo->sockinfo.peer), &ret,
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

	SOCKET fd;
	SOCKET cc;
	_obj* parent;
	_obj* child;

	_obj* oo = NULL;
	struct perfd* perfd = NULL;		//in struct _obj
	struct perio* perio = NULL;		//in struct perfd

	ret = GetCurrentThreadId();
	//logtoall("threadid = %x\n", ret);

	while(1)
	{
		tran = 0;
		ret = GetQueuedCompletionStatus(
			iocpfd,
			&tran,
			(void*)&oo,
			(void*)&perio,
			INFINITE
		);
		if(0 == ret)continue;

		fd = getsockbyobj(oo);
		if(_TCP_ == oo->type)
		{
			cc = perio->childsock;
			child = getobjbysock(cc);
			printf("accept: fd=%x, parent=%x\n", cc, fd);

			child->type = _Tcp_;
			child->sockinfo.parent = oo;
			child->sockinfo.child = 0;
			child->sockinfo.fd = cc;

			//oo->sockinfo.parent = 0;
			oo->sockinfo.child = child;

			DWORD dwret = 0;
			LPFN_GETACCEPTEXSOCKADDRS getacceptexsockaddrs = NULL;
			GUID guid_GetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
			ret = WSAIoctl(
				fd,
				SIO_GET_EXTENSION_FUNCTION_POINTER,
				&guid_GetAcceptExSockaddrs,
				sizeof(guid_GetAcceptExSockaddrs),
				&getacceptexsockaddrs,
				sizeof(getacceptexsockaddrs),
				&dwret,
				NULL,
				NULL
			);
			if(ret != 0){
				printf("errno=%d@WSAIoctl WSAID_ACCEPTEX\n",GetLastError());
				return 0;
			}

			struct sockaddr_in* localaddr;
			int localsize;
			struct sockaddr_in* remoteaddr;
			int remotesize;
			getacceptexsockaddrs(
				(void*)(child->sockinfo.self), 0, 0x20, 0x20,
				(struct sockaddr**)&localaddr, &localsize,
				(struct sockaddr**)&remoteaddr, &remotesize
			);
			printf("GetAcceptExSockaddrs: %p,%x,%p,%x\n",
				localaddr, localsize,
				remoteaddr, remotesize
			);

			if(localaddr){
				u8* pp = (u8*)&localaddr->sin_addr;
				printf("self: port=%d,addr=%d.%d.%d.%d\n", localaddr->sin_port,
					pp[0], pp[1], pp[2], pp[3]
				);
				memcpy(child->sockinfo.self, localaddr, localsize);
			}
			if(remoteaddr){
				u8* pp = (u8*)&remoteaddr->sin_addr;
				printf("peer: port=%d,addr=%d.%d.%d.%d\n", remoteaddr->sin_port,
					pp[0], pp[1], pp[2], pp[3]
				);
				memcpy(child->sockinfo.self, remoteaddr, localsize);
			}

			//must call to getpeername
			ret = setsockopt(cc, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&fd, sizeof(SOCKET));

			iocp_add(cc, _Tcp_);
			iocp_mod(cc, _Tcp_);
			continue;
		}
		if(0 == tran)
		{
			iocp_del(fd, 0);
			system_delete(oo);
			printf("discon: fd=%x\n", fd);
			continue;
		}

		switch(oo->type){
		case _Tcp_:{
			if(	(0 == oo->irel0) && (0 == oo->orel0) ){
				parent = oo->sockinfo.parent;
				memcpy(parent->sockinfo.peer, oo->sockinfo.peer, 8);
				//parent->sockinfo.parent = 0;
				parent->sockinfo.child = oo;

				//parent send
				oo = parent;
			}

			give_data_into_peer(oo,_dst_, stack,0, 0,0, perio->bufing.buf,tran);
			iocp_mod(fd, oo->type);
			break;
		}//Tcp
		case _UDP_:{
			printf("UDP recv:fd=%x,len=%x\n",fd,tran);
			give_data_into_peer(oo,_dst_, stack,0, (p64)oo->sockinfo.peer,0, perio->bufing.buf,tran);
			iocp_mod(fd, oo->type);
			break;
		}
		default:{
			give_data_into_peer(oo,_dst_, stack,0, 0,0, perio->bufing.buf,tran);
			iocp_mod(fd, oo->type);
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
	g_obj = addr;

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
