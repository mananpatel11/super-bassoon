#include "framebuffer.h"

void FrameBuffer::clear() {
    for (size_t i = 0; i < color_buffer.size() - 1; i++) {
        color_buffer[i] = Color(0, 0, 0);
    }
    for (size_t i = 0; i < depth_buffer.size() - 1; i++) {
        depth_buffer[i] = 1;
    }
}

Color FrameBuffer::readColor(Coord2D coord) {
    return color_buffer[coord.y*width + coord.x];
}

float FrameBuffer::readDepth(Coord2D coord) {
    return depth_buffer[coord.y*width + coord.x];
}

void FrameBuffer::writeColor(Coord2D coord, Color c) {
    color_buffer[coord.y*width + coord.x] = c;
}

void FrameBuffer::writeDepth(Coord2D coord, float depth) {
    depth_buffer[coord.y*width + coord.x] = depth;
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
        fs << "\n";
    }
}

void FrameBuffer::DumpDepthPPMFile(std::string filename) {
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
            float depth = depth_buffer[y*width + x];
            fs << int(depth*255) << " ";
            fs << 0 << " ";
            fs << 0 << " ";
        }
        fs << "\n";
    }
}

void FrameBuffer::update_surface(unsigned char *surface) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int y1 = height-y; // Flip Y
            int x1 = x;
            surface[4*(width*y1 + x1) + 0] = readColor(Coord2D(x, y)).r;
            surface[4*(width*y1 + x1) + 1] = readColor(Coord2D(x, y)).g;
            surface[4*(width*y1 + x1) + 2] = readColor(Coord2D(x, y)).b;
            surface[4*(width*y1 + x1) + 3] = 0;
        }
    }
}