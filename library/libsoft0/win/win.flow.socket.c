#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>	//IP_HDRINCL
#include <ws2bth.h>		//BTHPROTO_RFCOMM
#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include "libsoft.h"
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




int readsocket(int fd, int off, void* buf, int len)
{
	int c,j;
	char* src;
	char* dst = buf;
	struct per_io_data* pio = (void*)(obj[fd].data);
	c = pio->count;
	if(c == 0)return 0;	//disconnect

	src = pio->bufing.buf;
	for(j=0;j<c;j++)dst[j] = src[j];

	pio->count = 0;
	iocp_mod(fd*4);
	return c;
}
int writesocket(int fd, int off, void* buf, int len)
{
	int j,ret;
	DWORD dwret;
	WSABUF wbuf;

	ret = obj[fd].type;
	if(('u' == ret) | ('U' == ret))
	{
		while(1)
		{
			if(len <= 1024)break;

			wbuf.buf = buf;
			wbuf.len = 1024;

			buf += 1024;
			len -= 1024;
		}
	}

	wbuf.buf = buf;
	wbuf.len = len;
	ret = WSASend(fd*4, &wbuf, 1, &dwret, 0, 0, 0);

	printf("@send:len=%d,ret=%d,err=%d\n",len,ret,GetLastError());
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
	struct hostent* host;
	int ret;
	for(ret=0;ret<256;ret++)
	{
		if((addr[ret]>='a')&&(addr[ret]<='z'))
		{
			host = gethostbyname(addr);
			addr = inet_ntoa(*(struct in_addr*)host->h_addr_list[0]);
			break;
		}
	}

	if(type == 'R')		//RAW
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
		obj[fd/4].type = type;
		obj[fd/4].name = 0;
		iocp_add(fd);
		iocp_mod(fd);
		return fd/4;
	}
	else if(type == 'r')	//raw
	{
		return 0;
	}
	else if(type == 'U')	//UDP
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
		obj[fd/4].type = type;
		obj[fd/4].name = 0;
		iocp_add(fd);
		iocp_mod(fd);
		return fd/4;
	}
	else if(type == 'u')	//udp client
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
		server.sin_addr.s_addr = inet_addr(addr);
		server.sin_port = htons(port);

		//
		ret = connect(fd, (struct sockaddr*)&server, sizeof(server));
		if(ret < 0)
		{
			printf("connect error\n");
			return 0;
		}

		//
		obj[fd/4].type = type;
		obj[fd/4].name = 0;
		iocp_add(fd);
		iocp_mod(fd);
		return fd/4;
	}
	else if(type == 'T')	//tcp server
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

		obj[fd/4].type = type;
		obj[fd/4].name = 0;
		iocp_add(fd);
		return fd/4;
	}
	else if(type == 't')	//tcp client
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
		obj[fd/4].type = type;
		obj[fd/4].name = 0;
		iocp_add(fd);
		iocp_mod(fd);
		return fd/4;
	}
	else if(type == 'B')	//bluetooth server
	{
		return 0;
	}
	else if(type == 'b')	//bluetooth client
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
		obj[fd/4].type = type;
		obj[fd/4].name = 0;
		iocp_add(fd);
		iocp_mod(fd);
		return fd/4;
	}
	else printf("error@type\n");
	return 0;
}
void deletesocket()
{
	WSACleanup();
}
void createsocket(void* addr)
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
