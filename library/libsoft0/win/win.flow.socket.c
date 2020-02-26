#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>	//IP_HDRINCL
#include <ws2bth.h>		//BTHPROTO_RFCOMM
#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include "libsoft.h"
#define _BT_ hex16('B','T')
#define _bt_ hex16('b','t')
void iocp_add(SOCKET);
void iocp_del(SOCKET);
void iocp_mod(SOCKET);




struct per_io_data
{
	OVERLAPPED overlap;
	WSABUF bufing;
	int count;
	int stage;
	SOCKET fd;
};
static struct object* obj;




void selfname(u64 fd, u32* buf)
{
	struct sockaddr_in addr;
	u32 len = sizeof(struct sockaddr_in);
	getsockname(fd, (void*)&addr, &len);

	buf[0] = *(u32*)&addr.sin_addr;
	buf[1] = addr.sin_port;
}
void peername(u64 fd, u32* buf)
{
	struct sockaddr_in addr;
	u32 len = sizeof(struct sockaddr_in);
	getpeername(fd, (void*)&addr, &len);

	buf[0] = *(u32*)&addr.sin_addr;
	buf[1] = addr.sin_port;
}
u32 resolvehostname(char* addr)
{
	struct hostent* host;
	char** ptr;
	int j;

	//get
	host = gethostbyname(addr);
	if(0 == host){
		printf("err@gethostbyname: %d\n", errno);
		return 0;
	}

	//alias
	j = 0;
	ptr = host->h_aliases;
	while(1){
		if(0 == ptr[j])break;
		printf("alias:%s\n", ptr[j]);
		j++;
	}

	//address
	j = 0;
	ptr = host->h_addr_list;
	while(1){
		if(0 == ptr[j])break;
		printf("ipadd:%s\n", inet_ntoa(*(struct in_addr*)ptr[j]));
		j++;
	}
	return *(u32*)ptr[0];
}




int readsocket(int fd, void* tmp, void* buf, int len)
{
	int j,ret;
	char* src;
	char* dst;
	struct per_io_data* pio;

	//pio
	pio = (void*)(obj[fd].data);
	ret = pio->count;
	if(0 == ret)return 0;	//disconnect

	//data
	src = pio->bufing.buf;
	dst = buf;
	for(j=0;j<ret;j++)dst[j] = src[j];

	//peer
	j = obj[fd].type;
	if(_UDP_ == j)
	{
		dst = tmp;
		src = obj[fd].peer;
		for(j=0;j<8;j++)dst[j] = src[j];
	}

	pio->count = 0;
	iocp_mod(fd*4);
	return ret;
}
int writesocket(int fd, void* tmp, void* buf, int len)
{
	int ret;
	DWORD dwret;
	WSABUF wbuf;

	ret = obj[fd].type;
	if(_UDP_ == ret)
	{
		wbuf.buf = buf;
		wbuf.len = len;
		if(0 == tmp)tmp = obj[fd].peer;
		ret = WSASendTo(fd*4, &wbuf, 1, &dwret, 0,
			tmp, sizeof(struct sockaddr_in), 0, 0);
		return len;
	}

	wbuf.buf = buf;
	wbuf.len = len;
	ret = WSASend(fd*4, &wbuf, 1, &dwret, 0, 0, 0);
	//printf("@send:len=%d,ret=%d,err=%d\n",len,ret,GetLastError());
	return len;
}
int listsocket()
{
}
int choosesocket()
{
}
int stopsocket(SOCKET fd)
{
	LPFN_DISCONNECTEX disconnectex = NULL;
	GUID guiddisconnectex = WSAID_DISCONNECTEX;
	DWORD dwret = 0;
	int ret = WSAIoctl(
		fd*4,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guiddisconnectex,
		sizeof(guiddisconnectex),
		&disconnectex,
		sizeof(disconnectex),
		&dwret,
		NULL,
		NULL
	);
	if(ret != 0)
	{
		printf("error@stopsocket.WSAIoctl\n");
		return 0;
	}

	disconnectex(fd*4, 0, TF_REUSE_SOCKET, 0);
	return 0;
}
u64 startsocket(char* addr, int port, int type)
{
	int j,ret;
	u32 ipv4;

	//dns thing
	for(j=0;j<256;j++)
	{
		if((addr[j]>='a')&&(addr[j]<='z'))
		{
			ipv4 = resolvehostname(addr);
			say("%x\n",ipv4);
			if(0 == ipv4)return 0;

			addr = inet_ntoa(*(struct in_addr*)&ipv4);
			break;
		}
	}

	if(_RAW_ == type)
	{
		SOCKET fd = WSASocket(
			PF_INET, SOCK_RAW, IPPROTO_IP,
			0, 0, WSA_FLAG_OVERLAPPED
		);
		if(fd == SOCKET_ERROR)
		{
			printf("error:%d@socket\n", GetLastError());
			return 0;
		}

		//
		struct sockaddr_in serAddr;
		memset(&serAddr, 0, sizeof(serAddr));
		serAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		serAddr.sin_family = PF_INET;
		serAddr.sin_port = htons(0);

		//
		if(bind(fd, (void*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			printf("error:%d@bind\n", GetLastError());
			return 0;
		}

		//
		int one=1;
		if(WSAIoctl(fd, SIO_RCVALL, &one, 4, 0, 0, (LPDWORD)&ret, 0, 0) == SOCKET_ERROR)
		{
			printf("error:%d@WSAIoctl\n", GetLastError());
			return 0;
		}

		//
		if(setsockopt(fd, IPPROTO_IP, IP_HDRINCL, (char *)&ret, 4)==SOCKET_ERROR)
		{
			printf("error:%d@setsockopt\n", GetLastError());
			return 0;
		}

		//
		iocp_add(fd);
		iocp_mod(fd);
		return fd/4;
	}
	else if(_raw_ == type)
	{
		return 0;
	}
	else if(_UDP_ == type)
	{
		int ret;
		int addrlen = sizeof(SOCKADDR_IN);
		SOCKADDR_IN servaddr;

		//
		SOCKET fd = WSASocket(
			AF_INET, SOCK_DGRAM, 0,
			0, 0, WSA_FLAG_OVERLAPPED
		);

		//
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port);
		servaddr.sin_addr.s_addr = INADDR_ANY;

		//
		ret = bind(fd, (SOCKADDR*)&servaddr, addrlen);
		if(ret == SOCKET_ERROR)
		{
			printf("error@bind\n");
			closesocket(fd);
			return 0;
		}

		//
		iocp_add(fd);
		iocp_mod(fd);
		return fd/4;
	}
	else if(_udp_ == type)
	{
		//
		SOCKET fd = WSASocket(
			AF_INET, SOCK_DGRAM, IPPROTO_UDP,
			0, 0, WSA_FLAG_OVERLAPPED
		);
		if(fd == INVALID_SOCKET)
		{
			printf("error@socket\n");
			return 0;
		}

		struct sockaddr_in server;
		memset(&server, 0, sizeof(struct sockaddr_in));
		server.sin_family = AF_INET;
		server.sin_port = htons(port);
		server.sin_addr.s_addr = inet_addr(addr);

		//
		ret = connect(fd, (struct sockaddr*)&server, sizeof(server));
		if(ret < 0)
		{
			printf("connect error\n");
			return 0;
		}

		//
		iocp_add(fd);
		iocp_mod(fd);
		return fd/4;
	}
	else if(_TCP_ == type)
	{
		int ret;
		int addrlen = sizeof(SOCKADDR_IN);
		SOCKADDR_IN servaddr;

		//server.1
		SOCKET fd = WSASocket(
			AF_INET, SOCK_STREAM, IPPROTO_TCP,
			0, 0, WSA_FLAG_OVERLAPPED
		);
		if(fd == INVALID_SOCKET)
		{
			printf("error@wsasocket\n");
			return 0;
		}

		//server.2
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port);
		servaddr.sin_addr.s_addr = INADDR_ANY;

		//server.3
		ret = bind(fd, (SOCKADDR*)&servaddr, addrlen);
		if(ret == SOCKET_ERROR)
		{
			printf("error@bind\n");
			closesocket(fd);
			return 0;
		}

		//server.4
		ret = listen(fd, SOMAXCONN);
		if(ret == -1)
		{
			printf("error@listen\n");
			closesocket(fd);
			return 0;
		}

		//client.1
		LPFN_ACCEPTEX acceptex = NULL;
		GUID guidacceptex = WSAID_ACCEPTEX;
		DWORD dwret = 0;
		ret = WSAIoctl(
			fd,
			SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidacceptex,
			sizeof(guidacceptex),
			&acceptex,
			sizeof(acceptex),
			&dwret,
			NULL,
			NULL
		);
		if(ret != 0)
		{
			printf("error@WSAIoctl\n");
			return 0;
		}

		//clients.2
		int j;
		SOCKET t;
		u32* pfd;
		struct per_io_data* pio;
		for(j=0;j<0x400;j++)
		{
			t = WSASocket(
				AF_INET, SOCK_STREAM, IPPROTO_TCP,
				0, 0, WSA_FLAG_OVERLAPPED
			);
			if(t&0x3)printf("%d\n", t);

			pfd = (void*)(obj[t/4].self);
			*pfd = t;

			pio = (void*)(obj[t/4].data);
			pio->count = 0;
			pio->stage = 0;
			pio->fd = t;

			ret = acceptex(
				fd, t,
				(void*)pfd, 0, 0x20, 0x20, 0,
				(void*)pio
			);
		}

		iocp_add(fd);
		return fd/4;
	}
	else if(_tcp_ == type)
	{
		//
		SOCKET fd = WSASocket(
			AF_INET, SOCK_STREAM, IPPROTO_TCP,
			0, 0, WSA_FLAG_OVERLAPPED
		);
		if(fd == INVALID_SOCKET)
		{
			printf("error@socket\n");
			return 0;
		}

		//
		struct sockaddr_in serAddr;
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(port);
		serAddr.sin_addr.S_un.S_addr = inet_addr(addr); 
		if (connect(fd, (void*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			printf("error:%d@connect\n",GetLastError());
			stopsocket(fd/4);
			return 0;
		}

		//
		iocp_add(fd);
		iocp_mod(fd);
		return fd/4;
	}
	else if(_BT_ == type)
	{
		return 0;
	}
	else if(_bt_ == type)
	{
		SOCKET fd = WSASocket(
			AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM,
			0, 0, WSA_FLAG_OVERLAPPED
		);
		if(fd == INVALID_SOCKET)
		{
			printf("error@socket\n");
			return 0;
		}

		//
		iocp_add(fd);
		iocp_mod(fd);
		return fd/4;
	}
	else printf("error@type\n");
	return 0;
}




void freesocket()
{
	WSACleanup();
}
void initsocket(void* addr)
{
	obj = addr;

	//socket
	WSADATA data; 
	WORD sockVersion = MAKEWORD(2,2);
	if(WSAStartup(sockVersion, &data) != 0)
	{
		printf("error@WSAStartup\n");
		return;
	}
}
