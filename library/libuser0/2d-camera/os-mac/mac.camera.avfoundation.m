#include "libuser.h"
#include <AVFoundation/AVFoundation.h>


_obj* thewin = 0;


@interface MyDelegate : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>
{
}
@end

@implementation MyDelegate : NSObject

- (void) captureOutput:(AVCaptureOutput *)captureOutput
 didOutputSampleBuffer:(CMSampleBufferRef)videoFrame
	    fromConnection:(AVCaptureConnection *)connection
{
	//NSLog(@"captureOutput");

	// Get a CMSampleBuffer's Core Video image buffer for the media data
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(videoFrame);
    
    // Lock the base address of the pixel buffer
    CVPixelBufferLockBaseAddress(imageBuffer, 0);
	
    // Get the number of bytes per row for the pixel buffer
    void *baseAddress = CVPixelBufferGetBaseAddress(imageBuffer);
	
    // Get the number of bytes per row for the pixel buffer
	size_t bytesTotal = CVPixelBufferGetDataSize(imageBuffer);
    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer);
    
    // Get the pixel buffer width and height
    size_t width = CVPixelBufferGetWidth(imageBuffer);
    size_t height = CVPixelBufferGetHeight(imageBuffer);

	OSType pixelFormat = CVPixelBufferGetPixelFormatType(imageBuffer);
	//NSLog(@"fmt=%.4s", (char*)&pixelFormat);
    //NSLog(@"w=%zu, h=%zu, bytesPerRow=%zu, bytesTotal=%zu", width, height, bytesPerRow, bytesTotal);

	CMTime timestamp = CMSampleBufferGetPresentationTimeStamp(videoFrame);
    u64 timepkt = (u64)(1000*1000*1000*CMTimeGetSeconds(timestamp));
	u64 timenow = timeread_ns();
	NSLog(@"timepkt=%lld,timenow=%lld,dt=%lld", timepkt, timenow, timenow-timepkt);

	//be careful: mac's yuv2 = actually uyvy
	struct kv88 kv[4] = {
		{'t', 0},
		{ 0 , 0}
	};
	kv[0].val = timepkt;
	if(thewin)give_data_into_peer_temp_stack(thewin,_dst_, (p64)kv,_kv88_, baseAddress,bytesTotal);
    CVPixelBufferUnlockBaseAddress(imageBuffer,0);
}

@end

/*
	// Create a device-dependent RGB color space
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	
    // Create a bitmap graphics context with the sample buffer data
    CGContextRef context = CGBitmapContextCreate(
		baseAddress, 
		width, 
		height, 
		8, 
		bytesPerRow, 
		colorSpace, 
		kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst
	);

    // Create a Quartz image from the pixel data in the bitmap graphics context
    CGImageRef quartzImage = CGBitmapContextCreateImage(context);

    // Unlock the pixel buffer
    CVPixelBufferUnlockBaseAddress(imageBuffer,0);
	
    // Free up the context and color space
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);
	
    // Create an image object from the Quartz image
    //UIImage *image = [UIImage imageWithCGImage:quartzImage];
	UIImage *image = [UIImage imageWithCGImage:quartzImage 
	 			             scale:1.0f 
	     			       orientation:UIImageOrientationRight];
	
    // Release the Quartz image
    CGImageRelease(quartzImage);
*/

/*
//int bufferWidth = CVPixelBufferGetWidth(pixelBuffer);

int yHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer,0);
int uvHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer,1);

int yWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer,0);
int uvWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer,1);

int ybpr = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 0);
int uvbpr = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 1);

int ysize = yHeight * ybpr ;
int uvsize = uvHeight * uvbpr ;

static unsigned char *ypane;
if(!ypane)
   ypane = (unsigned char*)malloc(ysize);
static unsigned char *upane;
if(!upane)
   upane = (unsigned char*)malloc(uvsize);
static unsigned char *vpane;
if(!vpane)
   vpane = (unsigned char*)malloc(uvsize);

unsigned char *yBase = CVPixelBufferGetBaseAddressOfPlane(ypane, 0);
unsigned char *uBase = CVPixelBufferGetBaseAddressOfPlane(upane, 1;
unsigned char *vBase = CVPixelBufferGetBaseAddressOfPlane(vpane, 2);

for(int y=0,y<yHeight;y++)
{
   for(int x=0,x<yWidth;x++)
   {
      ypane[y*yWidth+x]=yBase[y*ybpr+x];
   }
 }

for(int y=0,y<uvHeight;y++)
{
   for(int x=0,x<uvWidth;x++)
   {
      upane[y*uvWidth+x]=uBase[y*uvbpr+x];
      vpane[y*uvWidth+x]=vBase[y*uvbpr+x];
   }
 }
*/



int videodevice_enumfmt(AVCaptureDevice* dev)
{
	for(AVCaptureDeviceFormat* fmt in dev.formats){
        NSLog(@"%@", fmt);
    }
	return 0;
}




int avfcam_attach()
{
	return 0;
}
int avfcam_detach()
{
	return 0;
}
int avfcam_take(_obj* sup,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	return 0;
}
int avfcam_give(_obj* sup,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	return 0;
}




int avfcam_reader()
{
	return 0;
}
int avfcam_writer()
{
	return 0;
}
int avfcam_delete(_obj* win)
{
	return 0;
}
int avfcam_create(_obj* win, void* arg, int argc, u8** argv)
{
	say("avfcam_create\n");
	int j;
	u32 w = 640;
	u32 h = 480;
	u32 fmt = 0;
	for(j=1;j<argc;j++){
		if(0 == ncmp(argv[j], (void*)"log:", 4)){
			//priv->log = 1;
		}
		else if(0 == ncmp(argv[j], (void*)"format:", 7)){
			fmt = *(u32*)(argv[j]+7);
		}
		else if(0 == ncmp(argv[j], (void*)"width:", 6)){
			decstr2u32(argv[j]+6, &w);
		}
		else if(0 == ncmp(argv[j], (void*)"height:", 7)){
			decstr2u32(argv[j]+7, &h);
		}
	}
	say((void*)"libcam_create fmt=%x,w=%d,h=%d\n", fmt, w, h);

	//authorization
	switch([AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo]){
	case AVAuthorizationStatusAuthorized:
		// Has access
		say("Authorized\n");
		break;
	case AVAuthorizationStatusDenied:
		// No access granted
		say("Denied\n");
		break;
	case AVAuthorizationStatusRestricted:
		// Microphone disabled in settings
		say("Restricted\n");
		break;
	case AVAuthorizationStatusNotDetermined:
		// Didn't request access yet
		say("NotDetermined\n");

//		[AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL granted) {
//		}];
		break;
	default:
		say("default\n");
		break;
	}

/*
	//enumerate: old way
	NSArray* video_devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
	NSArray* audio_devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeAudio];
	NSArray* muxed_devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeMuxed];
*/
	//enumerate: new way
	AVCaptureDeviceDiscoverySession* video_discovery = [AVCaptureDeviceDiscoverySession
		discoverySessionWithDeviceTypes:@[AVCaptureDeviceTypeBuiltInWideAngleCamera]
		mediaType:AVMediaTypeVideo
		position:AVCaptureDevicePositionUnspecified
	];
	AVCaptureDeviceDiscoverySession* audio_discovery = [AVCaptureDeviceDiscoverySession
		discoverySessionWithDeviceTypes:@[AVCaptureDeviceTypeMicrophone]	//AVCaptureDeviceTypeBuiltInMicrophone is deprecated
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

	AVCaptureDevice* chosen = 0;
	for (AVCaptureDevice* device in video_devices) {
		const char *name = [[device localizedName] UTF8String];
		int index        = [video_devices indexOfObject:device];
		say("video[%d] %s\n", index, name);
		videodevice_enumfmt(device);
		chosen = device;
	}
	for (AVCaptureDevice* device in audio_devices) {
		const char *name = [[device localizedName] UTF8String];
		int index        = [audio_devices indexOfObject:device];
		videodevice_enumfmt(device);
		say("audio[%d] %s\n", index, name);
	}/*
	for (AVCaptureDevice* device in muxed_devices) {
		const char *name = [[device localizedName] UTF8String];
		int index        = [muxed_devices indexOfObject:device];
		say("muxed[%d] %s\n", index, name);
	}*/

	//todo: debug
	if(0 == chosen)return 0;
	say("in iterm2 this works, in vscode this abort, i dont know why\n");

	//input
	NSError *err;
	AVCaptureDeviceInput* input = [AVCaptureDeviceInput deviceInputWithDevice:chosen error:&err];
	//say("error code=%d\n",[err code]);

	//output
	AVCaptureVideoDataOutput* output = [[AVCaptureVideoDataOutput alloc]init];
	//say("output\n");

	//output delegate
	id delegate = [[MyDelegate alloc] init];
	dispatch_queue_t queue = dispatch_queue_create("avf_queue", NULL);
	[output setSampleBufferDelegate:delegate queue:queue];

	//session
	AVCaptureSession* session = [[AVCaptureSession alloc] init];
	if((1920==w)&&(1080==h)){
		session.sessionPreset = AVCaptureSessionPreset1280x720;
	}
	else if((1280==w)&&(720==h)){
		session.sessionPreset = AVCaptureSessionPreset1280x720;
	}
	else{
		session.sessionPreset = AVCaptureSessionPreset640x480;
	}
	if([session canAddInput:input]){
		[session addInput:input];
	}
	if([session canAddOutput:output]){
		[session addOutput:output];
	}
	[session startRunning];

	//
	thewin = win;
	return 0;
}
