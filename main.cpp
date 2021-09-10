#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <chrono>
#include <math.h>

// local dependencies
#include "data_types.h"
#include "framebuffer.h"
#include "material.h"
#include "window.h"
#include "mesh.h"
#include "model.h"
#include "scene.h"

void update_surface(unsigned char *surface, FrameBuffer &fb) {
    for (int x = 0; x < fb.width; x++) {
        for (int y = 0; y < fb.height; y++) {
            int y1 = fb.height-y; // Flip Y
            int x1 = x;
            surface[4*(fb.width*y1 + x1) + 0] = fb.readColor(Coord2D(x, y)).r;
            surface[4*(fb.width*y1 + x1) + 1] = fb.readColor(Coord2D(x, y)).g;
            surface[4*(fb.width*y1 + x1) + 2] = fb.readColor(Coord2D(x, y)).b;
            surface[4*(fb.width*y1 + x1) + 3] = 0;
        }
    }
}

void game_loop() {
    int width = 500;
    int height = 500;

    // Create Scene
    Camera cam;

    Scene scn0 = create_scene_from_gltf("glTF-Sample-Models/2.0/BoxTextured/glTF/", "BoxTextured.gltf");
    scn0.models[0]->transform = translationMatrix(-0.5, 0.0, 0.0)*scalingMatrix(1.0, 1.0, 1.0);
    Scene scn1 = create_scene_from_gltf("glTF-Sample-Models/2.0/Duck/glTF/", "Duck.gltf");
    scn1.models[0]->transform = translationMatrix(0.5, -2.0, 0.0)*scalingMatrix(0.01, 0.01, 0.01);
    Scene scn;
    scn.models.insert(scn.models.end(), scn0.models.begin(), scn0.models.end());
    scn.models.insert(scn.models.end(), scn1.models.begin(), scn1.models.end());
    scn.projection_matrix = perspectiveProjectionMatrix(0.1, 10.0, M_PI_2*3/2, M_PI_2*3/2);
    // scn.projection_matrix = orthographicProjectionMatrix(5.0, 5.0, 0, 5.0);
    
    FrameBuffer fb = FrameBuffer(width, height);

    Window w(width, height);
    w.create();

    auto start_time = std::chrono::system_clock::now();
    auto curr_time = std::chrono::system_clock::now();
    int num_frames = 0;
    while (!w.should_close) {
        // Get keyboard events
        EventRecord e = w.process_events();
        // Send record of events to camera to update
        cam.update(e);
        // update scene using updated camera
        scn.update(cam);
        // render scene
        scn.Render(fb);
        // Update the window surface with new contents from the fb
        update_surface(w.surface, fb);
        // Tell view to present the new surface
        w.present();
        // Reset the record to process new keyboard events
        w.reset_record();
        num_frames++;
        curr_time = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = curr_time - start_time;
        if (elapsed_seconds.count() >= 1) {
            double fps = num_frames/elapsed_seconds.count();
            std::cout << "FPS = " << fps << "\n";
            num_frames = 0;
            start_time = std::chrono::system_clock::now();
        }
    }    
    w.destroy();
}

int main() {
    game_loop();
} 