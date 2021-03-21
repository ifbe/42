#include "libuser.h"
#include <AVFoundation/AVFoundation.h>




int videolist()
{
	return 0;
}
int videochoose()
{
	return 0;
}
int video_take(_sup* sup,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int video_give(_sup* sup,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int videostop()
{
	return 0;
}
int videostart()
{
	return 0;
}
int videodelete(struct supply* win)
{
	return 0;
}
int videocreate(struct supply* win)
{
	say("videocreate\n");
/*	switch([AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo]){
	case Authorized:
		// Has access
		say("Denied\n");
		break;
	case Denied:
		// No access granted
		say("Denied\n");
		break;
	case Restricted:
		// Microphone disabled in settings
		say("Restricted\n");
		break;
	case NotDetermined:
		// Didn't request access yet
		say("NotDetermined\n");
		break;
	default:
		say("default\n");
		break;
	}
*/
/*
	//old way
	NSArray* video_devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
	NSArray* audio_devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeAudio];
	NSArray* muxed_devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeMuxed];
*/
	//new way
	AVCaptureDeviceDiscoverySession* video_discovery = [AVCaptureDeviceDiscoverySession
		discoverySessionWithDeviceTypes:@[AVCaptureDeviceTypeBuiltInWideAngleCamera]
		mediaType:AVMediaTypeVideo
		position:AVCaptureDevicePositionUnspecified
	];
	AVCaptureDeviceDiscoverySession* audio_discovery = [AVCaptureDeviceDiscoverySession
		discoverySessionWithDeviceTypes:@[AVCaptureDeviceTypeBuiltInMicrophone]
		mediaType:AVMediaTypeAudio
		position:AVCaptureDevicePositionUnspecified
	];/*
	AVCaptureDeviceDiscoverySession* muxed_discovery = [AVCaptureDeviceDiscoverySession
		discoverySessionWithDeviceTypes:@[AVCaptureDeviceTypeExternalUnknown]
		mediaType:AVMediaTypeMuxed
		position:AVCaptureDevicePositionUnspecified
	];*/

	NSArray* video_devices  = video_discovery.devices;
	NSArray* audio_devices  = audio_discovery.devices;
	//NSArray* muxed_devices  = muxed_discovery.devices;

	for (AVCaptureDevice* device in video_devices) {
		const char *name = [[device localizedName] UTF8String];
		int index        = [video_devices indexOfObject:device];
		say("video[%d] %s\n", index, name);
	}
	for (AVCaptureDevice* device in audio_devices) {
		const char *name = [[device localizedName] UTF8String];
		int index        = [audio_devices indexOfObject:device];
		say("audio[%d] %s\n", index, name);
	}/*
	for (AVCaptureDevice* device in muxed_devices) {
		const char *name = [[device localizedName] UTF8String];
		int index        = [muxed_devices indexOfObject:device];
		say("muxed[%d] %s\n", index, name);
	}*/
	return 0;
}




void initcam()
{
}
void freecam()
{
}
