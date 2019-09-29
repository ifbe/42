#include "libsoft.h"




int gcodeclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int gcodeclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	switch(self->flag){
		case _src_:{
			say("%.*s", len, buf);
			break;
		}
		case _clk_:{
			say("@gcodeclient_write.clk\n");
			relationwrite(self->pchip, _src_, 0, 0, "M105\n", 5);
			break;
		}
	}
	return 0;
}
int gcodeclient_delete(struct element* ele)
{
	return 0;
}
int gcodeclient_create(struct element* ele, u8* url)
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
int gcodeserver_delete(struct element* ele)
{
	return 0;
}
int gcodeserver_create(struct element* ele, u8* url)
{
	struct object* obj = systemcreate(_file_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, 0, obj, 0, _fd_, 0);
	return 0;
}
