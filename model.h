#pragma once

#include "data_types.h"
#include "material.h"
#include "framebuffer.h"
#include "mesh.h"

class Model {
    public:
    Model() : mesh(nullptr), transform(identity()), material(nullptr) {}
    Model(std::shared_ptr<Mesh> _m, float4x4 _transform = identity()) : mesh(_m), transform(_transform) {}
    std::shared_ptr<Mesh> mesh;
    float4x4 transform;
    std::shared_ptr<Material> material;
    void draw(FrameBuffer &fb, const float4x4 &view_transform, const float4x4 &projection_matrix);
};