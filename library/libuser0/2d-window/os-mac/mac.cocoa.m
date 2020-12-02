#import "Cocoa/Cocoa.h"
#import "libuser.h"
int rgbanode_take(void*,void*, void*,int, void*,int, void*,int);
int rgbanode_give(void*,void*, void*,int, void*,int, void*,int);




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
static void stackevent()
{
	struct halfrel stack[16];

	struct event ev;
	ev.why = 'a';
	ev.what = _char_;
	ev.where = (u64)thewnd;

	rgbanode_write(thewnd,0, stack,2, 0,0, &ev,0);
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




@interface MyView : NSView {
}
@end

@implementation MyView
- (id)initWithFrame:(NSRect)rect{
NSLog(@"initWithFrame");
	self = [super initWithFrame:rect];
	if(nil != self){
	}
	return self;
}
- (void)drawRect:(NSRect)rect
{
NSLog(@"drawRect");
	//[[NSColor redColor] setFill];
	//NSRectFill(rect);
	//[super drawRect:rect];

	//Fill pixel buffer with color data
	int width = thewnd->width;
	int height= thewnd->height;
	UInt8* data = thewnd->rgbabuf;

	// Create a CGImage with the pixel data
	NSInteger dataLength = width * height * 4;
	CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, data, dataLength, NULL);
	CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
	CGImageRef image = CGImageCreate(
		width, height, 8, 32, width * 4, colorspace,
		kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast,
		provider, NULL, true, kCGRenderingIntentDefault
	);

	//draw
	CGContextRef ctx = (CGContextRef)[[NSGraphicsContext currentContext] CGContext];
	CGRect renderRect = CGRectMake(0., 0., 1024, 768);
	CGContextDrawImage(ctx, renderRect, image);

	//Clean up
	CGColorSpaceRelease(colorspace);
	CGDataProviderRelease(provider);
	CGImageRelease(image);
}
-(void)mouseDown:(NSEvent *)event{
NSLog(@"myview.mouseDown");
	stackevent();
}
-(void)mouseUp:(NSEvent *)event{
NSLog(@"myview.mouseUp");
}
-(void)keyDown:(NSEvent *)event{
NSLog(@"myview.keyDown");
}
-(void)keyUp:(NSEvent *)event{
NSLog(@"myview.keyUp");
}
@end




MyView* myview;




void window_take(struct supply* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	rgbanode_take(wnd,foot, stack,sp, arg,key, buf,len);

	[myview setNeedsDisplay:YES];

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
void window_give(struct supply* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
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
	//value
	wnd->fmt = _rgba_;
	wnd->vfmt = hex64('r','g','b','a','8','8','8','8');

	wnd->width = 1024;
	wnd->height = 768;

	wnd->fbwidth = 1024*4;
	//wnd->fbheight = 0;

	wnd->rgbabuf = malloc(2048*2048*4);
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
	myview = [[[MyView alloc] initWithFrame:windowRect] autorelease];
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
