#include "libuser.h"
#include <media/NdkImage.h>
#include <media/NdkImageReader.h>
#include <camera/NdkCameraMetadata.h>
#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraDevice.h>




void onCaptureFailed(void* context, ACameraCaptureSession* session,
					 ACaptureRequest* request, ACameraCaptureFailure* failure)
{
	logtoall("onCaptureFailed:context=%p,session=%p,request=%p,failure=%p\n", context, session, request, failure);
}
void onCaptureSequenceCompleted(void* context, ACameraCaptureSession* session,
								int sequenceId, int64_t frameNumber)
{
	logtoall("onCaptureSequenceCompleted:context=%p,session=%p,sequenceId=%x,frameNumber=%llx\n", context, session, sequenceId, frameNumber);
}
void onCaptureSequenceAborted(void* context, ACameraCaptureSession* session,
							  int sequenceId)
{
	logtoall("onCaptureCompleted:context=%p,session=%p,sequenceId=%x\n", context, session, sequenceId);
}
void onCaptureCompleted (void* context, ACameraCaptureSession* session,
		ACaptureRequest* request, const ACameraMetadata* result)
{
	logtoall("onCaptureCompleted:context=%p,session=%p,request=%p,result=%p\n", context, session, request, result);
}
static ACameraCaptureSession_captureCallbacks captureCallbacks = {
		.context = 0,
		.onCaptureStarted = 0,
		.onCaptureProgressed = 0,
		.onCaptureCompleted = onCaptureCompleted,
		.onCaptureFailed = onCaptureFailed,
		.onCaptureSequenceCompleted = onCaptureSequenceCompleted,
		.onCaptureSequenceAborted = onCaptureSequenceAborted,
		.onCaptureBufferLost = 0,
};




static void onSessionActive(void* context, ACameraCaptureSession* session)
{
	logtoall("onSessionActive:context=%p,session=%p\n", context, session);
}
static void onSessionReady(void* context, ACameraCaptureSession* session)
{
	logtoall("onSessionReady:context=%p,session=%p\n", context, session);
}
static void onSessionClosed(void* context, ACameraCaptureSession* session)
{
	logtoall("onSessionClosed:context=%p,session=%p\n", context, session);
}
static ACameraCaptureSession_stateCallbacks sessionStateCallbacks = {
		.context = 0,
		.onActive = onSessionActive,
		.onReady = onSessionReady,
		.onClosed = onSessionClosed
};




static void imageCallback(void* context, AImageReader* reader)
{
	logtoall("imageCallback:context=%p,reader=%p\n", context, reader);

	AImage* image = 0;
	int status = AImageReader_acquireNextImage(reader, &image);
	// Check status here ...

	_obj* cam = context;
	struct kv88 kv[4] = {
		{'w', 640},
		{'h', 480},
		{'f', _y4_uv_},
		{'.', 0}
	};

	uint8_t* buf = 0;
	int len = 0;
	int plane;
	for(plane=0;plane<3;plane++){
		AImage_getPlaneData(image, plane, &buf, &len);
		logtoall("plane[%d]:buf=%p,len=%x\n", plane, buf, len);
		printmemory(buf, 16);

		if(0 == plane){
			len = 640*480;
			kv[3].val = 'y';
		}
		if(1 == plane){
			len = 640*480/4;
			kv[3].val = 'u';
		}
		if(2 == plane){
			len = 640*480/4;
			kv[3].val = 'v';
		}
		give_data_into_peer_temp_stack(cam,_dst_, (p64)kv,_kv88_, buf,len);
	}

	AImage_delete(image);
}
static AImageReader_ImageListener listener = {
	.context = 0,
	.onImageAvailable = imageCallback,
};




static void onDisconnect(void* context, ACameraDevice* device)
{
	logtoall("onDisconnect:context=%p,device=%p\n", context, device);
}
static void onError(void* context, ACameraDevice* device, int error)
{
	logtoall("onError:context=%p,device=%p,error=%d\n", context, device, error);
}
static ACameraDevice_stateCallbacks callback = {
	.context = 0,
	.onDisconnected = onDisconnect,
	.onError = onError,
};




void camera_take(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
}
void camera_give(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
}
void camera_attach()
{
}
void camera_detach()
{
}




void camera_reader(_obj* sup,void* foot, p64 arg,int idx, void* buf,int len)
{
}
void camera_writer(_obj* sup,void* foot, p64 arg,int idx, void* buf,int len)
{
}
void camera_delete(_obj* cam)
{
}
void camera_create(_obj* cam, void* arg, int argc, u8** argv)
{
	ACameraManager* camManager = ACameraManager_create();
	logtoall((void*)"camManager=%p\n", camManager);

	ACameraIdList* camList;
	camera_status_t ret = ACameraManager_getCameraIdList(camManager, &camList);
	logtoall((void*)"camList=%p\n", camList);

	int j,k;
	for(j=0;j<camList->numCameras;j++){
		const char* id = camList->cameraIds[j];
		logtoall((void*)"camname=%s\n", id);

		ACameraMetadata* meta;
		ret = ACameraManager_getCameraCharacteristics(camManager, id, &meta);

		int32_t tagcnt;
		const uint32_t* tagbuf = 0;
		ACameraMetadata_const_entry entry;
		ret = ACameraMetadata_getAllTags(meta, &tagcnt, &tagbuf);
		for(k=0;k<tagcnt;k++){
			ret = ACameraMetadata_getConstEntry(meta, tagbuf[k], &entry);
			if(ACAMERA_OK != ret)logtoall("%d:tag=%x,ret=%d\n", k, tagbuf[k], ret);
			else logtoall("%d:tag=%x,type=%x,count=%x,ptr=%p\n", k, entry.tag, entry.type, entry.count, entry.data.u8);

			switch(tagbuf[k]){
			case ACAMERA_LENS_FACING:
				logtoall("ACAMERA_LENS_FACING=%d\n", entry.data.u8[0]);
				break;
			case ACAMERA_LENS_FOCUS_RANGE:
				logtoall("ACAMERA_LENS_FOCUS_RANGE=%f,%f\n", entry.data.f[0], entry.data.f[1]);
				break;
			case ACAMERA_LENS_FOCAL_LENGTH:
				logtoall("ACAMERA_LENS_FOCAL_LENGTH=%f\n", entry.data.f[0]);
				break;
			case ACAMERA_LENS_FOCUS_DISTANCE:
				logtoall("ACAMERA_LENS_FOCUS_DISTANCE=%f\n", entry.data.f[0]);
				break;
			case ACAMERA_SENSOR_INFO_PHYSICAL_SIZE:
				logtoall("ACAMERA_SENSOR_INFO_PHYSICAL_SIZE=%f,%f\n", entry.data.f[0], entry.data.f[1]);
				break;
			case ACAMERA_SENSOR_INFO_PIXEL_ARRAY_SIZE:
				logtoall("ACAMERA_SENSOR_INFO_PIXEL_ARRAY_SIZE=%d,%d\n", entry.data.i32[0], entry.data.i32[1]);
				break;
			default:
				break;
			}
		}

		ACameraMetadata_free(meta);
	}

	ACameraDevice* device = 0;
	ret = ACameraManager_openCamera(camManager, camList->cameraIds[0], &callback, &device);
	logtoall("device=%p\n", device);

	ACameraManager_deleteCameraIdList(camList);



	//AIMAGE_FORMAT_JPEG and AIMAGE_FORMAT_YUV_420_888 are always supported
	AImageReader* reader = 0;
	media_status_t status = AImageReader_new(640, 480, AIMAGE_FORMAT_YUV_420_888, 4, &reader);
	//if (status != AMEDIA_OK)
		// Handle errors here

	listener.context = cam;
	AImageReader_setImageListener(reader, &listener);

	ANativeWindow* nativeWindow;
	AImageReader_getWindow(reader, &nativeWindow);





	// Create session output from ANativeWindow (Surface)
	ACaptureSessionOutput* output = 0;
	ACaptureSessionOutput_create(nativeWindow, &output);

	// Create a container for outputs and add output
	ACaptureSessionOutputContainer* container = 0;
	ACaptureSessionOutputContainer_create(&container);
	ACaptureSessionOutputContainer_add(container, output);

	// Create the session
	ACameraCaptureSession* session = 0;
	ACameraDevice_createCaptureSession(device, container, &sessionStateCallbacks, &session);





	// Create request with preview template - high frame rate has priority
	// over quality post-processing
	ACaptureRequest* request = 0;
	//ACameraDevice_request_template
	ACameraDevice_createCaptureRequest(device, TEMPLATE_PREVIEW, &request);

	// Specify actual surfaces that should be used for output
	ACameraOutputTarget* jpegTarget = 0;
	ANativeWindow_acquire(nativeWindow);
	ACameraOutputTarget_create(nativeWindow, &jpegTarget);
	ACaptureRequest_addTarget(request, jpegTarget);

	// Configure additional parameters here
	uint8_t jpegQuality = 85;
	ACaptureRequest_setEntry_u8(request, ACAMERA_JPEG_QUALITY, 1, &jpegQuality);




	//Capture one image or continuously until you call ACameraCaptureSession_stopRepeating().
	//ACameraCaptureSession_capture(session, &captureCallbacks, 1, &request, 0);
	ACameraCaptureSession_setRepeatingRequest(session, &captureCallbacks, 1, &request, 0);



	//ret = ACameraManager_delete(camManager);
}
