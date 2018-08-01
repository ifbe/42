/*
 * File: OSXWindow.m
 *
 * Brief:
 *  Creates a OSX/Cocoa application and window without Interface Builder or XCode.
 *
 * Compile with:
 *  cc OSXWindow.m -o OSXWindow -framework Cocoa
 */
#import "Cocoa/Cocoa.h"
#import "libuser.h"




CGContextRef  cgcxtref;




void windowread()
{
}
void windowwrite()
{
	//CGColorSpaceRef colorspace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
	//cgcxtref = CGBitmapContextCreate(
	//	win->buf, 512, 512,
	//	8, 2048,
	//	colorspace, kCGImageAlphaPremultipliedLast
	//);
	//CGColorSpaceRelease(colorspace);
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
void windowdelete(struct arena* w)
{
}
void windowcreate(struct arena* w)
{
	w->fmt = hex64('b','g','r','a','8','8','8','8');

	w->width = w->stride = 512;
	w->height = 512;

	w->buf = malloc(2048*1024*4);

	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	// Create a shared app instance.
	// This will createialize the global variable
	// 'NSApp' with the application instance.
	[NSApplication sharedApplication];

	// Style flags:
	NSUInteger windowStyle = 
		NSWindowStyleMaskTitled |
		NSWindowStyleMaskClosable |
		NSWindowStyleMaskResizable |
		NSWindowStyleMaskMiniaturizable;

	// Window bounds (x, y, width, height).
	NSRect windowRect = NSMakeRect(0, 0, 512, 512);
	NSWindow* window = [
		[NSWindow alloc] initWithContentRect:windowRect
		styleMask:windowStyle
		backing:NSBackingStoreBuffered
		defer:NO];
	[window autorelease];
	[window setTitle:[NSString stringWithUTF8String:"hello world"]];

	// Window controller:
	NSWindowController * windowController = [[NSWindowController alloc] initWithWindow:window];
	[windowController autorelease];

	// This will add a simple text view to the window,
	// so we can write a test string on it.
	NSTextView* textView = [[NSTextView alloc] initWithFrame:windowRect];
	[textView autorelease];

	[window setContentView:textView];
	//[textView insertText:@"Hello OSX/Cocoa world!"];

	//TODO: Create app delegate to handle system events.
	//TODO: Create menus (especially Quit!)

	// Show window and run event loop.
	[window orderFrontRegardless];

	[NSApp run];

	[pool drain];
}




void initwindow()
{
}
void freewindow()
{
}
