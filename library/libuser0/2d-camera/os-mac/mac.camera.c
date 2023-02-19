#include "libuser.h"
int avfcam_take(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len);
int avfcam_give(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len);
int avfcam_attach();
int avfcam_detach();
int avfcam_reader(_obj* cam,void* foot, void* arg,int idx, u8* buf,int len);
int avfcam_writer(_obj* cam,void* foot, void* arg,int idx, u8* buf,int len);
int avfcam_delete(_obj* cam);
int avfcam_create(_obj* cam,void* arg, int argc, u8** argv);




int camera_take(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return avfcam_take(cam,foot, stack,sp, arg,idx, buf,len);
}
int camera_give(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return avfcam_give(cam,foot, stack,sp, arg,idx, buf,len);
}
int camera_attach()
{
	return avfcam_attach();
}
int camera_detach()
{
	return avfcam_detach();
}




int camera_reader(_obj* cam,void* foot, void* arg,int idx, u8* buf,int len)
{
	return avfcam_reader(cam,foot, arg,idx, buf,len);
}
int camera_writer(_obj* cam,void* foot, void* arg,int idx, u8* buf,int len)
{
	return avfcam_writer(cam,foot, arg,idx, buf,len);
}
int camera_delete(_obj* cam)
{
	return avfcam_delete(cam);
}
int camera_create(_obj* cam, void* arg, int argc, u8** argv)
{
	return avfcam_create(cam, arg, argc, argv);
}




void initcam()
{
}
void freecam()
{
}




