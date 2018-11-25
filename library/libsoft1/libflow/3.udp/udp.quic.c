#include "libsoft.h"




int quicclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int quicclient_read()
{
	return 0;
}
int quicclient_delete(struct element* ele)
{
	return 0;
}
int quicclient_create(struct element* ele, u8* url, u8* buf, int len)
{
	return 0;
}




int quicserver_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int quicserver_read()
{
	return 0;
}
int quicserver_delete(struct element* ele)
{
	return 0;
}
int quicserver_create(struct element* ele, u8* url, u8* buf, int len)
{
	return 0;
}




int quicmaster_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int ret;
	u8* tmp;
	u8 str[30];

	//who
	tmp = pin;
	if(tmp){
		ret = mysnprintf(str, 30,
			"%d.%d.%d.%d:%d",
			tmp[4],tmp[5],tmp[6],tmp[7],
			(tmp[2]<<8)+tmp[3]
		);
	}

	//print
	say("%s\n", str);
	printmemory(buf, len);

	//send back
	if(ret)
	{
		str[ret] = ':';
		str[ret+1] = ' ';
		system_leafwrite(obj,pin,ele,sty,str,ret+2);
	}
	system_leafwrite(obj,pin,ele,sty,buf,len);
	return 0;
}
int quicmaster_read()
{
	return 0;
}
int quicmaster_delete(struct element* ele)
{
	return 0;
}
int quicmaster_create(struct element* ele, u8* url, u8* buf, int len)
{
	int ret;
	void* obj = systemcreate(_UDP_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 0;
}
