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
//
int stopsocket(u64);
//
void eventwrite(u64,u64,u64,u64);
u64 startthread(void*, void*);
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
//
static HANDLE iocpfd;
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
			pov->stage = 1;
			eventwrite(0, 0x2b6e, fd/4, 0);

			hh = CreateIoCompletionPort(
				(void*)fd,
				iocpfd,
				(ULONG_PTR)(obj[fd/4].tempdat),
				0
			);
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




int readsocket(u64 fd, u8* buf, u64 off, u64 len)
{
	int c,j;
	char* p;
	struct per_io_data* pov;

	pov = (void*)(obj[fd].overlap);
	p = pov->bufdone.buf;
	c = pov->bufdone.len;
	for(j=0;j<c;j++)buf[j] = p[j];
printf("(read)len=%d\n",c);
	free(pov->bufdone.buf);
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
	printf("@send:len=%d,ret=%d,err=%d\n",len,ret,GetLastError());
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
	printf("[%x]close\n",fd);
	return 0;
}
u64 startsocket(char* addr, int port, int type)
{
	int ret;
	if(type == 'r')		//raw
	{
		rawlisten = socket(PF_INET, SOCK_RAW, IPPROTO_IP);
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
		u32* p = (void*)(obj[tcplisten/4].tempdat);
		*p = tcplisten;
		CreateIoCompletionPort(
			(void*)tcplisten,
			iocpfd,
			(ULONG_PTR)p,
			0
		);

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
		SOCKET tmp;
		void* pdata = (void*)(obj[tmp/4].tempdat);
		struct per_io_data* pov;
		for(j=0;j<1000;j++)
		{
			//
			tmp = WSASocket(
				AF_INET, SOCK_STREAM, IPPROTO_TCP,
				0, 0, WSA_FLAG_OVERLAPPED
			);
			if((tmp&0x3)|(tmp>=0x4000))printf("%d\n", tmp/4);

			//
			pov = (void*)(obj[tmp/4].overlap);
			pov->fd = tmp;
			pov->stage = 0;
			ret = acceptex(
				tcplisten, tmp,
				pdata, 0, 0x20, 0x20, 0,
				(void*)pov
			);
		}
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
