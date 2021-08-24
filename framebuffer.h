#pragma once
#include "data_types.h"

class FrameBuffer {
    public:
    FrameBuffer(int _width, int _height) : width(_width), height(_height), color_buffer(std::vector<Color>(_width * _height)) {}
    void DumpAsPPMFile(std::string filename);
    void writeColor(Coord2D coord, Color c);
    Color readColor(Coord2D coord);
    // void writeDepth(Coord2D coord, uint8_t depth);
    // uint8_t readDepth(Coord2D coord);
    int width;
    int height;
    int max_component_value = 255;
    std::vector<Color> color_buffer;
    void clear(); // clear color_buffer
};