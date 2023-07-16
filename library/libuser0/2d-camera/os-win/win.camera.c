#include "libuser.h"
int dshowcam_take(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
int dshowcam_give(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
int dshowcam_attach();
int dshowcam_detach();
int dshowcam_reader(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len);
int dshowcam_writer(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len);
int dshowcam_delete(_obj* cam);
int dshowcam_create(_obj* cam,void* arg, int argc, u8** argv);
int mfcam_take(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
int mfcam_give(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len);
int mfcam_attach();
int mfcam_detach();
int mfcam_reader(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len);
int mfcam_writer(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len);
int mfcam_delete(_obj* cam);
int mfcam_create(_obj* cam,void* arg, int argc, u8** argv);




int camera_take(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	return mfcam_take(cam,foot, stack,sp, arg,idx, buf,len);
	return dshowcam_take(cam,foot, stack,sp, arg,idx, buf,len);
}
int camera_give(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	return mfcam_give(cam,foot, stack,sp, arg,idx, buf,len);
	return dshowcam_give(cam,foot, stack,sp, arg,idx, buf,len);
}
int camera_attach()
{
	return mfcam_attach();
	return dshowcam_attach();
}
int camera_detach()
{
	return mfcam_detach();
	return dshowcam_detach();
}




int camera_reader(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len)
{
	return mfcam_reader(cam,foot, arg,idx, buf,len);
	return dshowcam_reader(cam,foot, arg,idx, buf,len);
}
int camera_writer(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len)
{
	return mfcam_writer(cam,foot, arg,idx, buf,len);
	return dshowcam_writer(cam,foot, arg,idx, buf,len);
}
int camera_delete(_obj* cam)
{
	return mfcam_delete(cam);
	return dshowcam_delete(cam);
}
int camera_create(_obj* cam, void* arg, int argc, u8** argv)
{
	return mfcam_create(cam, arg, argc, argv);
	return dshowcam_create(cam, arg, argc, argv);
}




void initcam()
{
}
void freecam()
{
}




