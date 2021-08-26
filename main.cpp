#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <math.h>
#include <random>

#include "data_types.h"
#include "framebuffer.h"
#include "window.h"

#include "json.hpp"
#include "lodepng.h"

using json = nlohmann::json;

// Mesh represents the geometry of the object in terms of
// vertices/faces/normals/texcoords etc.
class Mesh {
    public:
    Mesh(int _num_triangles, std::vector<float3> _vertices, std::vector<float3> _colors)
        : vertices(_vertices), num_triangles(_num_triangles), colors(_colors) {}
    Mesh(int _num_triangles, std::vector<float3> _vertices) :
        num_triangles(_num_triangles),
        vertices(_vertices), 
        colors(std::vector<float3>(_num_triangles*3, float3(1.0, 0.0, 0.0))) {}
    std::vector<float3> vertices;
    std::vector<float3> colors;
    int num_triangles;
    static Mesh createTriangleMesh();
    static Mesh createQuadMesh();
    static Mesh createCubeMesh();
    static Mesh createOrthographicsCubeMesh();
};

Mesh Mesh::createTriangleMesh() {
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
    return Mesh(1, vertices, colors);
}

Mesh Mesh::createQuadMesh() {
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
    return Mesh(num_triangles, vertices, colors);
}

Mesh Mesh::createCubeMesh() {
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
    return Mesh(num_triangles, vertices, colors);
}

class Model {
    public:
    Model(Mesh _m, float4x4 _transform) : mesh(_m), transform(_transform) {}
    Mesh mesh;
    float4x4 transform;
    void draw(FrameBuffer &fb, float4x4 &view_transform);
};

struct Varyings {
    float4 position;
    float3 color;
    Varyings() = default;
};

Varyings vertex_shader(float3 position, float3 color, float4x4 model_transform, float4x4 view_transform, float4x4 projection_matrix) {
    Varyings vout;
    vout.position.x = position.x;
    vout.position.y = position.y;
    vout.position.z = position.z;
    vout.position.w = 1;
    // std::cout << "Input position" << vout.position << "\n";
    // std::cout << projection_matrix << "\n";
    // std::cout << view_transform << "\n";
    // std::cout << model_transform << "\n";
    vout.position = projection_matrix*view_transform*model_transform*vout.position;
    vout.position = vout.position/vout.position.w;
    std::cout << "Output position" << vout.position << "\n";
    vout.color = color; 
    return vout;
}

// The position fragment shader receives is interpolated
float3 fragment_shader(Varyings frag_in) {
    return frag_in.color;
}

float edge_function(float4 a, float4 b, float4 c) {
    float w = (a.x - b.x)*(c.y - b.y) - (a.y - b.y)*(c.x - b.x);
    return w;
}

void Model::draw(FrameBuffer &fb, float4x4 &view_transform) {
    fb.clear(); // Clear the framebuffer

    float4x4 perspective_matrix = perspectiveProjectionMatrix(0.1, 10.0, M_PI_2*3/2, M_PI_2*3/2);
    float4x4 ortho_matrix = orthographicProjectionMatrix(5.0, 5.0, 0, 5.0);
    // float4x4 projection_matrix = perspective_matrix;
    float4x4 projection_matrix = ortho_matrix;
    for (int i = 0; i < mesh.num_triangles; i++) {
        std::vector<Varyings> vertex_outs;
        for (int vid = 0; vid < 3; vid++) {
            // run vertex shading
            float3 pos_in = mesh.vertices[i*3 + vid];
            float3 color_in = mesh.colors[i*3 + vid];
            std::cout << projection_matrix << "\n";
            Varyings vertex_out = vertex_shader(pos_in, color_in, transform, view_transform, projection_matrix);
            vertex_outs.push_back(vertex_out);
        }
        //exit(0);

        // TODO: Clipping
        // FIXME:Some missing steps here after vertex shader is run...

        // Rasterize and shade
        // https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
        for (int x = 0; x < fb.width; x++) {
            for (int y = 0; y < fb.height; y++) {
                Coord2D pixel(x, y);
                float4 p;
                // Find pixel center
                p.x = (pixel.x * 2.0/fb.width) - 1.0 + (1.0 / fb.width);
                p.y = (pixel.y * 2.0/fb.height) - 1.0 + (1.0 / fb.height);

                //std::cout << "p = (" << p.x << "," << p.y << ")\n"; 

                // Check if pixel is covered by triangle
                float area = edge_function(vertex_outs[0].position, vertex_outs[1].position, vertex_outs[2].position);
                float e01 = edge_function(p, vertex_outs[0].position, vertex_outs[1].position);
                float e12 = edge_function(p, vertex_outs[1].position, vertex_outs[2].position);
                float e20 = edge_function(p, vertex_outs[2].position, vertex_outs[0].position);
                //std::cout << "e01 = " << e01 << "\n";
                //std::cout << "e12 = " << e12 << "\n";
                //std::cout << "e20 = " << e20 << "\n";
                // If pixel is covered
                if ((e01 >= 0) && (e12 >= 0) && (e20 >= 0)) {
                    //std::cout << "p = (" << p.x << "," << p.y << ")\n"; 

                    // Compute barycentrics
                    float w2 = e01/area;
                    float w0 = e12/area;
                    float w1 = e20/area;

                    // Interpolate varyings using barycentrics computed above
                    Varyings varyings;
                    varyings.position = w0*vertex_outs[0].position + w1*vertex_outs[1].position + w2*vertex_outs[2].position;
                    varyings.color = w0*vertex_outs[0].color + w1*vertex_outs[1].color + w2*vertex_outs[2].color;
                    
                    // Run fragment shader
                    float3 color = fragment_shader(varyings);
                    fb.writeColor(pixel, color);
                }
            }
        }
    }
}



struct Camera {
    Camera() : yaw(M_PI_2), pitch(0) {}
    float yaw; // yaw
    float pitch; // pitch
    void update(const EventRecord& record) {
        yaw += record.right - record.left;
        pitch += record.up - record.down;
        std::cout << "hori_angle = " << yaw << "\n"; 
    }
};

class Scene {
    public:
    Scene() : models(std::vector<Model>()), projection_matrix(identity()), view_matrix(identity()) {}
    Scene(std::vector<Model> _models) : models(_models), projection_matrix(identity()), view_matrix(identity()){};
    std::vector<Model> models;
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
    float3 at(0, 0, 0);
    float3 up(0, 1, 0);
    view_matrix = lookAtMatrix(eye, at, up);
}

Scene Scene::CreateTriangleScene() {
    Mesh mesh = Mesh::createTriangleMesh();
    float4x4 model_matrix = scalingMatrix(1.0, 1.0, 1.0);
    Model model = Model(mesh, model_matrix);
    std::vector<Model> models;
    models.push_back(model);
    Scene s(models);
    float3 eye(0, 0, -1);
    float3 at(0, 0, 0);
    float3 up(0, 1, 0);
    s.view_matrix = lookAtMatrix(eye, at, up);
    return s;
}

Scene Scene::CreateQuadScene() {
    Mesh mesh = Mesh::createQuadMesh();
    Model model = Model(mesh, identity());
    std::vector<Model> models;
    models.push_back(model);
    Scene s(models);
    float3 eye(0, 0, -1);
    float3 at(0, 0, 0);
    float3 up(0, 1, 0);
    s.view_matrix = lookAtMatrix(eye, at, up);
    return s;
}

Scene Scene::CreateCubeScene() {
    std::vector<Model> models;
    Mesh mesh = Mesh::createCubeMesh();
    //float4x4 model_matrix = identity();
    float4x4 model_matrix = scalingMatrix(0.5, 0.5, 0.5);
    Model mode = Model(mesh, model_matrix);
    models.push_back(mode);
    Scene s(models);
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
    // std::cout << "Rendering started\n";
    for (int i = 0; i < scn.models.size(); i++) {
        Model &m = scn.models[i];
        m.draw(fb, scn.view_matrix);
    }
    // std::cout << "Rendering ended\n";
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

void test_matrix4x4() {
    float4x4 A = float4x4(float4(1, 2, 3, 4), float4(1, 2, 3, 4), float4(1, 2, 3, 4), float4(1, 2, 3, 4));
    float4x4 B = float4x4(float4(1, 2, 3, 4), float4(1, 2, 3, 4), float4(1, 2, 3, 4), float4(1, 2, 3, 4));
    float4x4 C = A*B;
    std::cout << C;
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

std::vector<uint16_t> access_uint16_data(const json &j, std::string base_path, int accessor_id) {
    std::vector<uint16_t> data;
    auto accessor = j["accessors"][accessor_id];
    auto buffer_view = j["bufferViews"][int(accessor["bufferView"])];
    int buffer_index = int(buffer_view["buffer"]);
    std::string bin_uri = j["buffers"][buffer_index]["uri"];
    std::string bin_path = base_path + bin_uri;
    std::ifstream bin_stream(bin_path, std::ios::binary);
    int count = accessor["count"];
    int byte_offset = int(buffer_view["byteOffset"]) + int(accessor["byteOffset"]);
    int stride = (buffer_view["byteStride"] == nullptr) ? 2 : int(buffer_view["byteStride"]);
    bin_stream.seekg(byte_offset);
    for (int i = 0; i < count; i++) {
        uint16_t ui;
        bin_stream.read(reinterpret_cast<char*>(&ui), sizeof(uint16_t));
        data.push_back(ui);
        // Since we extracted 1 uint16, move back 2 bytes 
        // and move forward by stride
        bin_stream.seekg(int(bin_stream.tellg()) - 2 + stride);
    }
    return data;
}

std::vector<float3> access_float3_data(const json &j, std::string base_path, int accessor_id) {
    std::vector<float3> data;
    auto accessor = j["accessors"][accessor_id];
    auto buffer_view = j["bufferViews"][int(accessor["bufferView"])];
    int buffer_index = int(buffer_view["buffer"]);
    std::string bin_uri = j["buffers"][buffer_index]["uri"];
    std::string bin_path = base_path + bin_uri;
    std::ifstream bin_stream(bin_path, std::ios::binary);
    int count = accessor["count"];
    int byte_offset = int(buffer_view["byteOffset"]) + int(accessor["byteOffset"]);
    int stride = (buffer_view["byteStride"] == nullptr) ? 12 : int(buffer_view["byteStride"]);
    bin_stream.seekg(byte_offset);
    for (int i = 0; i < count; i++) {
        float x;
        float y;
        float z;
        bin_stream.read(reinterpret_cast<char*>(&x), sizeof(float));
        bin_stream.read(reinterpret_cast<char*>(&y), sizeof(float));
        bin_stream.read(reinterpret_cast<char*>(&z), sizeof(float));
        data.push_back(float3(x, y, z));
        // Since we extracted 3 floats, move back 12 bytes 
        // and move forward by stride
        bin_stream.seekg(int(bin_stream.tellg()) - 12 + stride);
    }
    return data;
}

std::vector<Model> process_node(const json &j, std::string base_path, int node_id) {
    std::vector<Model> models;
    json node = j["nodes"][node_id];
    // Handle all children
    if (node["children"] != nullptr) {
        for (int child_id : node["children"]) {
            std::vector<Model> child_models = process_node(j, base_path, child_id);
            models.insert(models.end(), child_models.begin(), child_models.end());
        }
    }
    // If node is a mesh node, create a model
    if (node["mesh"] != nullptr) {
        int mesh_id = node["mesh"];
        auto mesh = j["meshes"][mesh_id];
        std::cout << "Mesh = " << mesh << "\n";
        for (auto primitive : mesh["primitives"]) {
            std::cout << "Primitive = " << primitive << "\n";
            auto attributes = primitive["attributes"];
            std::cout << "Attributes = " << attributes << "\n";
            // Get positions
            int position_accessor_id = attributes["POSITION"];
            std::vector<float3> positions_data = access_float3_data(j, 
                                                base_path, position_accessor_id);
            
            // Get indices if present
            if (primitive["indices"] != nullptr) {
                std::vector<uint16_t> indices = access_uint16_data(j, 
                                                base_path, primitive["indices"]);
                std::vector<float3> positions;
                for (uint16_t index : indices) {
                    std::cout << "index = " << index << "\n";
                    float3 position = positions_data[index];
                    positions.push_back(position);
                }
                Mesh mesh(indices.size()/3, positions);
                Model model(mesh, identity());
                models.push_back(model);
            } else {
                Mesh mesh(positions_data.size()/3, positions_data);
                Model model(mesh, identity());
                models.push_back(model);
            }
            // for (auto &p : positions) {
            //     std::cout << p << "\n";
            // }
        }
    }
    return models;
}

Scene create_scene_from_gltf() {
    json j;
    // std::string base_path = "glTF-Sample-Models/2.0/TriangleWithoutIndices/glTF/";
    // std::string gltf_path = base_path + "TriangleWithoutIndices.gltf";
    
    std::string base_path = "glTF-Sample-Models/2.0/Box/glTF/";
    std::string gltf_path = base_path + "Box.gltf";
    
    std::ifstream gltf(gltf_path);
    gltf >> j;
    Scene scn;

    auto scenes = j["scenes"];
    auto nodes = j["nodes"];
    auto meshes = j["meshes"];
    auto buffers = j["buffers"];
    auto bufferViews = j["bufferViews"];
    auto accessors = j["accessors"];
    for (auto scene : scenes) {
        std::cout << "Scene = " << scene << "\n";
        for (int node_id : scene["nodes"]) {
            std::vector<Model> models = process_node(j, base_path, node_id);
            scn.models.insert(scn.models.end(), models.begin(), models.end());
            // auto node = nodes[node_id];
            // std::cout << "Node = " << node << "\n";

            // for (int mesh_id : node["mesh"]) {
            //     auto mesh = meshes[mesh_id];
            //     std::cout << "Mesh = " << mesh << "\n";
            //     for (auto primitive : mesh["primitives"]) {
            //         std::cout << "Primitive = " << primitive << "\n";
            //         auto attributes = primitive["attributes"];
            //         std::cout << "Attributes = " << attributes << "\n";
            //         // Get positions
            //         int position_accessor_id = attributes["POSITION"];
            //         std::vector<float3> positions = access_float3_data(j, 
            //                                             base_path, position_accessor_id);
                    
            //         Mesh mesh(3, positions);
            //         Model model(mesh, identity());
            //         scn.models.push_back(model);
            //         for (auto &p : positions) {
            //             std::cout << p << "\n";
            //         }
            //     }
            // }
        }
    }
    //exit(0);
    for (auto &v: scn.models[0].mesh.vertices) {
        std::cout << v << "\n";
    }
    // exit(0);
    return scn;
}

void game_loop() {
    int width = 256;
    int height = 256;

    // Create Scene
    Camera cam;
    // Scene scn = Scene::CreateTriangleScene();
    //Scene scn = Scene::CreateCubeScene();
    Scene scn = create_scene_from_gltf();
    // Create FrameBuffer
    FrameBuffer fb = FrameBuffer(width, height);

    Window w(width, height);
    w.create();
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
    }
    
    w.destroy();
}

int main() {
    //create_scene_from_gltf();
    game_loop();
    //test_triangle();
    //test_quad();
    //test_cube();
} 