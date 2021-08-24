#include "framebuffer.h"

void FrameBuffer::writeColor(Coord2D coord, Color c) {
    //std::cout << "Writing color at x = " << coord.x << " y = " << coord.y << "\n";
    color_buffer[coord.y*width + coord.x] = c;
}

void FrameBuffer::DumpAsPPMFile(std::string filename) {
    std::fstream fs(filename, std::ios_base::out);
    if (!fs.is_open()) {
        std::cerr << "Failed to open file " << filename << " for writing\n";
        exit(-1);
    }
    /*
    P3
    256 256 // Width and Height
    255     // Max value of each channel
    r g b r g b r g b...
    */
    fs << "P3\n";
    fs << width << " " << height << "\n";
    fs << max_component_value << "\n";
    for (int y = height-1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            Color c = color_buffer[y*width + x];
            fs << c;
        }
    }
}

Color FrameBuffer::readColor(Coord2D coord) {
    return color_buffer[coord.y*width + coord.x];
}