#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<winsock2.h>
#include<ws2tcpip.h> //IP_HDRINCL is here
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




//
static int alive = 0;
static u64 thread=0;
static SOCKET sclient = 0;
//
static int st = 0;
static int serlen = sizeof(struct sockaddr_in);
static struct sockaddr_in serAddr;
//
static char IPADDRESS[32];
static int PORT;




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
			ret=recvfrom(sclient, buf, 0x1000, 0, NULL, NULL);
			printmemory(buf,ret);
		}
	}
	else if(st == IPPROTO_UDP)
	{
		while(alive == 1)
		{
			ret = recvfrom(sclient, buf, 256, 0, (void*)&serAddr, &serlen);
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
			ret = recv(sclient, buf, 256, 0);
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

	if(st == IPPROTO_TCP)
	{
		ret = send(sclient, buf, len, 0);
	}
	else if(st == IPPROTO_UDP)
	{
		ret = sendto(sclient, buf, len, 0, (void*)&serAddr, serlen);
	}
	//printf("[%d]%c,%d -> %d,%d\n", sclient, buf[0], len, ret, GetLastError());
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
int startsocket(char* addr, int port, int type)
{
	int ret;
	u8 temp[8];

	if(sclient != 0)
	{
		alive = 0;

		stopsocket(sclient);
	}
	if(addr == 0)return 0;
	if(addr[0] == 0)return 0;

	if(type == 'r')
	{
		sclient = socket(PF_INET, SOCK_RAW, IPPROTO_IP);
		if(sclient == SOCKET_ERROR)
		{
			printf("error:%d@socket\n", GetLastError());
			return 0;
		}

		//
		memset(&serAddr, 0, sizeof(serAddr));
		serAddr.sin_addr.s_addr = inet_addr(addr);
		serAddr.sin_family = PF_INET;
		serAddr.sin_port = htons(port);

		//
		if(bind(sclient, (void*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			printf("error:%d@bind\n", GetLastError());
			return 0;
		}

		//
		int one=1;
		if(WSAIoctl(sclient, SIO_RCVALL, &one, 4, 0, 0, (LPDWORD)&ret, 0, 0) == SOCKET_ERROR)
		{
			printf("error:%d@WSAIoctl\n", GetLastError());
			return 0;
		}

		//
		if(setsockopt(sclient, IPPROTO_IP, IP_HDRINCL, (char *)&ret, 4)==SOCKET_ERROR)
		{
			printf("error:%d@setsockopt\n", GetLastError());
			return 0;
		}

		st = IPPROTO_RAW;
	}
	else if(type == 'u')
	{
		u32 temp[2];
		//
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(port);
		serAddr.sin_addr.S_un.S_addr = inet_addr(addr); 

		//
		sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(sclient == INVALID_SOCKET)
		{
			printf("error@socket\n");
			return 0;
		}

		//
		selfname(sclient, temp);
		printf("%d.%d.%d.%d:%d\n",temp[0],temp[1],temp[2],temp[3],*(u32*)(temp+4));
		st = IPPROTO_UDP;
	}
	else
	{
		u32 temp[2];
		//
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(port);
		serAddr.sin_addr.S_un.S_addr = inet_addr(addr); 

		//
		sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(sclient == INVALID_SOCKET)
		{
			printf("error@socket\n");
			return 0;
		}

		if (connect(sclient, (void*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			printf("error:%d@connect\n",GetLastError());
			stopsocket(sclient);
			return 0;
		}

		//
		selfname(sclient, temp);
		printf("%d.%d.%d.%d:%d\n",temp[0],temp[1],temp[2],temp[3],*(u32*)(temp+4));
		st = IPPROTO_TCP;
	}

	//thread
	alive = 1;
	thread = startthread(iocpthread, 0);

	//success
	return 1;
}
int deletesocket()
{
	WSACleanup();
	return 1;
}
int createsocket()
{
	WSADATA data; 
	WORD sockVersion = MAKEWORD(2,2);
	if(WSAStartup(sockVersion, &data) != 0)
	{
		printf("error@WSAStartup\n");
		return 0;
	}
	return 1;
}
