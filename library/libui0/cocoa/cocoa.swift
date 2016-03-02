import Cocoa

NSApplication.sharedApplication()
// 1. Configure as a regular app, appearing in Dock and (possibly) having UI
NSApp.setActivationPolicy(.Regular)

let window = NSWindow(contentRect: NSMakeRect(0, 0, 320, 200),
                      styleMask: NSTitledWindowMask,
                      backing: .Buffered,
                      `defer`: true)
window.orderFrontRegardless()

dispatch_async(dispatch_get_main_queue()) {
    // 2. Make it the active app
    NSApp.activateIgnoringOtherApps(true)
}

NSApp.run()


import Cocoa

class AppController: NSObject, NSApplicationDelegate {
    var mainWindow: NSWindow?

    func applicationDidFinishLaunching(aNotification: NSNotification) {
        let window = NSWindow(contentRect: NSMakeRect(0, 0, 320, 200),
                              styleMask: NSTitledWindowMask | NSClosableWindowMask,
                              backing: NSBackingStoreType.Buffered,
                              `defer`: true)
        window.orderFrontRegardless()
        self.mainWindow = window

        NSApp.activateIgnoringOtherApps(true)
    }

    func applicationShouldTerminateAfterLastWindowClosed(app: NSApplication) -> Bool {
        return true
    }
}

NSApplication.sharedApplication()
NSApp.setActivationPolicy(.Regular)

let controller = AppController()
NSApp.delegate = controller

NSApp.run()
