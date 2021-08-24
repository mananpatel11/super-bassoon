
#import "Cocoa/Cocoa.h"
#include "window.h"
#include <iostream>

static NSAutoreleasePool *g_autoreleasepool = NULL;

static NSWindow *win;
@interface ContentView: NSView
@end

@implementation ContentView {
    Window *w;
}

- (instancetype)initWithWindow:(Window *)window {
    self = [super init];
    if (self != nil) {
        w = window;
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    //NSLog(@"Draw Rect called");
    int width = w->width;
    int height = w->height;
    int bytesPerPixel = 4;
    NSBitmapImageRep *rep = [[[NSBitmapImageRep alloc]
            initWithBitmapDataPlanes:&w->surface
                          pixelsWide:width
                          pixelsHigh:height
                       bitsPerSample:8
                     samplesPerPixel:3
                            hasAlpha:NO
                            isPlanar:NO
                      colorSpaceName:NSCalibratedRGBColorSpace
                         bytesPerRow:width * bytesPerPixel
                        bitsPerPixel:bytesPerPixel*8] autorelease];
    NSImage *nsimage = [[[NSImage alloc] init] autorelease];
    [nsimage addRepresentation:rep];
    [nsimage drawInRect:dirtyRect];
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (void)keyDown:(NSEvent *)event {
    //NSLog(@"Key down event");
    w->handle_key_down([event keyCode]);
}
@end

/* window related functions */

@interface WindowDelegate : NSObject <NSWindowDelegate>
@end

@implementation WindowDelegate {
    Window *_window;
}

- (instancetype)initWithWindow:(Window *)window {
    self = [super init];
    if (self != nil) {
        _window = window;
    }
    return self;
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
    _window->should_close = 1; // This shuts down the app
    return NO;
}

@end

static void create_menubar(void) {
    std::cout << "create_menubar\n";
    NSMenu *menu_bar, *app_menu;
    NSMenuItem *app_menu_item, *quit_menu_item;
    NSString *app_name, *quit_title;

    menu_bar = [[[NSMenu alloc] init] autorelease];
    [NSApp setMainMenu:menu_bar];

    app_menu_item = [[[NSMenuItem alloc] init] autorelease];
    [menu_bar addItem:app_menu_item];

    app_menu = [[[NSMenu alloc] init] autorelease];
    [app_menu_item setSubmenu:app_menu];

    app_name = [[NSProcessInfo processInfo] processName];
    quit_title = [@"Quit " stringByAppendingString:app_name];
    quit_menu_item = [[[NSMenuItem alloc] initWithTitle:quit_title
                                                 action:@selector(terminate:)
                                          keyEquivalent:@"q"] autorelease];
    [app_menu addItem:quit_menu_item];
}

static void create_application() {
    std::cout << "create application\n";
    if (NSApp == nil) {
        g_autoreleasepool = [[NSAutoreleasePool alloc] init];
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        create_menubar();
        [NSApp activateIgnoringOtherApps:YES];
        [NSApp finishLaunching];
    }
}

static NSWindow *create_window(Window *window, int width, int height) {
    std::cout << "create window\n";
    NSRect rect;
    NSUInteger mask;
    NSWindow *handle;
    WindowDelegate *delegate;
    ContentView *view;

    rect = NSMakeRect(0, 0, width, height);
    mask = NSWindowStyleMaskTitled
           | NSWindowStyleMaskClosable
           | NSWindowStyleMaskMiniaturizable;
    handle = [[NSWindow alloc] initWithContentRect:rect
                                         styleMask:mask
                                           backing:NSBackingStoreBuffered
                                             defer:NO];
    assert(handle != nil);
    const char *title = "abc";
    [handle setTitle:[NSString stringWithUTF8String:title]];
    [handle setColorSpace:[NSColorSpace genericRGBColorSpace]];

    /*
     * the storage semantics of NSWindow.setDelegate is @property(assign),
     * or @property(weak) with ARC, we must not autorelease the delegate
     */
    delegate = [[WindowDelegate alloc] initWithWindow:window];
    assert(delegate != nil);
    [handle setDelegate:delegate];

    view = [[[ContentView alloc] initWithWindow:window] autorelease];
    assert(view != nil);
    [handle setContentView:view];
    [handle makeFirstResponder:view];

    return handle;
}

int somefunc();

void Window::create() {
    //somefunc();
    create_application();
    win = create_window(this, width, height);
    std::cout << "Make key\n";
    [win makeKeyAndOrderFront:nil];
}

void Window::destroy() {
    assert(g_autoreleasepool != NULL);
    [g_autoreleasepool drain];
    g_autoreleasepool = [[NSAutoreleasePool alloc] init];
}

void Window::handle_key_down(int virtual_key) {
    std::cout << "Recieved key " << virtual_key << "\n";
    switch (virtual_key) {
        case 0x00: record.left  += 0.1;     break; // a
        case 0x02: record.right += 0.1;     break; // d
        case 0x01: record.down  += 0.1;     break; // s
        case 0x0D: record.up    += 0.1;     break; // w
    }
}

EventRecord Window::process_events() {
    // First flush all events
    while(1) {
            NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                    untilDate:[NSDate distantPast]
                                    inMode:NSDefaultRunLoopMode
                                    dequeue:YES];
            if (event == nil) {
                 break;
            }
            //NSLog(@"Event received");
            //NSLog(@"eventType = %lu", event.type);
            [NSApp sendEvent:event];
    }
    // No idea why this is needed here
    [g_autoreleasepool drain];
    g_autoreleasepool = [[NSAutoreleasePool alloc] init];

    return record;
}

void Window::present() {
    [[win contentView] setNeedsDisplay:YES]; 
}

void Window::reset_record() {
    record.reset();
}

void EventRecord::reset() {
    left = 0;
    right = 0;
    up = 0;
    down = 0;
}

void Window::print_surface() {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int r = (int)surface[4*(y*width + x) + 0];
            int g = (int)surface[4*(y*width + x) + 1];
            int b = (int)surface[4*(y*width + x) + 2];
            int a = (int)surface[4*(y*width + x) + 3];
            std::cout << "(" << r << "," << g << "," << b << "," << a << ")" << "\n";
        }
    }
}