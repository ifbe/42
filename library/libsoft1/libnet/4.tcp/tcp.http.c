#include "artery.h"
//
int findzero(void*);
int findhead(void*);
int findtail(void*);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
int openreadclose(void* name, u64 off, void* mem, u64 len);
int openwriteclose(void* name, u64 off, void* mem, u64 len);
int systemwrite(void*,void*,void*,void*,void*,int);
int systemdelete(void*);




int http_write_request(u8* buf, int len, char* url, char* host)
{
	if((url==0)|(url[0]==0)){url="/";}

	return mysnprintf(buf, 100,
		"GET %s HTTP/1.1\r\n"
		"\r\n",
		url
	);
}
int http_help(void* act, void* pin, u8* buf, int len)
{
	char temp[0x1000];
	int j,k = 0;
	u8* GET = 0;
	u8* POST = 0;
	u8* Upgrade = 0;
	u8* Connection = 0;
	for(j=0;j<=len;j++)
	{
		if((j<len)&&(0xd != buf[j])&&(0xa != buf[j]))continue;

		say("%.*s\n", j-k, buf+k);
		if(ncmp(buf+k, "Connection: ", 12) == 0)Connection = buf+k+12;
		else if(ncmp(buf+k, "Upgrade: ", 9) == 0)Upgrade = buf+k+9;
		else if(ncmp(buf+k, "POST ", 5) == 0)POST = buf+k+5;
		else if(ncmp(buf+k, "GET ", 4) == 0)GET = buf+k+4;

		if(0xa == buf[j+1])j++;
		k = j+1;
	}

	//websocket
	if((0 != Connection)&&(0 != Upgrade))
	{
		return hex32('w','s',0,0);
	}
	else if(0 != GET)
	{
		k = mysnprintf(temp,0x1000,
			"HTTP/1.1 200 OK\r\n"
			"Content-type: text/plain\r\n"
			"Content-Length: %d\r\n"
			"\r\n",
			len
		);
		systemwrite(act, pin, 0, 0, temp, k);
		systemwrite(act, pin, 0, 0, buf, len);
		if(0 != Connection)
		{
			if(0 == ncmp(Connection, "keep-alive", 10))return 0;
		}
	}
	else if(0 != POST)
	{
	}

	systemdelete(act);
	return 0;
}




/*
static u8* Connection = 0;
static u8* Upgrade = 0;
static u8* GET = 0;
static u8 indexhtml[] = "/index.html";




int http_write_file(u8* buf, int len, u8* name)
{
	int ret;
	say("file=%s\n",name);

	ret = openreadclose(name, 0, buf, 0x100000);
	if(ret <= 0)
	{
		*(u32*)buf = 0x343034;
		return 3;
	}
	return ret;
}
int http_read(u8* buf, int len)
{
	int ret;
	u8* p = buf;

	GET = 0;
	Connection = 0;
	Upgrade = 0;

	while(1)
	{
		if(ncmp(p, "GET ", 4) == 0)GET = p+4;
		else if(ncmp(p, "Connection: ", 12) == 0)Connection = p+12;
		else if(ncmp(p, "Upgrade: ", 9) == 0)Upgrade = p+9;

		ret = findhead(p);
		if(ret <= 0)break;

		p += ret;
		if(p > buf+len)break;
	}//while

	return 0;
}




#define CHAT 0x54414843
#define chat 0x74616863
#define HTTP 0x50545448
#define http 0x70747468
#define PROXY 0x59584f5250
#define proxy 0x79786f7270
#define WS 0x5357
#define ws 0x7377
int http_check(struct object* obj, int fd, u8* buf, int len)
{
	int ret;

	//check
	http_read(buf, len);
	if( (GET != 0) && (Connection != 0) && (Upgrade != 0) )return WS;
	if( (Connection != 0) && (Upgrade != 0) )return ws;
	if(GET == 0)return CHAT;

	//parse
	if(GET[0] != '/')return 0;
	if(GET[1] <= ' ')GET = indexhtml;
	else
	{
		for(ret=0;ret<256;ret++)
		{
			if(GET[ret] <= 0x20)break;
		}
		GET[ret] = 0;
	}

	//read body
	len = http_write_file(buf, len, GET+1);
	if(len <= 0)goto byebye;
//say("len=%d\n",len);

	//read head	"Content-Length: %d\r\n"
	ret = mysnprintf(buf+len, 0x100000-len,
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/html\r\n"
		"\r\n"
	);
//say("ret=%d\n",ret);

	//send head
	ret = writesocket(fd, 0, buf+len, ret);

	//send body
	ret = writesocket(fd, 0, buf, len);

byebye:
	return 0;
}
int http_client(struct object* obj, int fd, u8* buf, int len)
{
	return 0;
}
int http_server(struct object* obj, int fd, u8* buf, int len)
{
	return 0;
}
int https_client(struct object* obj, int fd, u8* buf, int len)
{
	return 0;
}
int https_server(struct object* obj, int fd, u8* buf, int len)
{
	return 0;
}
*/