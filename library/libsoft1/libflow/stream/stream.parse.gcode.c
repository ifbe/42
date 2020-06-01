#include "libsoft.h"




int gcodeclient_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int gcodeclient_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	switch(foot){
		case _src_:{
			say("%.*s", len, buf);
			break;
		}
		case _clk_:{
			//say("@gcodeclient_write.clk\n");
			give_data_into_peer(art,_src_, stack,sp, 0,0, "M105\n",5);
			break;
		}
	}
	return 0;
}
int gcodeclient_delete(struct artery* ele)
{
	return 0;
}
int gcodeclient_create(struct artery* ele, u8* url)
{
	return 0;
}





int gcodeserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int gcodeserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int gcodeserver_delete(struct artery* ele)
{
	return 0;
}
int gcodeserver_create(struct artery* ele, u8* url)
{
	return 0;
}
