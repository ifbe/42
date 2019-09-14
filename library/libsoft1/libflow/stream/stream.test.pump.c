#include "libsoft.h"




int pump_read(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	return 0;
}
int pump_write(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	int ret;
	u8 tmp[0x1000];

	struct element* ele = (void*)(self->chip);
	if(0 == ele)return 0;

	say("@pump.%4s\n", &self->flag);
	if(_clk_ == self->flag){
		//whenever clock
		ret = relationread(ele, _src_, tmp, 0x1000);
		ret = relationwrite(ele, _dst_, tmp, ret);
	}
	return 0;
}
int pump_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int pump_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int pump_create(struct element* ele, u8* url)
{
	say("@pump_create\n");
	return 1;
}
