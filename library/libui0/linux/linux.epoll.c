#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/types.h>
//
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
#define IPADDRESS "0.0.0.0"
#define PORT 2222
#define MAXSIZE 1024
//
void sha1sum(unsigned char* out, const unsigned char* str, int len);
void base64_encode(unsigned char* out,const unsigned char* in,int len);
//
void printmemory(char*,int);
void say(char*,...);




//
static int listenfd;
static int epollfd;
static struct epoll_event epollevent[MAXSIZE];
//
static int clienttype[MAXSIZE];
static int websocket_last;
static int websocket_count;
//
static char* http_response = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
static int http_response_size;
static char http_context[0x100000];
static int http_context_size;
//
static char event_queue[0x100000];
static int event_count=0;
//
static unsigned char buf1[256];
static unsigned char buf2[256];
static unsigned char recvbuf[0x100000];
static unsigned char* sendbuf;



//
static char* GET = 0;
static char* Connection = 0;
static char* Upgrade = 0;
static char* Sec_WebSocket_Key = 0;
static void explainstr(char* buf, int max)
{
	int flag;
	int linehead;

	GET = 0;
	Connection = 0;
	Upgrade = 0;
	Sec_WebSocket_Key = 0;

	linehead = 0;
	while(1)
	{
		if(strncmp(buf+linehead, "GET ", 4) == 0)GET = buf+linehead+4;
		else if(strncmp(buf+linehead, "Connection: ", 12) == 0)Connection = buf+linehead+12;
		else if(strncmp(buf+linehead, "Upgrade: ", 9) == 0)Upgrade = buf+linehead+9;
		else if(strncmp(buf+linehead, "Sec-WebSocket-Key: ", 19) == 0)Sec_WebSocket_Key = buf+linehead+19;

		//eat until next character
		flag=0;
		while(1)
		{
			if(buf[linehead] == 0)
			{
				//printf("[0x0@(%d,%d)]\n",linehead,max);
			}
			else if(buf[linehead] == 0xd)
			{
				flag=1;
				//printf("[0xd@(%d,%d)]\n",linehead,max);
			}
			else if(buf[linehead] == 0xa)
			{
				flag=1;
				//printf("[0xa@(%d,%d)]",linehead,max);
				printf("\n");
			}
			else
			{
				if(flag==0)
				{
					printf("%c", buf[linehead]);
				}
				else break;
			}

			linehead++;
			if(linehead >= max)break;
		}

		if(linehead >= max)break;
	}
	printf("GET@%llx,Connection@%llx,Upgrade@%llx,Sec-WebSocket-Key@%llx\n",
		(u64)GET,
		(u64)Connection,
		(u64)Upgrade,
		(u64)Sec_WebSocket_Key
	);
}
void epoll_del(u32 fd)
{
	int j;
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);

	if(fd < MAXSIZE)
	{
		if(clienttype[fd] == 0x10)
		{
			websocket_count--;
			if(fd == websocket_last)
			{
				for(j=fd-1;j>2;j--)
				{
					if(clienttype[j] == 0x10)
					{
						websocket_last = j;
						break;
					}
				}
			}//less search
		}
		clienttype[fd] = 0;
	}
	close(fd);
	printf("last=%d\n",websocket_last);
}
void epoll_add(u32 fd)
{
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);

	if(fd < MAXSIZE)clienttype[fd] = 1;
}








void windowwrite()
{
	int x,y,z;
	u64 len;
	u8 type;

	if(websocket_count == 0)
	{
		printf("@windowwrite:count=0\n");
		return;
	}

	if(sendbuf[0]==0)
	{
		len=512*512*4;
		type=2;
	}
	else
	{
		len = strlen(sendbuf+0x1000);
		type=1;
	}

	if(len<=125)
	{
		x = 2;
		sendbuf[0x1000-x] = 0x80|type;
		sendbuf[0x1000-x+1] = len;
	}
	else if(len<0xffff)
	{
		x = 4;
		sendbuf[0x1000-x] = 0x80|type;
		sendbuf[0x1000-x+1] = 126;
		sendbuf[0x1000-x+2] = (len>>8)&0xff;
		sendbuf[0x1000-x+3] = len&0xff;
	}
	else
	{
		x = 10;
		sendbuf[0x1000-x] = 0x80|type;
		sendbuf[0x1000-x+1] = 127;
		sendbuf[0x1000-x+2] = (len>>56)&0xff;
		sendbuf[0x1000-x+3] = (len>>48)&0xff;
		sendbuf[0x1000-x+4] = (len>>40)&0xff;
		sendbuf[0x1000-x+5] = (len>>32)&0xff;
		sendbuf[0x1000-x+6] = (len>>24)&0xff;
		sendbuf[0x1000-x+7] = (len>>16)&0xff;
		sendbuf[0x1000-x+8] = (len>>8)&0xff;
		sendbuf[0x1000-x+9] = (len)&0xff;
	}

	//
	for(y=0;y<x+8;y++)printf("%.2x ",sendbuf[0x1000-x+y]);
	printf("\n");
	//printf("%s\n", sendbuf+0x1000);

	for(z=3;z<MAXSIZE;z++)
	{
		if(clienttype[z] == 0x10)
		{
			y = write(z, sendbuf+0x1000-x, len+x);
			if(y <= 0)printf("error@%d\n\n\n\n\n",z);
		}
	}
}
void windowread()
{
}
void windowlist()
{
}
void windowchoose()
{
}








void serve_websocket(int fd, int nread)
{
	int i,j,k,type,masked;
	unsigned char key[4];
	u64 temp;

	printf("[%d]:\n",fd);
	for(k=0;k<nread;k++)printf("%.2x ",recvbuf[k]);
	printf("\n");

	if(websocket_count == 0)
	{
		printf("@serve_websocket:count=0\n");
		return;
	}

	//byte0.bit7
	if((recvbuf[0]&0x80)==0x80)printf("tail,");
	else printf("part,");

	//byte0.[3,0]
	k = recvbuf[0]&0xf;
	if(k==0)
	{
		type=0;
		printf("external,");
	}
	else if(k==1)
	{
		type=1;
		printf("text,");
	}
	else if(k==2)
	{
		type=2;
		printf("binary,");
	}
	else if(k==9)
	{
		printf("ping\n");
		return;
	}
	else if(k==0xa)
	{
		printf("pong\n");
		return;
	}
	else if(k==8)
	{
		printf("[%d]type8->close\n",fd);
		epoll_del(fd);
		return;
	}
	else
	{
		printf("[%d]known->close\n",fd);
		epoll_del(fd);
		return;
	}

	//byte1.bit7
	if( (recvbuf[1]>>7) == 1)
	{
		masked=1;
		printf("masked,");
	}
	else
	{
		masked=0;
		printf("unmasked,");
	}

	//
	k = recvbuf[1]&0x7f;
	if(k==127)
	{
		temp	= ((u64)recvbuf[2]<<56)
			+ ((u64)recvbuf[3]<<48)
			+ ((u64)recvbuf[4]<<40)
			+ ((u64)recvbuf[5]<<32)
			+ ((u64)recvbuf[6]<<24)
			+ ((u64)recvbuf[7]<<16)
			+ ((u64)recvbuf[8]<<8)
			+ recvbuf[9];
		k = 10;
		printf("len=%llx,", temp);
	}
	else if(k==126)
	{
		temp	= (recvbuf[2]<<8)
			+ (recvbuf[3]);
		k = 4;
		printf("len=%llx,", temp);
	}
	else
	{
		temp = k;
		k = 2;
		printf("len=%llx,", temp);
	}

	if(masked != 1)printf("\n");
	else
	{
		*(u32*)key = *(u32*)(recvbuf + k);
		j = k;
		k += 4;
		printf("key=%x\n",*(u32*)key);

		if(type==1)
		{
			recvbuf[0] &= 0x8f;
			recvbuf[1] &= 0x7f;
			for(i=0;i<temp;i++)
			{
				recvbuf[j+i] = recvbuf[i+k] ^ key[i%4];

				event_queue[i] = recvbuf[j+i];
				printf("%c",recvbuf[j+i]);
			}
			event_queue[i] = 0;
			printf("\n");
			//j = write(fd, recvbuf, j+temp);

		}//type=ascii
	}//masked=1

	temp = *(u32*)event_queue;
	if(temp == 0x2064626b)
	{
		temp = atoi(event_queue+4);
		*(u64*)(event_queue+8) = temp;
		*(u64*)(event_queue+0) = 0x64626b;
	}
	else if(temp == 0x72616863)
	{
		temp = atoi(event_queue+5);
		*(u64*)(event_queue+8) = temp;
		*(u32*)(event_queue+4) = 0;
	}
	event_count = 1;
}
void handshake_websocket(int fd)
{
	int j;

	j=0;
	while(1)
	{
		if(Sec_WebSocket_Key[j] == 0xa)
		{
			buf1[j] = 0;
			break;
		}
		if(Sec_WebSocket_Key[j] == 0xd)
		{
			buf1[j] = 0;
			break;
		}

		buf1[j] = Sec_WebSocket_Key[j];
		j++;
	}

	snprintf(buf2, 256,"%s%s", buf1, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	printf("%s\n",buf2);

	sha1sum( buf1, buf2, strlen(buf2) );
	for(j=0;j<20;j++)printf("%.2x",buf1[j]);
	printf("\n");

	base64_encode( buf2 ,buf1, 20 );
	printf("%s\n",buf2);

	snprintf(recvbuf, 0x10000,
		"HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: %s\r\n\r\n",

		buf2
	);
	printf("%s",recvbuf);

	//handshake
	j = write(fd, recvbuf, strlen(recvbuf));

	//
	clienttype[fd] = 0x10;
	websocket_count++;
	if(fd > websocket_last)websocket_last = fd;

	//
	*(u64*)(event_queue+0) = 0xabcdef;
	event_count = 1;
}
void handshake_http(int fd)
{
	int ret = open("42.html",O_RDONLY);
	if(ret <= 0)
	{
		printf("error@html\n");
	}
	http_context_size = read(ret, http_context, 0x100000);
	close(ret);

	ret = write(fd, http_response, strlen(http_response) );
	printf("writing http_response\n");
	ret = write(fd, http_context, http_context_size);
	printf("writing http_context\n");

	epoll_del(fd);
	printf("[%d]done->close\n\n\n\n\n", fd);
}
static void handle_read(int fd)
{
	int nread;

	nread = read(fd, recvbuf, MAXSIZE);
	if(nread>0)
	{
		recvbuf[nread] = 0;

		//这是个websocket包
		if(clienttype[fd] == 0x10)
		{
			serve_websocket(fd, nread);
			return;
		}

		//这是个普通socket
		else
		{
			//printf("[%d]start {\n", fd);
			//printf("%s", recvbuf);
			//printf("}end [%d]\n\n", fd);
			explainstr(recvbuf, nread);

			//普通socket，而且不是GET请求，丢弃
			if(GET == 0)return;

			//可能是http，websocket
			else
			{
				//这是个websocket请求
				if( (Upgrade != 0) && (Sec_WebSocket_Key != 0) )
				{
					handshake_websocket(fd);
				}

				//http请求根
				else if( (GET[0]=='/')&&(GET[1]==' ') )
				{
					handshake_http(fd);
					return;
				}

				//http请求其他
				else
				{
					printf("[%d]ignore->close\n\n\n\n\n", fd);
					epoll_del(fd);
					return;
				}
			}
		}
	}
	else
	{
		if (nread == -1)printf("[%d]read error\n", fd);
		else if (nread == 0)printf("[%d]fd closed\n", fd);

		epoll_del(fd);
	}
}
static void handle_accpet(int listenfd)
{
	int fd;
	struct sockaddr_in cliaddr;

	socklen_t cliaddrlen = sizeof(struct sockaddr_in);
	fd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddrlen);
	if (fd == -1)printf("accept error\n");

	//
	printf("[%d]accept@%s:%d\n",
		fd,
		inet_ntoa(cliaddr.sin_addr),
		cliaddr.sin_port
	);
	if(fd > MAXSIZE)
	{
		printf("[%d]MAXSIZE->close\n\n\n\n\n", fd);
		close(fd);
		return;
	}

	epoll_add(fd);
}
static void handle_events()
{
	int i;
	int fd;
	int num = epoll_wait(epollfd, epollevent, MAXSIZE, -1);

	for (i = 0;i < num;i++)
	{
		if (epollevent[i].events & EPOLLIN)
		{
			fd = epollevent[i].data.fd;

			if(fd == listenfd)
			{
				handle_accpet(listenfd);
			}
			else handle_read(fd);
		}
	}
}








void uievent(char* who, char* type, char* key)
{
	while(1)
	{
		if(event_count > 0)
		{
			*(u64*)type = *(u64*)event_queue;
			*(u64*)key = *(u64*)(event_queue + 8);
			event_count = 0;

			printmemory(event_queue, 16);
			return;
		}

		handle_events();
	}
}
void windowstop()
{
	//only close all client
	//listenfd and epollfd unchanged
}
void windowstart(char* addr, char* pixfmt, int x, int y)
{
	//
	sendbuf = addr;
	snprintf(pixfmt, 5, "html");
}
void windowdelete(int num)
{
	int j;
	for(j=0;j<MAXSIZE;j++)
	{
		if(clienttype[j] != 0)close(j);
	}
	close(listenfd);
	close(epollfd);
}
int windowcreate()
{
	int ret;
	struct sockaddr_in servaddr;
	struct sigaction sa;




	//clean
	for(ret=0;ret<MAXSIZE;ret++)
	{
		clienttype[ret] = 0;
	}
	websocket_count = 0;
	websocket_last = 0;




	//socket
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, IPADDRESS, &servaddr.sin_addr);
	servaddr.sin_port = htons(PORT);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1)
	{
		printf("error@socket\n");
		exit(1);
	}

	ret = 0;
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
	if(ret<0)
	{
		printf("error@setsockopet\n");
		return -1;
	}

	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
		printf("error@bind\n");
		exit(1);
	}
	listen(listenfd, 5);




	//epoll
	epollfd = epoll_create(MAXSIZE);
	epoll_add(listenfd);
	if(listenfd < MAXSIZE)clienttype[listenfd] = 0;




	//do not stop when SIGPIPE
	sa.sa_handler=SIG_IGN;
	sigaction(SIGPIPE,&sa,0);
}
