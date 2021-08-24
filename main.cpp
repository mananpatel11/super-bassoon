#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <math.h>

#include "data_types.h"
#include "framebuffer.h"
#include "window.h"

// Mesh represents the geometry of the object in terms of
// vertices/faces/normals/texcoords etc.
class Mesh {
    public:
    Mesh(int _num_triangles, std::vector<float3> _vertices, std::vector<float3> _colors) : vertices(_vertices), num_triangles(_num_triangles), colors(_colors) {}
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
    // f = front, b = back
    // u = up, d = down
    // l = left, r = right
    float3 v00(-0.8, 0.8, 0.8);
    float3 v01(0.8, 0.8, 0.8);
    float3 v02(0.8, -0.8, 0.8);
    float3 v03(-0.8, -0.8, 0.8);
    float3 v10(-0.8, 0.8, -0.8);
    float3 v11(0.8, 0.8, -0.8);
    float3 v12(0.8, -0.8, -0.8);
    float3 v13(-0.8, -0.8, -0.8);
    
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

    std::vector<float3> colors;
    float3 c0(1.0, 0.0, 0.0);
    float3 c1(0.0, 1.0, 0.0);
    float3 c2(0.0, 0.0, 1.0);
    int num_faces = 6;
    int num_triangles = num_faces * 2;
    for (int i = 0; i < num_triangles; i++) {
        colors.push_back(c0);
        colors.push_back(c1);
        colors.push_back(c2);
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
    // std::cout << "Output position" << vout.position << "\n";
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
    for (int i = 0; i < mesh.num_triangles; i++) {
        std::vector<Varyings> vertex_outs;
        for (int vid = 0; vid < 3; vid++) {
            // run vertex shading
            float3 pos_in = mesh.vertices[i*3 + vid];
            float3 color_in = mesh.colors[i*3 + vid];
            float4x4 perspective_matrix = perspectiveProjectionMatrix(0.01, 1.0, M_PI_2*3/2, M_PI_2*3/2);
            float4x4 ortho_matrix = orthographicProjectionMatrix(5.0, 5.0, 0.1, 5.0);
            float4x4 projection_matrix = perspective_matrix;
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

// https://stackoverflow.com/questions/349050/calculating-a-lookat-matrix
float4x4 lookAtMatrix(float3 eye, float3 at, float3 up) {
    float3 zaxis = normal(at - eye);
    float3 xaxis = normal(cross(up, zaxis));
    float3 yaxis = cross(zaxis, xaxis);

    float4 row0(xaxis.x, xaxis.y, xaxis.z, -dot(xaxis, eye));
    float4 row1(yaxis.x, yaxis.y, yaxis.z, -dot(yaxis, eye));
    float4 row2(zaxis.x, zaxis.y, zaxis.z, -dot(zaxis, eye));
    float4 row3(0, 0, 0, 1);
    float4x4 view_matrix(row0, row1, row2, row3);
    return view_matrix;
}

class Scene {
    public:
    Scene(std::vector<Model> _models) : models(_models), projection_matrix(identity()), view_matrix(identity()){};
    std::vector<Model> models;
    float4x4 projection_matrix;
    float4x4 view_matrix;

    // Constructors
    //static Scene CreateSceneFromGLTF(std::string filename);
    static Scene CreateTriangleScene();
    static Scene CreateQuadScene();
    static Scene CreateCubeScene();
};

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
    float4x4 model_matrix = identity();
    Model mode = Model(mesh, model_matrix);
    models.push_back(mode);
    Scene s(models);
    float3 eye(0, 2, -2);
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
            surface[4*(fb.width*y + x) + 0] = fb.readColor(Coord2D(x, y)).r;
            // surface[4*(fb.width*y + x) + 0] = 255;
            surface[4*(fb.width*y + x) + 1] = fb.readColor(Coord2D(x, y)).g;
            surface[4*(fb.width*y + x) + 2] = fb.readColor(Coord2D(x, y)).b;
            surface[4*(fb.width*y + x) + 3] = 0;
            
        }
    }
}

void game_loop() {
/*
    While window is not closed
        process mouse/keyboard updates
        update the scene based on those updates
        render the scene
        let window know that image has been updated        
*/
/*
    What is a window? It is the abstraction of the GUI window.
    Creating a window means one should see an empty window
    Drawing in the window means the framebuffer contents should be 
    drawn on the window
    Process inputs means all key/mouse events are processed. 
*/
    int width = 256;
    int height = 256;

    // Create Scene
    Scene scn = Scene::CreateTriangleScene();

    // Create FrameBuffer
    FrameBuffer fb = FrameBuffer(width, height);

    // Draw Scene into FrameBuffer
    Renderer::Render(fb, scn);

    Window w(width, height);
    w.create();
    
    while (!w.should_close) {
        //std::cout << "Here\n";
        EventRecord e = w.process_events();
        if (e.a > 0) {
            update_surface(w.surface, fb);
            //w.print_surface();
            w.present();
        }
        // Use the record to update transform
        // update scene and render
        // Reset event record
        w.reset_record();
    }
    
    w.destroy();
}

int main() {
    game_loop();
    test_triangle();
    test_quad();
    test_cube();
} 