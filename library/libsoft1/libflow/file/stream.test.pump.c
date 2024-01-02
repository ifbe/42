#include "libsoft.h"
int sleep_us(int);
int copypath(void*,void*);
int openreadclose(void*,int, void*,int);




struct perobj{
	void* buf;
	char path[128];
};




int pump_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int pump_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	int ret;
	u8 tmp[0x1000];

	say("@pump.%4s\n", &foot);
	if(_clk_ == stack[sp-1].foottype){
		//whenever clock
		ret = take_data_from_peer(art,_src_, stack,sp, 0,0, tmp,0x1000);
		ret = give_data_into_peer(art,_dst_, stack,sp, 0,0, tmp,ret);
	}
	return 0;
}
int pump_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int pump_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}


static void funcaddr(_obj* art)
{
	struct halfrel stack[12];
	struct perobj* per = (void*)art->priv_256b;
	per->buf = memoryalloc(0x100000, 4);
	while(1){
		int ret = openreadclose(per->path, 0, per->buf, 0x100000);

		give_data_into_peer(art,_dst_, stack,0, 0,0, per->buf,ret);

		sleep_us(1000*1000);
	}

	memoryfree(per->buf);
	per->buf = 0;
}


int pump_create(_obj* ele, u8* arg)
{
	say("@pump_create:%.16s\n", arg);

	struct perobj* per = (void*)ele->priv_256b;
	copypath(per->path, arg);

	threadcreate(funcaddr, ele);
	return 1;
}
