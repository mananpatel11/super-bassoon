#pragma once

#include <random>
#include <chrono>

#include "data_types.h"

// Mesh represents the geometry of the object in terms of
// vertices/faces/normals/texcoords etc.
class Mesh {
    public:
    Mesh(int _num_triangles,
         std::vector<float3> _vertices,
         std::vector<float3> _colors)
        : num_triangles(_num_triangles), vertices(_vertices), colors(_colors) {}
    Mesh(int _num_triangles,
         std::vector<float3> _vertices) :
        num_triangles(_num_triangles),
        vertices(_vertices) 
        {
            colors = std::vector<float3>();
            std::random_device rd;  //Will be used to obtain a seed for the random number engine
            std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
            std::uniform_int_distribution<> distrib(0,5);
            float3 c0(1.0, 0.0, 0.0);
            float3 c1(0.0, 1.0, 0.0);
            float3 c2(0.0, 0.0, 1.0);
            float3 c3(1.0, 1.0, 0.0);
            float3 c4(0.0, 1.0, 1.0);
            float3 c5(1.0, 0.0, 1.0);
            float3 color_options[6] = {c0, c1, c2, c3, c4, c5};
            for (int i = 0; i < _num_triangles*3; i++) {
                colors.push_back(color_options[distrib(gen)]);
            }
        }
    int num_triangles;
    std::vector<uint16_t> indices;
    std::vector<float3> vertices;
    std::vector<float3> normals;
    std::vector<float3> colors;
    std::vector<float2> texcoords;
    static std::shared_ptr<Mesh> createTriangleMesh();
    static std::shared_ptr<Mesh> createQuadMesh();
    static std::shared_ptr<Mesh> createCubeMesh();
    static std::shared_ptr<Mesh> createOrthographicsCubeMesh();
};