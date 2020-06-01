#include "libsoft.h"




int quicclient_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int quicclient_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int quicclient_delete(struct artery* ele)
{
	return 0;
}
int quicclient_create(struct artery* ele, u8* url)
{
	return 0;
}




int quicserver_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int quicserver_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int quicserver_delete(struct artery* ele)
{
	return 0;
}
int quicserver_create(struct artery* ele, u8* url)
{
	return 0;
}




int quicmaster_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
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
int quicmaster_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int quicmaster_delete(struct artery* ele)
{
	return 0;
}
int quicmaster_create(struct artery* ele, u8* url)
{
	return 0;
}
