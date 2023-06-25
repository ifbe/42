#include "libsoft.h"




int flvserver_takeby(_obj* obj,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
    return 0;
}
int flvserver_giveby(_obj* obj,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
    say("%s\n",__FUNCTION__);
    return 0;
}
int flvserver_attach(struct halfrel* self, struct halfrel* peer)
{
    return 0;
}
int flvserver_detach(struct halfrel* self, struct halfrel* peer)
{
    return 0;
}




int flvserver_read(_obj* obj,void* foot, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int flvserver_write(_obj* obj,void* foot,void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int flvserver_create(_obj* obj, void* arg, int argc, u8** argv)
{
    say("%s\n",__FUNCTION__);
	return 0;
}
int flvserver_delete(_obj* ele)
{
	return 0;
}