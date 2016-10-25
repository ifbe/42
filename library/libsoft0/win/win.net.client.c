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
void printmemory(char*, ...);
void say(char* , ...);




//
static int alive = 0;
static HANDLE thread=0;
static SOCKET sclient = 0;
//
static int st = 0;
static int serlen = sizeof(struct sockaddr_in);
static struct sockaddr_in serAddr;




int deleteclient()
{
	return 1;
}
int createclient()
{
	return 1;
}
int stopclient()
{
    closesocket(sclient);
    WSACleanup();

	sclient=0;
	return 0;
}
int startclient()
{
	return 0;
}





DWORD WINAPI readclient(LPVOID pM)
{
	int ret;
	u8 buf[0x1000];

	if(st == SOCK_RAW)
	{
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
}
int writeclient(char* buf,int len)
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




int listclient(char* type)
{
	if(strncmp(type, "udp", 3) == 0)
	{
		//list udp
	}
	else
	{
		//list tcp
	}
	return 1;
}
int chooseclient(char* type, char* addr, int port, char* extra)
{
	int ret;
	if(sclient != 0)
	{
		alive = 0;

		stopclient();
	}
	if(addr == 0)return 0;
	if(addr[0] == 0)return 0;

    WORD sockVersion = MAKEWORD(2,2);
    WSADATA data; 
    if(WSAStartup(sockVersion, &data) != 0)
    {
		printf("error@WSAStartup\n");
        return 0;
    }

	//
	if(strncmp(type, "raw", 3) == 0)
	{
		sclient = socket(PF_INET, SOCK_RAW, IPPROTO_IP);
		if(sclient == SOCKET_ERROR)
		{
			printf("error:%d@socket\n", GetLastError());
			return 0;
		}

		//
		printf("type=%s, addr=%s, port=%d\n", type, addr, port);
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
	else if(strncmp(type, "udp", 3) == 0)
	{
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

		st = IPPROTO_UDP;
	}
	else
	{
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
			stopclient();
			return 0;
		}

		st = IPPROTO_TCP;
	}

	//thread
	alive = 1;
	thread = CreateThread(NULL, 0, readclient, NULL, 0, NULL);

	//success
	return 1;
}