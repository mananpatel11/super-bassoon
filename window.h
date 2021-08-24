#pragma once
#include <stdlib.h>

struct EventRecord {
    int a;
    void reset();
};

class Window {
public:
    Window(int _width, int _height) : width(_width), height(_height) {
        surface = (unsigned char *)malloc(width*height*4);
    };
    
    // Create the window
    void create();
    // Destroy the window/app etc
    void destroy();
    // Action to perform when key down is pressed
    void handle_key_down();
    // Process all events and create an EventRecord
    EventRecord process_events();
    // Check if window was closed
    //bool is_window_closed();
    // Sets flags required to tell NSView that framebuffer is ready to be displayed
    void present();
    // Boolean indicating if window should close.
    bool should_close = false;
    // Resets the record
    void reset_record();
    void print_surface();
    int width;
    int height;
    EventRecord record;
    unsigned char *surface;
};