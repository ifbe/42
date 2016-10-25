#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<winsock2.h>
#include<windows.h>
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
	char buf[256];

	while(alive == 1)
	{
		if(st == IPPROTO_TCP)
		{
			ret = recv(sclient, buf, 256, 0);
		}
		else if(st == IPPROTO_UDP)
		{
			ret = recvfrom(sclient, buf, 256, 0, (void*)&serAddr, &serlen);
		}
		if(ret <= 0)continue;

		buf[ret] = 0;
		printf("%s", buf);
		fflush(stdout);
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
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);
    serAddr.sin_addr.S_un.S_addr = inet_addr(addr); 

	if(strncmp(type, "udp", 3) == 0)
	{
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