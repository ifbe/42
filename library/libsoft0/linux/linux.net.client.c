#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
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
	char buf[256];

	if(st == SOCK_DGRAM)
	{
		while(alive == 1)
		{
			ret=recvfrom(fd, buf, 256, 0, (void*)&server, (void*)&serlen);
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
			ret=read(fd, buf, 256);
			if(ret<0)break;

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

	//create struct
	memset(&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(addr);
	server.sin_port = htons(port);
	serlen = sizeof(struct sockaddr_in);

	if(strncmp(type, "udp", 3) == 0)
	{
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




int startclient()
{
	return 0;
}
int stopclient()
{
	return 0;
}
int createclient()
{
	//do not delete process while receiving SIGPIPE
	struct sigaction sa;
	sa.sa_handler=SIG_IGN;
	sigaction(SIGPIPE,&sa,0);

	return 1;
}
int deleteclient()
{
	return 1;
}
