#include "libsoft.h"
int findzero(void*);
int findhead(void*);
int findtail(void*);
int openreadclose(void* name, u64 off, void* mem, u64 len);
int openwriteclose(void* name, u64 off, void* mem, u64 len);
int wsserver_write(void*, void*, void*, void*, void* buf, int len);
int tlsserver_write(void*, void*, void*, void*, void* buf, int len);
int nodetree_read(void*, void*, void*, int);
int nodetree_write(void*, void*, void*, int);




int httpclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int j,k;
	void* dc;
	void* df;
	struct relation* orel;
	if(0 == ele)return 0;
	if(0 == obj)return 0;
	//say("%.*s\n", len, buf);

	if(0 == ele->stage1)
	{
		k = 0;
		for(j=0;j<=len;j++)
		{
			if((j>=len)|(0xa == buf[j])|(0xd == buf[j]))
			{
				say("%.*s\n", j-k, buf+k);
				if(j >= len)break;
				if((0xd == buf[k])&&(0xa == buf[k+1]))
				{
					buf += k+2;
					len -= k+2;
					break;
				}

				if(0xd == buf[j])j++;
				k = j+1;
			}
		}
	}
	ele->stage1 += 1;

	//if no o rel
	orel = ele->orel0;
	if(0 == orel)
	{
		printmemory(buf, len);
		return 0;
	}

	//send to o rel
	while(1)
	{
		if(0 == orel)break;

		dc = (void*)(orel->dstchip);
		df = (void*)(orel->dstfoot);
		if(_win_ == orel->dsttype)
		{
			arenawrite(dc, df, ele, 0, buf, len);
		}
		else if(_act_ == orel->dsttype)
		{
			actorwrite(dc, df, ele, 0, buf, len);
		}

		orel = samesrcnextdst(orel);
	}

	return 0;
}
int httpclient_read()
{
	return 0;
}
int httpclient_delete(struct element* ele)
{
	return 0;
}
int httpclient_create(struct element* ele, u8* url, u8* buf, int len)
{
	int ret;
	void* obj = systemcreate(_tcp_, url);
	if(0 == obj)return 0;

	ret = mysnprintf(buf, 0x1000,
		"GET %s HTTP/1.1\r\n"
		"Host: %s\r\n"
		"\r\n",
		"/", url
	);

	ret = systemwrite(obj, 0, ele, 0, buf, ret);
	if(ret <= 0)return 0;

	ele->type = _http_;
	ele->stage1 = 0;
	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 1;
}




int httpserver_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	printmemory(buf,len);
	return 0;
}
int httpserver_read()
{
	return 0;
}
int httpserver_delete(struct element* ele)
{
	return 0;
}
int httpserver_create(struct element* ele, u8* url, u8* buf, int len)
{
	return 0;
}




int httpmaster_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int j,k,ret;
	u8* GET = 0;
	u8* POST = 0;
	u8* Upgrade = 0;
	u8* Connection = 0;
	struct element* e;

	//https
	if(0x16 == buf[0])
	{
		e = arterycreate(_Tls_, 0);
		if(e)
		{
			relationcreate(e, 0, _art_, obj, 0, _fd_);
			tlsserver_write(e, sty, obj, pin, buf, len);
		}
		return 0;
	}

	//parse
	k = 0;
	for(j=0;j<=len;j++)
	{
		if((j<len)&&(0xd != buf[j])&&(0xa != buf[j]))continue;

		//say("%.*s\n", j-k, buf+k);
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
		e = arterycreate(_Ws_, 0);
		if(e)
		{
			relationcreate(e, 0, _art_, obj, 0, _fd_);
			wsserver_write(e, sty, obj, pin, buf, len);
		}
		return 0;
	}

	//GET / HTTP/1.1
	if(GET)
	{
		if(0 == ncmp(GET, "/favicon.ico", 12))goto byebye;

		if(ele->orel0)
		{
			//read data
			len = nodetree_read(ele, sty, buf, len);
			if(len <= 0)goto byebye;

			//text html?
			ret = mysnprintf(buf+len, 0x1000,
				"HTTP/1.1 200 OK\r\n"
				"Content-type: text/html\r\n"
				"Content-Length: %d\r\n"
				"\r\n",
				len
			);

			//send response
			systemwrite(obj, pin, ele, sty, buf+len, ret);

			//send context
			systemwrite(obj, pin, ele, sty, buf, len);
		}
		else
		{
			//send back
			ret = mysnprintf(buf+len, 0x1000,
				"HTTP/1.1 200 OK\r\n"
				"Content-type: text/plain\r\n"
				"Content-Length: %d\r\n"
				"\r\n",
				len
			);

			//send response
			systemwrite(obj, pin, ele, sty, buf+len, ret);

			//send context
			systemwrite(obj, pin, ele, sty, buf, len);
		}

	}

	//POST / HTTP/1.1
	if(POST)
	{
		say("%.*s\n", len, buf);
		return 0;
	}

	//unknown
	printmemory(buf,len);

byebye:
	//close or not
	if(0 != Connection)
	{
		if(0 == ncmp(Connection, "keep-alive", 10))return 0;
	}
	systemdelete(obj);
	return 0;
}
int httpmaster_read()
{
	return 0;
}
int httpmaster_delete(struct element* ele)
{
	return 0;
}
int httpmaster_create(struct element* ele, u8* url, u8* buf, int len)
{
	int ret;
	void* obj = systemcreate(_TCP_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, obj, 0, _fd_);
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
