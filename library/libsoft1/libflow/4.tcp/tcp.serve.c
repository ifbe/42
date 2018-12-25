#include "libsoft.h"
void serve_gettype(u8* src, u8* dst)
{
	int j;
	for(j=0;j<8;j++)dst[j] = 0;

	if(0 == src)return;
	if('/' != src[0])return;

	for(j=0;j<8;j++)
	{
		if(src[j] <= 0x20)break;
		dst[j] = src[j];
	}
}




int serveclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int serveclient_read()
{
	return 0;
}
int serveclient_delete(struct element* ele)
{
	return 0;
}
int serveclient_create(struct element* ele, u8* url, u8* buf, int len)
{
	int ret;
	void* obj = systemcreate(_tcp_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 1;
}




int serveserver_leafwrite(
	struct element* ele, void* sty,
	struct object* sc, void* sf,
	u8* buf, int len)
{
	return 0;
}
int serveserver_leafread(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int serveserver_rootwrite(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	printmemory(buf,len);
	return 0;
}
int serveserver_rootread(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int serveserver_delete(struct element* ele)
{
	return 0;
}
int serveserver_create(struct element* ele, u8* url)
{
	return 0;
}




int servemaster_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	//0. old && state=serve: search, send, state=serve
	//1. new && GET && /: debug
	//2. new && GET && other: search, send, state=wait
	//3. new && serve: state=serve
	int j, k;
	u64 type, type1;
	u8 tmp[16];

	struct relation* irel;
	u8* GET = 0;
	u8* SERVE = 0;

	//old
	if(obj->orel0)
	{
		printmemory(buf, len);
		if(0 == sty)return 0;

		irel = ele->irel0;
		while(1)
		{
			if(0 == irel)break;

			if(irel->srcchip != (u64)obj)
			{
				type = ((u64)sty) & 0xffffffffffffff00;
				type1 = (irel->dstfoot) & 0xffffffffffffff00;
				if(type == type1)
				{
					system_leafwrite((void*)(irel->srcchip), 0, ele, 0, buf, len);
				}
			}

			irel = samedstnextsrc(irel);
		}
		return 0;
	}

	//parse
	k = 0;
	for(j=0;j<=len;j++)
	{
		if((j<len)&&(0xd != buf[j])&&(0xa != buf[j]))continue;

		//say("%.*s\n", j-k, buf+k);
		if(ncmp(buf+k, "SERVE ", 6) == 0)SERVE = buf+k+6;
		else if(ncmp(buf+k, "GET ", 4) == 0)GET = buf+k+4;

		if(0xa == buf[j+1])j++;
		k = j+1;
	}

	if(SERVE)
	{
		serve_gettype(SERVE, tmp);
		type = *(u64*)tmp;
		relationcreate(ele, (void*)type, _art_, obj, 0, _fd_);
	}
	else if(GET)
	{
		if(0 == ncmp(GET, "/ ", 2))
		{
			j = mysnprintf(buf, 0x1000, "<html><body>");

			irel = ele->irel0;
			while(1)
			{
				if(0 == irel)break;

				j += mysnprintf(buf+j, 0x1000,
					"%llx, %llx, %llx, %llx<br>",
					irel->srcchip, irel->srcfoot, irel->dstchip, irel->dstfoot
				);

				irel = samedstnextsrc(irel);
			}

			j += mysnprintf(buf+j, 0x1000, "</body></html>");

			k = mysnprintf(buf+j, 0x1000,
				"HTTP/1.1 200 OK\r\n"
				"Content-type: text/html\r\n"
				"Content-Length: %d\r\n"
				"\r\n",
				j
			);

			system_leafwrite(obj, 0, ele, 0, buf+j, k);
			system_leafwrite(obj, 0, ele, 0, buf+0, j);
		}
		else
		{
			serve_gettype(GET, tmp);
			type = *(u64*)tmp;

			irel = ele->irel0;
			while(1)
			{
				if(0 == irel)break;

				if(type == irel->dstfoot)
				{
					system_leafwrite((void*)(irel->srcchip), 0, ele, 0, buf, len);
					type = type&0xffffffffffffff00;
					type |= '?';
					relationcreate(ele, (void*)type, _art_, obj, 0, _fd_);
					break;
				}

				irel = samedstnextsrc(irel);
			}
		}
	}
	return 0;
}
int servemaster_read()
{
	return 0;
}
int servemaster_delete(struct element* ele)
{
	return 0;
}
int servemaster_create(struct element* ele, u8* url)
{
	int ret;
	void* obj = systemcreate(_TCP_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 0;
}