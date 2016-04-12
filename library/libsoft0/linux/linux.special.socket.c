#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
void say(char* , ...);




//
static int selftype=0;
static int selffd=0;
static struct sockaddr_in selfbody={0};
//
static int remotefd[16]={0};
static struct sockaddr_in remote;




int listsocket()
{
	int ret;

	//作为client的时候，列出所有可能的server
	//作为server的时候，列出所有连上的client
	if(selftype<0)
	{
		for(ret=0;ret<16;ret++)
		{
			if(remotefd[ret]!=0)
			{
				printf("remote[%d]=%d\n",ret,remotefd[ret]);
			}
		}
	}

	return 1;
}
int switchsocket()
{
	int ret;
	socklen_t addrlen;

	//作为client的时候，切到其他server
	if(selftype>0)
	{
	}

	//作为server的时候，面对其他client
	if(selftype<0)
	{
		//accept new
		remotefd[0] = accept(selffd, (struct sockaddr*)&remote, &addrlen);
		if(remotefd[0]<=0)
		{
			printf("accept error:%d",remotefd[0]);
			return 0;
		}
		printf("%s:%d\n",inet_ntoa(remote.sin_addr),ntohs(remote.sin_port));

		//non block
                ret=fcntl(remotefd[0],F_GETFL,0);
                if(ret<0)
                {
                        printf("getfl failed\n");
                }
                ret=fcntl(remotefd[0],F_SETFL,ret|O_NONBLOCK);
                if(ret<0)
                {
                        printf("setfl failed\n");
                }
	}

	return 0;
}
int readsocket(char* addr,int size)
{
	int ret;

	if(selftype>0)
	{
		ret=read(selffd,addr,size);
		if(ret<=0)
		{
		}
	}

	if(selftype<0)
	{
		//接收当前地一个连上的人的消息
		ret=read(remotefd[0],addr,size);
		if(ret<=0)
		{
		}
	}

	return ret;
}
int writesocket(char* addr,int size)
{
	int ret;

	if(selftype>0)
	{
		ret=write(selffd,addr,size);
		if(ret<=0)
		{
		}
	}

	if(selftype<0)
	{
		//发给当前第一个连上的人
		ret=write(remotefd[0],addr,size);
		if(ret<=0)
		{
		}
	}

	return ret;
}




int startsocket(char* address,int port)
{
	int ret;

	//check
	if( (port>0) && (port<65536) )
	{
		selftype=1;
	}
	else if( (port>-65536) && (port<0) )
	{
		selftype=-1;
		port=-port;
	}
	else return 0;

	//init struct
	memset(&selfbody, 0, sizeof(struct sockaddr_in));
	selfbody.sin_family=AF_INET;
	selfbody.sin_addr.s_addr=inet_addr(address);
	selfbody.sin_port=htons(port);

	//create socket
	selffd=socket(AF_INET,SOCK_STREAM,0);
	if(selffd==-1)
	{
		//printf("socketcreate error\n");
		return 0;
	}

	//be client
	if(selftype>0)
	{
		//non block
		ret=fcntl(selffd,F_GETFL,0);
		if(ret<0)
		{
			printf("getfl failed\n");
		}
		ret=fcntl(selffd,F_SETFL,ret|O_NONBLOCK);
		if(ret<0)
		{
			printf("setfl failed\n");
		}

		//connect
		ret=connect(selffd,(struct sockaddr*)&selfbody,sizeof(selfbody));
		if(ret<0)
		{
			//printf("connect error\n");
			return 0;
		}
	}

	//or server
	if(selftype<0)
	{
		//
		ret=bind(selffd,(struct sockaddr*)&selfbody,sizeof(struct sockaddr_in));
		if(ret!=0)
		{
			printf("bind error\n");
			return 0;
		}

		//
		ret=listen(selffd,256);
		if(ret!=0)
		{
			printf("listen error\n");
			return 0;
		}
	}

	return 1;
}
int stopsocket()
{
	if(selffd!=0)
	{
		close(selffd);
		selffd=0;
	}
}
int initsocket()
{
	//do not kill process while receiving SIGPIPE
	struct sigaction sa;
	sa.sa_handler=SIG_IGN;
	sigaction(SIGPIPE,&sa,0);

	return 1;
}
int killsocket()
{
	stopsocket();
	return 1;
}
