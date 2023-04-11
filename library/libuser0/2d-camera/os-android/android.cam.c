#include "libuser.h"
#include <camera/NdkCameraMetadata.h>
#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraDevice.h>


static void onDisconnect(void* context, ACameraDevice* device)
{
	say("onDisconnect:context=%p,device=%p\n", context, device);
}
static void onError(void* context, ACameraDevice* device, int error)
{
	say("onError:context=%p,device=%p,error=%d\n", context, device, error);
}
static ACameraDevice_stateCallbacks callback = {
	.context = 0,
	.onDisconnected = onDisconnect,
	.onError = onError,
};


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
	camera_status_t ret = ACameraManager_getCameraIdList(camManager, &camList);
	say((void*)"camList=%p\n", camList);

	int j,k;
	for(j=0;j<camList->numCameras;j++){
		const char* id = camList->cameraIds[j];
		say((void*)"camname=%s\n", id);

		ACameraMetadata* meta;
		ret = ACameraManager_getCameraCharacteristics(camManager, id, &meta);

		int32_t tagcnt;
		const uint32_t* tagbuf = 0;
		ACameraMetadata_const_entry entry;
		ret = ACameraMetadata_getAllTags(meta, &tagcnt, &tagbuf);
		for(k=0;k<tagcnt;k++){
			ret = ACameraMetadata_getConstEntry(meta, tagbuf[k], &entry);
			if(ACAMERA_OK != ret)say("%d:tag=%x,ret=%d\n", k, tagbuf[k], ret);
			else say("%d:tag=%x,type=%x,count=%x,ptr=%p\n", k, entry.tag, entry.type, entry.count, entry.data.u8);

			switch(tagbuf[k]){
			case ACAMERA_LENS_FACING:
				say("ACAMERA_LENS_FACING=%d\n", entry.data.u8[0]);
				break;
			case ACAMERA_LENS_FOCUS_RANGE:
				say("ACAMERA_LENS_FOCUS_RANGE=%f,%f\n", entry.data.f[0], entry.data.f[1]);
				break;
			case ACAMERA_LENS_FOCAL_LENGTH:
				say("ACAMERA_LENS_FOCAL_LENGTH=%f\n", entry.data.f[0]);
				break;
			case ACAMERA_LENS_FOCUS_DISTANCE:
				say("ACAMERA_LENS_FOCUS_DISTANCE=%f\n", entry.data.f[0]);
				break;
			case ACAMERA_SENSOR_INFO_PHYSICAL_SIZE:
				say("ACAMERA_SENSOR_INFO_PHYSICAL_SIZE=%f,%f\n", entry.data.f[0], entry.data.f[1]);
				break;
			case ACAMERA_SENSOR_INFO_PIXEL_ARRAY_SIZE:
				say("ACAMERA_SENSOR_INFO_PIXEL_ARRAY_SIZE=%d,%d\n", entry.data.i32[0], entry.data.i32[1]);
				break;
			default:
				break;
			}
		}

		ACameraMetadata_free(meta);
	}

	ACameraDevice* device = 0;
	ret = ACameraManager_openCamera(camManager, camList->cameraIds[0], &callback, &device);
	say("device=%p\n", device);

	ACameraManager_deleteCameraIdList(camList);

	//ret = ACameraManager_delete(camManager);
}
