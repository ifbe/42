#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ws2tcpip.h>	//IP_HDRINCL
#include<ws2bth.h>		//BTHPROTO_RFCOMM
#include<winsock2.h>
#include<mswsock.h>
#include<windows.h>
#include"system.h"
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
//
static SOCKET btlisten;
static SOCKET rawlisten;
static SOCKET tcplisten;
static SOCKET udplisten;




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




int readsocket(u64 fd, u8* buf, u64 off, u64 len)
{
	int c,j;
	char* p;
	struct per_io_data* pio = (void*)(obj[fd].data);
	c = pio->count;
	if(c == 0)return -1;	//disconnect

	p = pio->bufing.buf;
	for(j=0;j<c;j++)buf[j] = p[j];

	pio->count = 0;
	iocp_mod(fd);
	return c;
}
int writesocket(u64 fd, u8* buf, u64 off, u64 len)
{
	int ret;
	DWORD dwret;
	WSABUF wbuf;
/*
	if(st == IPPROTO_UDP)
	{
		ret = sizeof(struct sockaddr_in);
		ret = sendto(fd, buf, len, 0, (void*)&serAddr, ret);
	}
*/
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
		tcplisten,
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
		printf("error@WSAIoctl\n");
		return 0;
	}

	disconnectex(fd*4, 0, TF_REUSE_SOCKET, 0);
	printf("[%x]close\n", fd*4);
	return 0;
}
u64 startsocket(char* addr, int port, int type)
{
	int ret;
	if(type == 'R')		//RAW
	{
		rawlisten = WSASocket(
			PF_INET, SOCK_RAW, IPPROTO_IP,
			0, 0, WSA_FLAG_OVERLAPPED
		);
		if(rawlisten == SOCKET_ERROR)
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
		if(bind(rawlisten, (void*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			printf("error:%d@bind\n", GetLastError());
			return 0;
		}

		//
		int one=1;
		if(WSAIoctl(rawlisten, SIO_RCVALL, &one, 4, 0, 0, (LPDWORD)&ret, 0, 0) == SOCKET_ERROR)
		{
			printf("error:%d@WSAIoctl\n", GetLastError());
			return 0;
		}

		//
		if(setsockopt(rawlisten, IPPROTO_IP, IP_HDRINCL, (char *)&ret, 4)==SOCKET_ERROR)
		{
			printf("error:%d@setsockopt\n", GetLastError());
			return 0;
		}

		//
		obj[rawlisten/4].type_sock = type;
		obj[rawlisten/4].type_road = 0;
		iocp_add(rawlisten);
		iocp_mod(rawlisten);
		return rawlisten/4;
	}
	else if(type == 'r')	//raw
	{
		SOCKET fd;
		return fd/4;
	}
	else if(type == 'U')	//UDP
	{
		int ret;
		int addrlen = sizeof(SOCKADDR_IN);
		SOCKADDR_IN servaddr;

		//
		udplisten = WSASocket(
			AF_INET, SOCK_DGRAM, 0,
			0, 0, WSA_FLAG_OVERLAPPED
		);

		//
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port);
		servaddr.sin_addr.s_addr = INADDR_ANY;

		//
		ret = bind(udplisten, (SOCKADDR*)&servaddr, addrlen);
		if(ret == SOCKET_ERROR)
		{
			printf("error@bind\n");
			closesocket(udplisten);
			return 0;
		}

		//
		obj[udplisten].type_sock = type;
		obj[udplisten].type_road = 0;
		iocp_add(udplisten);
		iocp_mod(udplisten);
		return udplisten/4;
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
		obj[fd].type_sock = type;
		obj[fd].type_road = 0;
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
		tcplisten = WSASocket(
			AF_INET, SOCK_STREAM, IPPROTO_TCP,
			0, 0, WSA_FLAG_OVERLAPPED
		);
		if(tcplisten == INVALID_SOCKET)
		{
			printf("error@wsasocket\n");
			return 0;
		}

		//server.2
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port);
		servaddr.sin_addr.s_addr = INADDR_ANY;

		//server.3
		ret = bind(tcplisten, (SOCKADDR*)&servaddr, addrlen);
		if(ret == SOCKET_ERROR)
		{
			printf("error@bind\n");
			closesocket(tcplisten);
			return 0;
		}

		//server.4
		ret = listen(tcplisten, SOMAXCONN);
		if(ret == -1)
		{
			printf("error@listen\n");
			closesocket(tcplisten);
			return 0;
		}

		//server.5
		obj[tcplisten].type_sock = type;
		obj[tcplisten].type_road = 0;
		iocp_add(tcplisten);

		//client.1
		LPFN_ACCEPTEX acceptex = NULL;
		GUID guidacceptex = WSAID_ACCEPTEX;
		DWORD dwret = 0;
		ret = WSAIoctl(
			tcplisten,
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
				tcplisten, t,
				(void*)pfd, 0, 0x20, 0x20, 0,
				(void*)pio
			);
		}
		return tcplisten/4;
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
		iocp_add(fd);
		iocp_mod(fd);
		obj[fd].type_sock = type;
		obj[fd].type_road = 0;
		return fd/4;
	}
	else if(type == 'B')	//bluetooth server
	{
		return btlisten/4;
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
		iocp_add(fd);
		iocp_mod(fd);
		obj[fd].type_sock = type;
		obj[fd].type_road = 0;
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
