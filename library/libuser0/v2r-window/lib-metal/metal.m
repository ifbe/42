#import "Cocoa/Cocoa.h"
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import "libuser.h"
const int uniformBufferCount = 3;




struct supply* thewnd = 0;
static void CreateApplicationMenus(void)
{
	id appName = [[NSProcessInfo processInfo] processName];
	id quitTitle = [@"Quit " stringByAppendingString:appName];
	id quitMenuItem = [[[NSMenuItem alloc]
		initWithTitle:quitTitle
		action:@selector(terminate:)
		keyEquivalent:@"q"] autorelease];

	id appMenu = [[NSMenu new] autorelease];
	[appMenu addItem:quitMenuItem];

	id appMenuItem = [[NSMenuItem new] autorelease];
	[appMenuItem setSubmenu:appMenu];

	id menubar = [[NSMenu new] autorelease];
	[menubar addItem:appMenuItem];
	[NSApp setMainMenu:menubar];
}




@interface MyApplicationDelegate : NSObject <NSApplicationDelegate> {
}
@end

@implementation MyApplicationDelegate : NSObject
- (id)init{
NSLog(@"init MyApplicationDelegate");
	self = [super init];
	if(nil != self){
	}
	return self;
}
- (void)windowDidResize:(NSNotification*)noti{
	NSLog(@"windowDidResize");
}
- (void)applicationWillFinishLaunching:(NSNotification*)noti{
	NSLog(@"applicationWillFinishLaunching");
	CreateApplicationMenus();
}
- (void)applicationDidFinishLaunching:(NSNotification*)noti{
	NSLog(@"applicationDidFinishLaunching");
	[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	[NSApp activateIgnoringOtherApps:YES];
}
- (void)applicationShouldTerminate{
	NSLog(@"applicationShouldTerminate");
}
- (void)applicationWillTerminate{
	NSLog(@"applicationWillTerminate");
}
- (void)applicationDidChangeScreenParameters{
	NSLog(@"applicationDidChangeScreenParameters");
}
- (void)dealloc{
	NSLog(@"free");
	[super dealloc];
}
@end




@interface MyWindowDelegate : NSObject <NSWindowDelegate> {
}
@end

@implementation MyWindowDelegate : NSObject
- (id)init{
NSLog(@"init MyWindowDelegate");
	self = [super init];
	if(nil != self){
	}
	return self;
}
- (void)windowWillBeginSheet:(NSNotification*)noti{
	NSLog(@"windowWillBeginSheet");
}
- (void)windowDidEndSheet:(NSNotification*)noti{
	NSLog(@"windowDidEndSheet");
}
- (void)windowDidResize:(NSNotification*)noti{
	NSLog(@"windowDidResize");
}
- (void)windowWillMiniaturize:(NSNotification*)noti{
	NSLog(@"windowWillMiniaturize");
}
- (void)windowDidMiniaturize:(NSNotification*)noti{
	NSLog(@"windowDidMiniaturize");
}
- (void)windowDidDeminiaturize:(NSNotification*)noti{
	NSLog(@"windowDidDeminiaturize");
}
- (void)windowWillEnterFullScreen:(NSNotification*)noti{
	NSLog(@"windowWillEnterFullScreen");
}
- (void)windowDidEnterFullScreen:(NSNotification*)noti{
	NSLog(@"windowDidEnterFullScreen");
}
- (void)windowWillExitFullScreen:(NSNotification*)noti{
	NSLog(@"windowWillExitFullScreen");
}
- (void)windowDidExitFullScreen:(NSNotification*)noti{
	NSLog(@"windowDidExitFullScreen");
}
- (void)windowWillClose:(NSNotification*)noti{
	NSLog(@"windowWillClose");
	eventwrite(0, 0, 0, 0);
}
-(void)mouseDown:(NSEvent *)event{
NSLog(@"mywindow.mouseDown");
}
-(void)mouseUp:(NSEvent *)event{
NSLog(@"mywindow.mouseUp");
}
-(void)keyDown:(NSEvent *)event{
NSLog(@"mywindow.keyDown");
}
-(void)keyUp:(NSEvent *)event{
NSLog(@"mywindow.keyUp");
}
@end




@interface MyView : MTKView
@end

@implementation MyView {
    id <MTLLibrary> _library;
    id <MTLCommandQueue> _commandQueue;
    id <MTLRenderPipelineState> _pipelineState;
    id <MTLDepthStencilState> _depthState;
    dispatch_semaphore_t _semaphore;
    id <MTLBuffer> _uniformBuffers[uniformBufferCount];
    id <MTLBuffer> _vertexBuffer;
    int uniformBufferIndex;
    long frame;
}
- (id)initWithFrame:(CGRect)inFrame {
NSLog(@"initWithFrame");
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    self = [super initWithFrame:inFrame device:device];
    if (self) {
        [self setup];
    }
    return self;
}
- (void)setup {
NSLog(@"setup");
}
- (void)drawRect:(CGRect)rect
{
NSLog(@"drawRect");
        [super drawRect:rect];
}
-(void)mouseDown:(NSEvent *)event{
NSLog(@"mouseDown");
}
@end




void windowread(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	while(1){
		NSEvent *event = [NSApp
			nextEventMatchingMask:NSEventMaskAny
			untilDate:[NSDate distantPast]
			inMode:NSDefaultRunLoopMode
			dequeue:YES
		];
		if(nil == event)break;

		switch ([event type]) {
		case NSEventTypeLeftMouseDown:
		case NSEventTypeOtherMouseDown:
		case NSEventTypeRightMouseDown:
		case NSEventTypeLeftMouseUp:
		case NSEventTypeOtherMouseUp:
		case NSEventTypeRightMouseUp:
		case NSEventTypeLeftMouseDragged:
		case NSEventTypeRightMouseDragged:
		case NSEventTypeOtherMouseDragged: /* usually middle mouse dragged */
		case NSEventTypeMouseMoved:
		case NSEventTypeScrollWheel:
		case NSEventTypeKeyDown:
		case NSEventTypeKeyUp:
		case NSEventTypeFlagsChanged:
		default:break;
		}
		/* Pass through to NSApp to make sure everything stays in sync */
		[NSApp sendEvent:event];
	}
}
void windowwrite(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
}
void windowlist()
{
}
void windowchoose()
{
}
void windowstart()
{
}
void windowstop()
{
}
void windowdelete(struct supply* w)
{
}
void windowcreate(struct supply* wnd)
{
	wnd->width = 1024;
	wnd->height = 768;

	wnd->fbwidth = 1024;
	wnd->fbheight = 768;

	thewnd = wnd;


	//window
	NSRect windowRect = NSMakeRect(0, 0, 1024, 768);
	NSUInteger windowStyle =
		NSWindowStyleMaskTitled |
		NSWindowStyleMaskClosable |
		NSWindowStyleMaskResizable |
		NSWindowStyleMaskMiniaturizable;
	id window = [[[NSWindow alloc]
		initWithContentRect:windowRect
		styleMask:windowStyle
		backing:NSBackingStoreBuffered
		defer:NO]
	autorelease];
	[window makeKeyAndOrderFront:nil];

	id appName = [[NSProcessInfo processInfo] processName];
	[window setTitle:appName];

	MyWindowDelegate* windowdelegate = [[[MyWindowDelegate alloc] init] autorelease];
	[window setDelegate:windowdelegate];


	//view
	MyView* myview = [[[MyView alloc] initWithFrame:windowRect] autorelease];
	[window setContentView:myview];
}




void initwindow()
{
	//app
	[NSApplication sharedApplication];

	MyApplicationDelegate* applicationdelegate = [[[MyApplicationDelegate alloc] init] autorelease];
	[NSApp setDelegate:applicationdelegate];

	[NSApp finishLaunching];
}
void freewindow()
{
}
