#pragma once
#include "data_types.h"

class FrameBuffer {
    public:
    FrameBuffer(int _width, int _height) : 
        width(_width), height(_height),
        color_buffer(std::vector<Color>(_width * _height)),
        depth_buffer(std::vector<float>(_width * _height, 1)) {}
    void DumpAsPPMFile(std::string filename);
    void DumpDepthPPMFile(std::string filename);
    void writeColor(Coord2D coord, Color c);
    void writeDepth(Coord2D coord, float depth);
    float readDepth(Coord2D coord);
    Color readColor(Coord2D coord);
    void update_surface(unsigned char *surface);
    int width;
    int height;
    int max_component_value = 255;
    std::vector<Color> color_buffer;
    std::vector<float> depth_buffer;
    void clear();
};