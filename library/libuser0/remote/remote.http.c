#include "arena.h"
#define _TCP_ hex32('T','C','P',0)
#define _WS_ hex32('W','S',0,0)
#define _fd_ hex32('f','d',0,0)
int systemcreate(u64, void*);
void* arenacreate(u64 type, void* addr);
void* systemread(int);
void* relation_write(void*,void*,u64,void*,void*,u64);




int httpclient_write(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin,
	u8* buf, int len)
{
	if(0 == win)return 0;
	if(0 == act)return 0;
	return 0;
}
int httpclient_read()
{
	return 0;
}
int httpclient_delete(struct arena* win)
{
	return 0;
}
int httpclient_create(struct arena* win)
{
	return 0;
}




int httpserver_write(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin,
	u8* buf, int len)
{
	int j,k;
	void* addr;
	u8* GET = 0;
	u8* POST = 0;
	u8* Upgrade = 0;
	u8* Connection = 0;
	if(0 == win)return 0;
	if(0 == act)return 0;

	k = 0;
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

	if(0)
	{
		addr = arenacreate(_WS_, act);
		if(0 == addr)return 0;

		relation_write(addr, 0, _win_, act, 0, _fd_);
	}
	return 0;
}
int httpserver_read()
{
	return 0;
}
int httpserver_delete(struct arena* win)
{
	return 0;
}
int httpserver_create(struct arena* win, void* str)
{
	int fd;
	void* tmp;
	if(0 == str)str = "127.0.0.1:2222";

	fd = systemcreate(_TCP_, str);
	if(fd <= 0)return 0;

	tmp = systemread(fd);
	if(0 == tmp)return 0;

	relation_write(win, 0, _win_, tmp, 0, _fd_);
	say("fd=%x,tmp=%llx,irel=%llx\n",fd,tmp,win->irel);
	return 0;
}