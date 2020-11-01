#include "libsoft.h"




int tftpclient_read(_art* art,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return 0;
}
int tftpclient_write(_art* art,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return 0;
}
int tftpclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tftpclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tftpclient_delete(struct artery* ele)
{
	return 0;
}
int tftpclient_create(struct artery* ele, u8* url)
{
	return 0;
}




int tftpserver_read(_art* art,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return 0;
}
int tftpserver_write(_art* art,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	int ret;
	u8* tmp;
	u8 str[30];

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
		give_data_into_peer(art,_src_, stack,sp, 0,0, str,ret+2);
	}
	give_data_into_peer(art,_src_, stack,sp, 0,0, buf,len);
	return 0;
}
int tftpserver_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tftpserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tftpserver_delete(struct artery* ele)
{
	return 0;
}
int tftpserver_create(struct artery* ele, u8* url)
{
	return 0;
}
