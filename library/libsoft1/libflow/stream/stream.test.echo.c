#include "libsoft.h"




int echo_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@echo_read\n");

	float f[10];
	take_data_from_peer(art,_src_, stack,sp, 0,0, f,10);
	return 0;
}
int echo_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@echo_write\n");
	return give_data_into_peer(art,_src_, stack,sp, 0,0, buf,len);
}
int echo_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int echo_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int echo_create(_obj* ele, u8* url)
{
	say("@echo_create\n");
	return 1;
}
