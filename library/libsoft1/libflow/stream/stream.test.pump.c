#include "libsoft.h"




int pump_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int pump_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	int ret;
	u8 tmp[0x1000];

	say("@pump.%4s\n", &foot);
	if(_clk_ == foot){
		//whenever clock
		ret = take_data_from_peer(art,_src_, stack,sp, 0,0, tmp,0x1000);
		ret = give_data_into_peer(art,_dst_, stack,sp, 0,0, tmp,ret);
	}
	return 0;
}
int pump_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int pump_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int pump_create(struct artery* ele, u8* url)
{
	say("@pump_create\n");
	return 1;
}
