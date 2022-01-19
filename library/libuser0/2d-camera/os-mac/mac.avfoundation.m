#include "libuser.h"
#include <AVFoundation/AVFoundation.h>


_obj* thewin = 0;


@interface MyDelegate : NSObject
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

	//be careful: mac's yuv2 = actually uyvy
	struct halfrel stack[0x80];
	if(thewin)give_data_into_peer(thewin,_dst_, stack,0, 0,_uyvy_, baseAddress,bytesTotal);
    CVPixelBufferUnlockBaseAddress(imageBuffer,0);

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
}
@end




int videodevice_enumfmt(AVCaptureDevice* dev)
{
	for(AVCaptureDeviceFormat* fmt in dev.formats){
        NSLog(@"%@", fmt);
    }
	return 0;
}




int videolist()
{
	return 0;
}
int videochoose()
{
	return 0;
}
int video_take(_obj* sup,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int video_give(_obj* sup,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
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
int videodelete(_obj* win)
{
	return 0;
}
int videocreate(_obj* win)
{
	say("videocreate\n");

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
	session.sessionPreset = AVCaptureSessionPreset640x480;
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




void initcam()
{
}
void freecam()
{
}
