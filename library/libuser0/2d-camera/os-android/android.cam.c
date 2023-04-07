#include "libuser.h"
#include <camera/NdkCameraMetadata.h>
#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraDevice.h>




void camera_take(_obj* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
}
void camera_give(_obj* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
}
void camera_attach()
{
}
void camera_detach()
{
}
void camera_reader(_obj* sup,void* foot, void* arg,int idx, void* buf,int len)
{
}
void camera_writer(_obj* sup,void* foot, void* arg,int idx, void* buf,int len)
{
}
void camera_delete(_obj* cam)
{
}
void camera_create(_obj* cam, void* arg, int argc, u8** argv)
{
	ACameraManager* camManager = ACameraManager_create();
	say((void*)"camManager=%p\n", camManager);

	ACameraIdList* camList;
	camera_status_t result = ACameraManager_getCameraIdList(camManager, &camList);
	say((void*)"camList=%p\n", camList);

	int j;
	for(j=0;j<camList->numCameras;j++){
		say((void*)"camname=%s\n", camList->cameraIds[j]);
	}
}
