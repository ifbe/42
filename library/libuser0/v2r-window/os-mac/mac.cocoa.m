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
	//	win->rgbabuf, 512, 512,
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
void windowdelete(struct supply* w)
{
}
void windowcreate(struct supply* w)
{
	w->fmt = _rgba_;
	w->vfmt = hex64('b','g','r','a','8','8','8','8');

	w->width = 1024;
	w->height = 768;

	w->fbwidth = 1024*4;
	//w->fbheight = 0;

	w->rgbabuf = malloc(2048*1024*4);

	[NSAutoreleasePool new];
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        id menubar = [[NSMenu new] autorelease];
        id appMenuItem = [[NSMenuItem new] autorelease];
        [menubar addItem:appMenuItem];
        [NSApp setMainMenu:menubar];

        id appMenu = [[NSMenu new] autorelease];
        id appName = [[NSProcessInfo processInfo] processName];
        id quitTitle = [@"Quit " stringByAppendingString:appName];
        id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle
                action:@selector(terminate:) keyEquivalent:@"q"] autorelease];
        [appMenu addItem:quitMenuItem];
        [appMenuItem setSubmenu:appMenu];

	NSUInteger windowStyle =
                NSWindowStyleMaskTitled |
                NSWindowStyleMaskClosable |
                NSWindowStyleMaskResizable |
                NSWindowStyleMaskMiniaturizable;
        NSRect windowRect = NSMakeRect(0, 0, 1024, 768);
        id window = [[[NSWindow alloc]
                initWithContentRect:windowRect
                styleMask:windowStyle
                backing:NSBackingStoreBuffered
                defer:NO]
        autorelease];
        //[window cascadeTopLeftFromPoint:NSMakePoint(20,20)];
        [window setTitle:appName];
        [window makeKeyAndOrderFront:nil];

        NSTextView* textView = [[NSTextView alloc] initWithFrame:windowRect];
        [textView autorelease];
        [window setContentView:textView];
        [textView insertText:@"Hello OSX/Cocoa world!"];

	[NSApp activateIgnoringOtherApps:YES];
        [NSApp run];
}




void initwindow()
{
}
void freewindow()
{
}
