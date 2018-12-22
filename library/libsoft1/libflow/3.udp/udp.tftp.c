#include "libsoft.h"




int tftpclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int tftpclient_read()
{
	return 0;
}
int tftpclient_delete(struct element* ele)
{
	return 0;
}
int tftpclient_create(struct element* ele, u8* url)
{
	int ret;
	void* obj = systemcreate(_UDP_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 0;
}




int tftpserver_write(
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
int tftpserver_read()
{
	return 0;
}
int tftpserver_delete(struct element* ele)
{
	return 0;
}
int tftpserver_create(struct element* ele, u8* url)
{
	int ret;
	void* obj = systemcreate(_UDP_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 0;
}
