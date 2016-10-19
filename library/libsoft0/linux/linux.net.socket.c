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
static int fd = 0;
static struct sockaddr_in selfbody = {0};




void* readsocket(void* p)
{
	int ret;
	char buf[256];

	while(alive == 1)
	{
		ret=read(fd, buf, 256);
		if(ret<0)break;

		buf[ret] = 0;
		printf("%s", buf);
		fflush(stdout);
	}
	return 0;
}
int writesocket(char* buf,int len)
{
	int ret;
	ret=write(fd, buf, len);
	return ret;
}




int listsocket()
{
	return 1;
}
int choosesocket(char* addr, int port)
{
	int ret;
	if(addr == 0)
	{
		if(fd > 0)
		{
			alive = 0;

			close(fd);
			fd=0;
		}
	}

	//create struct
	memset(&selfbody, 0, sizeof(struct sockaddr_in));
	selfbody.sin_family=AF_INET;
	selfbody.sin_addr.s_addr=inet_addr(addr);
	selfbody.sin_port=htons(port);

	//create socket
	fd=socket(AF_INET,SOCK_STREAM,0);
	if(fd==-1)
	{
		//printf("socketcreate error\n");
		return 0;
	}

	//connect
	ret=connect(fd,(struct sockaddr*)&selfbody,sizeof(selfbody));
	if(ret<0)
	{
		//printf("connect error\n");
		return 0;
	}

	//thread
	alive = 1;
	pthread_create(&id, NULL, readsocket, NULL);

	//success
	return 1;
}




int startsocket()
{
}
int stopsocket()
{
}
int createsocket()
{
	//do not delete process while receiving SIGPIPE
	struct sigaction sa;
	sa.sa_handler=SIG_IGN;
	sigaction(SIGPIPE,&sa,0);

	return 1;
}
int deletesocket()
{
	stopsocket();
	return 1;
}
