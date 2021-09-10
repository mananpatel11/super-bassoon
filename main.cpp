#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <chrono>
#include <math.h>

// External dependencies
#include "json.hpp"
#include "lodepng.h"

// local dependencies
#include "data_types.h"
#include "framebuffer.h"
#include "material.h"
#include "window.h"

using json = nlohmann::json;

/*
    GL_BYTE (5120)
    GL_DOUBLE (5130)
    GL_FALSE (0)
    GL_FLOAT (5126)
    GL_HALF_NV (5121)
    GL_INT (5124)
    GL_SHORT (5122)
    GL_TRUE (1)
    GL_UNSIGNED_BYTE (5121)
    GL_UNSIGNED_INT (5125)
    GL_UNSIGNED_INT64_AMD (35778)
    GL_UNSIGNED_SHORT (5123)
*/

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

std::shared_ptr<Mesh> Mesh::createTriangleMesh() {
    int num_triangles = 1;
    std::vector<float3> vertices;
    float3 v0(-1.0, -1.0, 0.0);
    float3 v1(0.0, 1.0, 0.0);
    float3 v2(1.0, -1.0, 0.0);
    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);

    std::vector<float3> colors;
    float3 c0(1.0, 0.0, 0.0);
    float3 c1(0.0, 1.0, 0.0);
    float3 c2(0.0, 0.0, 1.0);
    colors.push_back(c0);
    colors.push_back(c1);
    colors.push_back(c2);
    return std::make_shared<Mesh>(num_triangles, vertices, colors);
}

std::shared_ptr<Mesh> Mesh::createQuadMesh() {
    int num_triangles = 2;
    std::vector<float3> vertices;
    float3 v0(-0.8, -0.8, 0.0);
    float3 v1(-0.8, 0.8, 0.0);
    float3 v2(0.8, -0.8, 0.0);
    float3 v3(0.8, 0.8, 0.0);
    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v2);
    vertices.push_back(v1);
    vertices.push_back(v3);

    std::vector<float3> colors;
    float3 c0(1.0, 0.0, 0.0);
    float3 c1(0.0, 1.0, 0.0);
    float3 c2(0.0, 0.0, 1.0);
    float3 c3(0.0, 1.0, 1.0);
    colors.push_back(c0);
    colors.push_back(c1);
    colors.push_back(c2);
    colors.push_back(c2);
    colors.push_back(c1);
    colors.push_back(c3);
    return std::make_shared<Mesh>(num_triangles, vertices, colors);
}

std::shared_ptr<Mesh> Mesh::createCubeMesh() {
    std::vector<float3> vertices;

    float3 v00(-0.8, 0.8, -0.8);
    float3 v01(0.8, 0.8, -0.8);
    float3 v02(0.8, -0.8, -0.8);
    float3 v03(-0.8, -0.8, -0.8);
    float3 v10(-0.8, 0.8, 0.8);
    float3 v11(0.8, 0.8, 0.8);
    float3 v12(0.8, -0.8, 0.8);
    float3 v13(-0.8, -0.8, 0.8);
    
    // Front face
    vertices.push_back(v00);
    vertices.push_back(v01);
    vertices.push_back(v02);
    vertices.push_back(v02);
    vertices.push_back(v03);
    vertices.push_back(v00);

    // Back face
    vertices.push_back(v11);
    vertices.push_back(v10);
    vertices.push_back(v13);
    vertices.push_back(v13);
    vertices.push_back(v12);
    vertices.push_back(v11);
    
    // Top face
    vertices.push_back(v00);
    vertices.push_back(v10);
    vertices.push_back(v11);
    vertices.push_back(v11);
    vertices.push_back(v01);
    vertices.push_back(v00);

    // Bottom face
    vertices.push_back(v13);
    vertices.push_back(v03);
    vertices.push_back(v02);
    vertices.push_back(v02);
    vertices.push_back(v12);
    vertices.push_back(v13);
    
    // Left face
    vertices.push_back(v10);
    vertices.push_back(v00);
    vertices.push_back(v03);
    vertices.push_back(v03);
    vertices.push_back(v13);
    vertices.push_back(v10);

    // Right face
    vertices.push_back(v01);
    vertices.push_back(v11);
    vertices.push_back(v12);
    vertices.push_back(v12);
    vertices.push_back(v02);
    vertices.push_back(v01);

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0,5);

    std::vector<float3> colors;
    float3 c0(1.0, 0.0, 0.0);
    float3 c1(0.0, 1.0, 0.0);
    float3 c2(0.0, 0.0, 1.0);
    float3 c3(1.0, 1.0, 0.0);
    float3 c4(0.0, 1.0, 1.0);
    float3 c5(1.0, 0.0, 1.0);
    float3 color_options[6] = {c0, c1, c2, c3, c4, c5};
    int num_faces = 6;
    int num_triangles = num_faces * 2;
    for (int i = 0; i < num_triangles*3; i++) {
        colors.push_back(color_options[distrib(gen)]);
    } 
    return std::make_shared<Mesh>(num_triangles, vertices, colors);
}

class Model {
    public:
    Model() : mesh(nullptr), transform(identity()), material(nullptr) {}
    Model(std::shared_ptr<Mesh> _m, float4x4 _transform = identity()) : mesh(_m), transform(_transform) {}
    std::shared_ptr<Mesh> mesh;
    float4x4 transform;
    std::shared_ptr<Material> material;
    void draw(FrameBuffer &fb, const float4x4 &view_transform, const float4x4 &projection_matrix);
};

struct Varyings {
    float4 position;
    float3 color;
    float2 texture_coord;
    Varyings() = default;
};

Varyings vertex_shader(const float3 &position,
                       const float3 &color,
                       const float2 &texture_coord,
                       const float4x4 &mvp) {
    Varyings vout;
    vout.position.x = position.x;
    vout.position.y = position.y;
    vout.position.z = position.z;
    vout.position.w = 1;
    vout.position = mvp*vout.position;
    vout.position = vout.position/vout.position.w;
    vout.color = color;
    vout.texture_coord = texture_coord;
    return vout;
}

// The position fragment shader receives is interpolated
float3 fragment_shader(const Varyings &frag_in, const std::shared_ptr<Texture> &texture) {
    float2 coord = frag_in.texture_coord;
    float4 c = texture->Sample(coord);
    return float3(c.x, c.y, c.z);
    // return frag_in.color;
}

float edge_function(const float4 &a, const float4 &b, const float4 &c) {
    float w = (a.x - b.x)*(c.y - b.y) - (a.y - b.y)*(c.x - b.x);
    return w;
}

void Model::draw(FrameBuffer &fb, const float4x4 &view_transform, const float4x4 &projection_matrix) {
    float4x4 mvp = projection_matrix*view_transform*transform;
    std::array<Varyings, 3> vertex_outs;
    int depth_test_failure_count = 0;
    for (int i = 0; i < mesh->num_triangles; i++) {    
        float2 bbmin(INFINITY, INFINITY);
        float2 bbmax(-INFINITY, -INFINITY);
        bool use_indices = (mesh->indices.size() != 0);
        for (int vid = 0; vid < 3; vid++) {
            // run vertex shading
            int index = -1;
            if (use_indices) {
                index = mesh->indices[i*3 + vid];
            } else {
                index = i*3 + vid;
            }
            float3 pos_in = mesh->vertices[index];
            float3 color_in = mesh->colors[index];
            float2 texture_coord = mesh->texcoords[index];
            Varyings vertex_out = vertex_shader(pos_in, color_in, texture_coord, mvp);
            if (vertex_out.position.x < bbmin.x) bbmin.x = vertex_out.position.x;
            if (vertex_out.position.y < bbmin.y) bbmin.y = vertex_out.position.y;
            if (vertex_out.position.x > bbmax.x) bbmax.x = vertex_out.position.x;
            if (vertex_out.position.y > bbmax.y) bbmax.y = vertex_out.position.y;
            vertex_outs[vid] = vertex_out;
        }
        //exit(0);

        // TODO: Clipping
        // FIXME:Some missing steps here after vertex shader is run...

        // Rasterize and shade
        // https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
        float4 p;
        for (int x = 0; x < fb.width; x++) {
            p.x = (x * 2.0/fb.width) - 1.0 + (1.0 / fb.width);
            if (p.x < bbmin.x || p.x > bbmax.x) {
                continue;
            }
            for (int y = 0; y < fb.height; y++) {
                p.y = (y * 2.0/fb.height) - 1.0 + (1.0 / fb.height);
                if (p.y < bbmin.y || p.y > bbmax.y) {
                    continue;
                }
                // Check if pixel is covered by triangle
                float area = edge_function(vertex_outs[0].position, vertex_outs[1].position, vertex_outs[2].position);
                float e01 = edge_function(p, vertex_outs[0].position, vertex_outs[1].position);
                float e12 = edge_function(p, vertex_outs[1].position, vertex_outs[2].position);
                float e20 = edge_function(p, vertex_outs[2].position, vertex_outs[0].position);

                // If pixel is covered
                if ((e01 >= 0) && (e12 >= 0) && (e20 >= 0)) {
                    // Compute barycentrics
                    float w2 = e01/area;
                    float w0 = e12/area;
                    float w1 = e20/area;

                    // Compute Z-value
                    float inverse_z = w0/vertex_outs[0].position.z + w1/vertex_outs[1].position.z + w2/vertex_outs[2].position.z;
                    float new_z = 1/inverse_z;
                    
                    
                    // Do depth testing
                    float old_z = fb.readDepth(Coord2D(x, y));
                    if (new_z < old_z) {         
                        fb.writeDepth(Coord2D(x, y), new_z);

                        // Interpolate varyings using barycentrics computed above
                        Varyings varyings;
                        varyings.position = w0*vertex_outs[0].position + w1*vertex_outs[1].position + w2*vertex_outs[2].position;
                        varyings.position.z = new_z;
                        varyings.color = w0*vertex_outs[0].color + w1*vertex_outs[1].color + w2*vertex_outs[2].color;
                        varyings.texture_coord = w0*vertex_outs[0].texture_coord + w1*vertex_outs[1].texture_coord + w2*vertex_outs[2].texture_coord;
                        // Run fragment shader
                        float3 color = fragment_shader(varyings, material->base_color_texture);
                        fb.writeColor(Coord2D(x, y), color);
                    } else {
                        depth_test_failure_count++;
                    }
                }
            }
        }
    }
    std::cout << "Failed depth test " << depth_test_failure_count << " times\n";
}

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

    // Constructors
    //static Scene CreateSceneFromGLTF(std::string filename);
    static Scene CreateTriangleScene();
    static Scene CreateQuadScene();
    static Scene CreateCubeScene();

};

void Scene::update(const Camera &c) {
    float3 eye(cos(c.yaw) * cos(c.pitch), sin(c.pitch), -sin(c.yaw)*cos(c.pitch));
    eye.x = eye.x*2.5;
    eye.y = eye.y*2.5;
    eye.z = eye.z*2.5;
    float3 at(0, 0, 0);
    float3 up(0, 1, 0);
    view_matrix = lookAtMatrix(eye, at, up);
}

Scene Scene::CreateTriangleScene() {
    Scene s;
    std::shared_ptr<Mesh> mesh = Mesh::createTriangleMesh();
    float4x4 model_matrix = scalingMatrix(1.0, 1.0, 1.0);
    std::shared_ptr<Model> model = std::make_shared<Model>(mesh, model_matrix);
    s.models.push_back(model);
    float3 eye(0, 0, -1);
    float3 at(0, 0, 0);
    float3 up(0, 1, 0);
    s.view_matrix = lookAtMatrix(eye, at, up);
    return s;
}

Scene Scene::CreateQuadScene() {
    Scene s;
    std::shared_ptr<Mesh> mesh = Mesh::createQuadMesh();
    float4x4 model_matrix = scalingMatrix(1.0, 1.0, 1.0);
    std::shared_ptr<Model> model = std::make_shared<Model>(mesh, model_matrix);
    s.models.push_back(model);
    float3 eye(0, 0, -1);
    float3 at(0, 0, 0);
    float3 up(0, 1, 0);
    s.view_matrix = lookAtMatrix(eye, at, up);
    return s;
}

Scene Scene::CreateCubeScene() {
    Scene s;
    std::vector<Model> models;
    std::shared_ptr<Mesh> mesh = Mesh::createCubeMesh();
    float4x4 model_matrix = scalingMatrix(0.5, 0.5, 0.5);
    std::shared_ptr<Model> model = std::make_shared<Model>(mesh, model_matrix);
    s.models.push_back(model);
    float3 eye(2, 0, -1);
    float3 at(0, 0, 0);
    float3 up(0, 1, 0);
    s.view_matrix = lookAtMatrix(eye, at, up);
    return s;
}

class Renderer {
    public:
    static void Render(FrameBuffer &fb, Scene &scn);
};

void Renderer::Render(FrameBuffer &fb, Scene &scn) {
    fb.clear(); 
    for (size_t i = 0; i < scn.models.size(); i++) {
        std::shared_ptr<Model> &m = scn.models[i];
        m->draw(fb, scn.view_matrix, scn.projection_matrix);
    }
}

void update_surface(unsigned char *surface, FrameBuffer &fb) {
    for (int x = 0; x < fb.width; x++) {
        for (int y = 0; y < fb.height; y++) {
            int y1 = fb.height-y; // Flip Y
            int x1 = fb.width-x;  // Flip X
            surface[4*(fb.width*y1 + x1) + 0] = fb.readColor(Coord2D(x, y)).r;
            surface[4*(fb.width*y1 + x1) + 1] = fb.readColor(Coord2D(x, y)).g;
            surface[4*(fb.width*y1 + x1) + 2] = fb.readColor(Coord2D(x, y)).b;
            surface[4*(fb.width*y1 + x1) + 3] = 0;
        }
    }
}

template<typename T>
std::vector<T> access_data(const json &j, std::string base_path, int accessor_id) {
    std::vector<T> data;
    auto accessor = j["accessors"][accessor_id];
    auto buffer_view = j["bufferViews"][int(accessor["bufferView"])];
    int buffer_index = int(buffer_view["buffer"]);
    std::string bin_uri = j["buffers"][buffer_index]["uri"];
    std::string bin_path = base_path + bin_uri;
    std::ifstream bin_stream(bin_path, std::ios::binary);
    int count = accessor["count"];
    int buffer_view_byte_offset = (buffer_view["byteOffset"] == nullptr)
                                ? 0 : buffer_view["byteOffset"].get<int>();
    int accessor_byte_offset = (accessor["byteOffset"] == nullptr)
                                ? 0 : accessor["byteOffset"].get<int>();
    int byte_offset = buffer_view_byte_offset + accessor_byte_offset;
    int stride = (buffer_view["byteStride"] == nullptr) 
                                ? sizeof(T) : int(buffer_view["byteStride"]);
    bin_stream.seekg(byte_offset);
    for (int i = 0; i < count; i++) {
        T d;
        bin_stream.read(reinterpret_cast<char*>(&d), sizeof(T));
        data.push_back(d);
        bin_stream.seekg(int(bin_stream.tellg()) - sizeof(T) + stride);
    }
    return data;
}

std::vector<std::shared_ptr<Model>> process_node(const json &j,
                                std::string base_path,
                                int node_id,
                                std::vector<std::shared_ptr<Material>> materials) {
    std::vector<std::shared_ptr<Model>> models;
    json node = j["nodes"][node_id];
    // Handle all children
    if (node["children"] != nullptr) {
        for (int child_id : node["children"]) {
            std::vector<std::shared_ptr<Model>> child_models = process_node(j, base_path, child_id, materials);
            models.insert(models.end(), child_models.begin(), child_models.end());
        }
    }
    
    std::shared_ptr<Model> model(nullptr);
    std::shared_ptr<Mesh> mesh(nullptr);

    // If node is a mesh node, create a model
    if (node["mesh"] != nullptr) {
        int mesh_id = node["mesh"];
        auto mesh_j = j["meshes"][mesh_id];
        std::cout << "Mesh = " << mesh << "\n";
        for (auto primitive : mesh_j["primitives"]) {
            std::cout << "Primitive = " << primitive << "\n";
            auto attributes = primitive["attributes"];
            std::cout << "Attributes = " << attributes << "\n";
            // Get positions
            int position_accessor_id = attributes["POSITION"];
            std::vector<float3> positions_data = access_data<float3>(j, 
                                                base_path, position_accessor_id);
            std::cout << "Got positions data\n";
            mesh = std::make_shared<Mesh>(positions_data.size()/3, positions_data);

            // Get indices
            if (primitive["indices"] != nullptr) {
                std::cout << "Indices are present\n";
                std::vector<uint16_t> indices = access_data<uint16_t>(j, 
                                                base_path, primitive["indices"]);
                mesh->indices = indices;
                // FIXME: Hacking correct num_triangles here
                mesh->num_triangles = mesh->indices.size()/3;
            }

            // Get normals       
            if (attributes["NORMAL"] != nullptr) {
                int normal_id = attributes["NORMAL"];
                auto normals_data = access_data<float3>(j, base_path, normal_id);
                mesh->normals = normals_data;
            }
            // Get TEXCOORD_0
            if (attributes["TEXCOORD_0"] != nullptr) {
                int texcoord_id = attributes["TEXCOORD_0"];
                auto texture_coord_data = access_data<float2>(j, base_path, texcoord_id);
                mesh->texcoords = texture_coord_data;
            }

            model = std::make_shared<Model>(mesh);

            // Get material
            if (primitive["material"] != nullptr) {
                int material_id = primitive["material"].get<int>();
                model->material = materials[material_id];    
            }
            
            // model->transform = scalingMatrix(0.015, 0.015, 0.015);
            models.push_back(model);
            // for (auto &p : positions) {
            //     std::cout << p << "\n";
            // }
        }
    } else if (node["camera"] != nullptr) {
        std::cout << "Not handling camera ATM\n";
        //exit(-1);
    } else if (node["skin"] != nullptr) {
        std::cout << "Not handling skin ATM\n";
    }
    return models;
}

Scene create_scene_from_gltf(const std::string&& base_path, const std::string&& gltf_file_name) {
    json j;
    std::string gltf_path = base_path + gltf_file_name;
    
    std::ifstream gltf(gltf_path);
    gltf >> j;
    Scene scn;

    // Pre-Create all materials
    std::vector<std::shared_ptr<Material>> materials = CreateMaterials(j, base_path);

    auto scenes = j["scenes"];
    auto nodes = j["nodes"];
    for (auto scene : scenes) {
        std::cout << "Scene = " << scene << "\n";
        for (int node_id : scene["nodes"]) {
            std::vector<std::shared_ptr<Model>> models = process_node(j, base_path, node_id, materials);
            scn.models.insert(scn.models.end(), models.begin(), models.end());
        }
    }
    return scn;
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
    
    // Create FrameBuffer
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
        Renderer::Render(fb, scn);
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

int test_triangle() {
    int width = 256;
    int height = 256;

    // Create Scene
    Scene scn = Scene::CreateTriangleScene();

    // Create FrameBuffer
    FrameBuffer fb = FrameBuffer(width, height);

    // Draw Scene into FrameBuffer
    Renderer::Render(fb, scn);

    // Dump FrameBuffer into an image file
    fb.DumpAsPPMFile("triangle.ppm");
    return 0;
}

int test_quad() {
    int width = 256;
    int height = 256;

    // Create Scene
    Scene scn = Scene::CreateQuadScene();

    // Create FrameBuffer
    FrameBuffer fb = FrameBuffer(width, height);

    // Draw Scene into FrameBuffer
    Renderer::Render(fb, scn);

    // Dump FrameBuffer into an image file
    fb.DumpAsPPMFile("quad.ppm");
    return 0;
}

int test_cube() {
    int width = 256;
    int height = 256;

    // Create Scene
    Scene scn = Scene::CreateCubeScene();

    // Create FrameBuffer
    FrameBuffer fb = FrameBuffer(width, height);

    // Draw Scene into FrameBuffer
    Renderer::Render(fb, scn);

    // Dump FrameBuffer into an image file
    fb.DumpAsPPMFile("cube.ppm");
    return 0;
}

int main() {
    //create_scene_from_gltf();
    game_loop();
    //test_triangle();
    //test_quad();
    //test_cube();
} 