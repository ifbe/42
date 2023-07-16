#include "libuser.h"
int gdigrab_take(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
int gdigrab_give(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
int gdigrab_attach();
int gdigrab_detach();
int gdigrab_reader(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len);
int gdigrab_writer(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len);
int gdigrab_delete(_obj* cam);
int gdigrab_create(_obj* cam,void* arg, int argc, u8** argv);




int screencap_take(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	return gdigrab_take(cam,foot, stack,sp, arg,idx, buf,len);
}
int screencap_give(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	return gdigrab_give(cam,foot, stack,sp, arg,idx, buf,len);
}
int screencap_attach()
{
	return gdigrab_attach();
}
int screencap_detach()
{
	return gdigrab_detach();
}




int screencap_reader(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len)
{
	return gdigrab_reader(cam,foot, arg,idx, buf,len);
}
int screencap_writer(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len)
{
	return gdigrab_writer(cam,foot, arg,idx, buf,len);
}
int screencap_delete(_obj* cam)
{
	return gdigrab_delete(cam);
}
int screencap_create(_obj* cam, void* arg, int argc, u8** argv)
{
	return gdigrab_create(cam, arg, argc, argv);
}

