#include "libuser.h"
int v4l2cam_take(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len);
int v4l2cam_give(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len);
int v4l2cam_attach();
int v4l2cam_detach();
int v4l2cam_reader(_obj* cam,void* foot, void* arg,int idx, u8* buf,int len);
int v4l2cam_writer(_obj* cam,void* foot, void* arg,int idx, u8* buf,int len);
int v4l2cam_delete(_obj* cam);
int v4l2cam_create(_obj* cam,void* arg, int argc, u8** argv);
int libcam_take(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len);
int libcam_give(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len);
int libcam_attach();
int libcam_detach();
int libcam_reader(_obj* cam,void* foot, void* arg,int idx, u8* buf,int len);
int libcam_writer(_obj* cam,void* foot, void* arg,int idx, u8* buf,int len);
int libcam_delete(_obj* cam);
int libcam_create(_obj* cam,void* arg, int argc, u8** argv);




int camera_take(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return libcam_take(cam,foot, stack,sp, arg,idx, buf,len);
	return v4l2cam_take(cam,foot, stack,sp, arg,idx, buf,len);
}
int camera_give(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return libcam_give(cam,foot, stack,sp, arg,idx, buf,len);
	return v4l2cam_give(cam,foot, stack,sp, arg,idx, buf,len);
}
int camera_attach()
{
	return libcam_attach();
	return v4l2cam_attach();
}
int camera_detach()
{
	return libcam_detach();
	return v4l2cam_detach();
}




int camera_reader(_obj* cam,void* foot, void* arg,int idx, u8* buf,int len)
{
	return libcam_reader(cam,foot, arg,idx, buf,len);
	return v4l2cam_reader(cam,foot, arg,idx, buf,len);
}
int camera_writer(_obj* cam,void* foot, void* arg,int idx, u8* buf,int len)
{
	return libcam_writer(cam,foot, arg,idx, buf,len);
	return v4l2cam_writer(cam,foot, arg,idx, buf,len);
}
int camera_delete(_obj* cam)
{
	return libcam_delete(cam);
	return v4l2cam_delete(cam);
}
int camera_create(_obj* cam, void* arg, int argc, u8** argv)
{
	return libcam_create(cam, arg, argc, argv);
	return v4l2cam_create(cam, arg, argc, argv);
}




void initcam()
{
}
void freecam()
{
}




