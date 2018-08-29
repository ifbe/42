#include "libsoft.h"




int tftp_read(u8* buf, int len)
{
	return 0;
}
int tftp_write(u8* buf, int len)
{
	buf[0] = 0xd;
	buf[1] = 0xa;
	buf[2] = 0;
	return 3;
}




#define tftp 0x70746674
#define TFTP 0x50544654
int tftp_client(struct object* obj, int fd, u8* buf, int len)
{
	printmemory(buf, len);
	return 0;
}
int tftp_server(struct object* obj, int fd, u8* buf, int len)
{
	printmemory(buf, len);
	return 0;
}




int chatserver_write(
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
		systemwrite(obj,pin,ele,sty,str,ret+2);
	}
	systemwrite(obj,pin,ele,sty,buf,len);
	return 0;
}
int chatserver_read()
{
	return 0;
}
int chatserver_delete(struct element* ele)
{
	return 0;
}
int chatserver_create(struct element* ele, u8* url, u8* buf, int len)
{
	int ret;
	void* obj = systemcreate(_UDP_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 0;
}
