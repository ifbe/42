#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ws2tcpip.h>	//IP_HDRINCL
#include<ws2bth.h>		//BTHPROTO_RFCOMM
#include<winsock2.h>
#include<windows.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
u64 startthread(void*, void*);
void stopthread();
//
void printmemory(char*, ...);
void say(char* , ...);



struct object
{
	//[0x00,0x0f]
	u64 type0;	//raw, bt, udp, tcp?
	u64 type1;	//ssh, tls?

	//[0x10,0x1f]
	u64 port_src;
	u64 port_dst;

	//[0x20,0x3f]
	u8 addr_src[0x10];
	u8 addr_dst[0x10];

	//[0x40,0xff]
	u8 data[0xc0];
};
struct object* obj;
//
static HANDLE iocpfd;
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
DWORD WINAPI iocpthread(LPVOID pM)
{
	int ret;
	DWORD trans;
	HANDLE iocp = (HANDLE)pM;
return 0;
	while(1)
	{
		ret = GetQueuedCompletionStatus(
			iocp,
			&trans,
			0,//(PULONG_PTR)&PerHandleData,
			0,//(LPOVERLAPPED*)&IpOverlapped,
			INFINITE
		);
	}
	return 0;
}




int readsocket(u64 fd, u8* buf, u64 off, u64 len)
{
/*
	int ret;
	u8 buf[0x1000];

	if(st == IPPROTO_RAW)
	{
		printf("if(nothing incoming){turnoff firewall}\n");
		while(alive == 1)
		{
			ret=recvfrom(fd, buf, 0x1000, 0, NULL, NULL);
			printmemory(buf,ret);
		}
	}
	else if(st == IPPROTO_UDP)
	{
		while(alive == 1)
		{
			ret = sizeof(struct sockaddr_in);
			ret = recvfrom(fd, buf, 256, 0, (void*)&serAddr, &ret);
			if(ret <= 0)continue;

			buf[ret] = 0;
			printf("%s", buf);
			fflush(stdout);
		}
	}
	else if(st == IPPROTO_TCP)
	{
		while(alive == 1)
		{
			ret = recv(fd, buf, 256, 0);
			if(ret <= 0)continue;

			buf[ret] = 0;
			printf("%s", buf);
			fflush(stdout);
		}
	}
	return 0;
*/
}
int writesocket(u64 fd, u8* buf, u64 off, u64 len)
{
	int ret;
/*
	if(st == IPPROTO_TCP)
	{
	}
	else if(st == IPPROTO_UDP)
	{
		ret = sizeof(struct sockaddr_in);
		ret = sendto(fd, buf, len, 0, (void*)&serAddr, ret);
	}
*/
	ret = send(fd, buf, len, 0);
	return ret;
}
int listsocket()
{
}
int choosesocket()
{
}
int stopsocket(u64 fd)
{
	return 0;
}
u64 startsocket(char* addr, int port, int type)
{
	int ret;
	u32 temp[2];

	if(type == 'r')		//raw
	{
		SOCKET rawlisten = socket(PF_INET, SOCK_RAW, IPPROTO_IP);
		if(rawlisten == SOCKET_ERROR)
		{
			printf("error:%d@socket\n", GetLastError());
			return 0;
		}

		//
		struct sockaddr_in serAddr;
		memset(&serAddr, 0, sizeof(serAddr));
		serAddr.sin_addr.s_addr = inet_addr(addr);
		serAddr.sin_family = PF_INET;
		serAddr.sin_port = htons(port);

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
		return rawlisten;
	}
	else if(type == 'U')	//udp server
	{
		int ret;
		int addrlen = sizeof(SOCKADDR_IN);
		SOCKADDR_IN servaddr;

		//
		udplisten = socket(AF_INET,SOCK_DGRAM,0);

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

		return udplisten;
	}
	else if(type == 'T')	//tcp server
	{
		int ret;
		int addrlen = sizeof(SOCKADDR_IN);
		SOCKADDR_IN servaddr;

		//
		tcplisten = socket(AF_INET, SOCK_STREAM, 0);

		//
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port);
		servaddr.sin_addr.s_addr = INADDR_ANY;

		//
		ret = bind(tcplisten, (SOCKADDR*)&servaddr, addrlen);
		if(ret == SOCKET_ERROR)
		{
			printf("error@bind\n");
			closesocket(tcplisten);
			return 0;
		}

		//
		listen(tcplisten, SOMAXCONN);

		return tcplisten;
	}
	else if(type == 'B')	//bluetooth server
	{
	}
	else if(type == 'u')	//udp client
	{
		//
		SOCKET fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(fd == INVALID_SOCKET)
		{
			printf("error@socket\n");
			return 0;
		}

		return fd;
	}
	else if(type == 't')	//tcp client
	{
		//
		SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
			stopsocket(fd);
			return 0;
		}

		return fd;
	}
	else if(type == 'b')	//bluetooth client
	{
		SOCKET fd = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
		if(fd == INVALID_SOCKET)
		{
			printf("error@socket\n");
			return 0;
		}
		return fd;
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