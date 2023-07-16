#include "libuser.h"




int codecv_take(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
    say("codecv_take\n");
	return 0;
}
int codecv_give(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
    say("codecv_give\n");
	return 0;
}
int codecv_attach()
{
	return 0;
}
int codecv_detach()
{
	return 0;
}




int codecv_reader(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len)
{
	return 0;
}
int codecv_writer(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len)
{
	return 0;
}
int codecv_delete(_obj* cam)
{
	return 0;
}
int codecv_create(_obj* cam, void* arg, int argc, u8** argv)
{
	return 0;
}

