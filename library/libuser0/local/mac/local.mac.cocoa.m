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
#import "arena.h"
CGContextRef  cgcxtref;
//int main(int argc, const char * argv[])




void windowread()
{
}
void windowwrite()
{
}
void windowlist()
{
}
void windowchoose()
{
}
void windowstart(struct window* this)
{
	void* data = malloc(2048*1024*4);
	CGColorSpaceRef colorspace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
	cgcxtref = CGBitmapContextCreate(
		data, 512, 512,
		8, 2048,
		colorspace, kCGImageAlphaPremultipliedLast
	);
	CGColorSpaceRelease(colorspace);

	this->width = this->stride = 512;
	this->height = 512;
}
void windowstop()
{
}
int windowcreate()
{
	// Autorelease Pool:
	// Objects declared in this scope will be automatically
	// released at the end of it, when the pool is "drained".
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	// Create a shared app instance.
	// This will createialize the global variable
	// 'NSApp' with the application instance.
	[NSApplication sharedApplication];

	//
	// Create a window:
	//

	// Style flags:
	NSUInteger windowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;

	// Window bounds (x, y, width, height).
	NSRect windowRect = NSMakeRect(0, 0, 512, 512);
	NSWindow * window = [[NSWindow alloc] initWithContentRect:windowRect
		styleMask:windowStyle
		backing:NSBackingStoreBuffered
		defer:NO];
	[window autorelease];

	// Window controller:
	NSWindowController * windowController = [[NSWindowController alloc] initWithWindow:window];
	[windowController autorelease];

	// This will add a simple text view to the window,
	// so we can write a test string on it.
	NSTextView * textView = [[NSTextView alloc] initWithFrame:windowRect];
	[textView autorelease];

	[window setContentView:textView];
	//[textView insertText:@"Hello OSX/Cocoa world!"];

	//TODO: Create app delegate to handle system events.
	//TODO: Create menus (especially Quit!)

	// Show window and run event loop.
	[window orderFrontRegardless];
	[NSApp run];

	[pool drain];

	return 0;
}
void windowdelete()
{
}
