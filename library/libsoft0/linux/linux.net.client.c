#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<pthread.h>
#include<signal.h>
#include<arpa/inet.h>
#include<net/if.h>
#include<netinet/in.h>
#include<netinet/ether.h>
#include<sys/ioctl.h>
#include<sys/socket.h>
#include<sys/types.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(char*,int);
void say(char* , ...);




//
static int alive = 0;
static pthread_t id;
//
static int st = 0;
static int fd = 0;
static socklen_t serlen = 0;
static struct sockaddr_in server = {0};




void* readclient(void* p)
{
	int ret;
	u8 buf[0x1000];

	if(st == SOCK_RAW)
	{
		while(alive == 1)
		{
			ret=recvfrom(fd, buf, 0x1000, 0, NULL, NULL);
			printmemory(buf,ret);
		}
	}
	else if(st == SOCK_DGRAM)
	{
		while(alive == 1)
		{
			ret=recvfrom(fd, buf, 0x1000, 0, (void*)&server, (void*)&serlen);
			if(ret<0)break;

			buf[ret] = 0;
			printf("%s", buf);
			fflush(stdout);
		}
	}
	else
	{
		while(alive == 1)
		{
			ret=read(fd, buf, 0x1000);
			if(ret<0)break;

			buf[ret] = 0;
			printf("%s", buf);
			fflush(stdout);
		}
	}
	return 0;
}
int writeclient(u8* buf,int len)
{
	int ret;
	if(st == SOCK_DGRAM)
	{
		ret=sendto(fd, buf, len, 0, (void*)&server, serlen);
		//say("err:%d@sendto:%d\n",errno,ret);
	}
	else
	{
		ret=write(fd, buf, len);
		//say("err:%d@write:%d\n",errno,ret);
	}
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

	if(fd > 0)
	{
		alive = 0;

		close(fd);
		fd = 0;
	}
	if(addr == 0)return 0;
	if(addr[0] == 0)return 0;

	if(strncmp(type, "raw", 3) == 0)
	{
		int sockopt;
		struct ifreq ifopts;
		char* ifName = "eth0";

		fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
		if(fd == -1)
		{
			printf("error%d@socket:%d\n",errno,fd);
			return 0;
		}

		strncpy(ifopts.ifr_name, ifName, 5);
		ioctl(fd, SIOCGIFFLAGS, &ifopts);
		ifopts.ifr_flags |= IFF_PROMISC;
		ioctl(fd, SIOCSIFFLAGS, &ifopts);

		ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt);
		if(ret == -1)
		{
			perror("setsockopt");
			close(fd);
			exit(EXIT_FAILURE);
		}

		ret = setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, ifName, 5);
		if(ret == -1)
		{
			perror("SO_BINDTODEVICE");
			close(fd);
			exit(EXIT_FAILURE);
		}

		st = SOCK_RAW;
	}
	else if(strncmp(type, "udp", 3) == 0)
	{
		//create struct
		memset(&server, 0, sizeof(struct sockaddr_in));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = inet_addr(addr);
		server.sin_port = htons(port);
		serlen = sizeof(struct sockaddr_in);

		//create socket
		fd = socket(AF_INET, SOCK_DGRAM, 0);
		if(fd == -1)
		{
			printf("error%d@socket:%d\n",errno,fd);
			return 0;
		}

		st = SOCK_DGRAM;
	}
	else
	{
		//create struct
		memset(&server, 0, sizeof(struct sockaddr_in));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = inet_addr(addr);
		server.sin_port = htons(port);
		serlen = sizeof(struct sockaddr_in);

		//create socket
		fd = socket(AF_INET, SOCK_STREAM, 0);
		if(fd == -1)
		{
			printf("error%d@socket:%d\n",errno,fd);
			return 0;
		}

		//connect
		ret = connect(fd, (struct sockaddr*)&server, sizeof(server));
		if(ret < 0)
		{
			//printf("connect error\n");
			return 0;
		}

		st = SOCK_STREAM;
	}

	//thread
	alive = 1;
	pthread_create(&id, NULL, readclient, NULL);

	//success
	return 1;
}




int stopclient()
{
	return 0;
}
int startclient()
{
	return 0;
}
int deleteclient()
{
	return 1;
}
int createclient()
{
	struct sigaction sa;
	sa.sa_handler=SIG_IGN;
	sigaction(SIGPIPE,&sa,0);

	return 1;
}
