#include "libsoft.h"




int gcodeclient_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int gcodeclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	switch(stack[sp-1].foottype){
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
int gcodeclient_delete(_obj* ele)
{
	return 0;
}
int gcodeclient_create(_obj* ele, u8* arg)
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
int gcodeserver_delete(_obj* ele)
{
	return 0;
}
int gcodeserver_create(_obj* ele, u8* arg)
{
	return 0;
}
