#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "json.hpp"

#include "window.h"
#include "model.h"

struct Camera {
    Camera() : yaw(M_PI_2), pitch(0) {}
    float yaw; // yaw
    float pitch; // pitch
    void update(const EventRecord& record) {
        yaw += record.right - record.left;
        pitch += record.up - record.down;
    }
};

class Scene {
    public:
    Scene() : models(std::vector<std::shared_ptr<Model>>()),
              projection_matrix(identity()),
              view_matrix(identity()) {}
    Scene(std::vector<std::shared_ptr<Model>> _models) : models(_models),
                                                         projection_matrix(identity()),
                                                         view_matrix(identity()){};
    std::vector<std::shared_ptr<Model>> models;
    float4x4 projection_matrix;
    float4x4 view_matrix;
    void update(const Camera &c);
    void Render(FrameBuffer &fb);

    // Constructors
    //static Scene CreateSceneFromGLTF(std::string filename);
    static Scene CreateTriangleScene();
    static Scene CreateQuadScene();
    static Scene CreateCubeScene();
};

Scene create_scene_from_gltf(const std::string&& base_path, const std::string&& gltf_file_name);