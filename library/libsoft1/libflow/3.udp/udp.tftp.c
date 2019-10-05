#include "libsoft.h"




int tftpclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int tftpclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int tftpclient_delete(struct element* ele)
{
	return 0;
}
int tftpclient_create(struct element* ele, u8* url)
{
	return 0;
}




int tftpserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int tftpserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	int ret;
	u8* tmp;
	u8 str[30];
	struct element* ele = self->pchip;

	//who
	tmp = arg;
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
		relationwrite(ele, _src_, 0, 0, str,ret+2);
	}
	relationwrite(ele, _src_, 0, 0, buf,len);
	return 0;
}
int tftpserver_delete(struct element* ele)
{
	return 0;
}
int tftpserver_create(struct element* ele, u8* url)
{
	return 0;
}
