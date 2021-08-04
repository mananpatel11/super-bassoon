#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct float3 {
    float x;
    float y;
    float z;
    float3() = default;
    float3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

float3 operator*(float lhs, float3 rhs) {
    return float3(lhs*rhs.x, lhs*rhs.y, lhs*rhs.z);
}

float3 operator+(float3 lhs, float3 rhs) {
    return float3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

struct float4 {
    float x;
    float y;
    float z;
    float w;
    float4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    Color() : r(0), g(0), b(0) {}
    Color(float3 col) {
        r = col.x * 255;
        g = col.y * 255;
        b = col.z * 255;
    }
    Color(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}
};

struct Coord2D {
    int x;
    int y;
    Coord2D() = default;
    Coord2D(int _x, int _y) : x(_x), y(_y) {}
};

std::ostream& operator<<(std::ostream &os, Color &c) {
    os << uint(c.r) << " " << uint(c.g) << " " << uint(c.b) << " ";
    return os;
}

class FrameBuffer {
    public:
    FrameBuffer(int _width, int _height) : width(_width), height(_height), color_buffer(std::vector<Color>(_width * _height)) {}
    void DumpAsPPMFile(std::string filename);
    void writeColor(Coord2D coord, Color c);
    // Color readColor(Coord2D coord);
    // void writeDepth(Coord2D coord, uint8_t depth);
    // uint8_t readDepth(Coord2D coord);
    int width;
    int height;
    int max_component_value = 255;
    std::vector<Color> color_buffer;
};

void FrameBuffer::writeColor(Coord2D coord, Color c) {
    std::cout << "Writing color at x = " << coord.x << " y = " << coord.y << "\n";
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
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color c = color_buffer[y*width + x];
            fs << c;
        }
    }
}

// Mesh represents the geometry of the object in terms of
// vertices/faces/normals/texcoords etc.
class Mesh {
    public:
    Mesh(int _num_triangles, std::vector<float3> _vertices, std::vector<float3> _colors) : vertices(_vertices), num_triangles(_num_triangles), colors(_colors) {}
    std::vector<float3> vertices;
    std::vector<float3> colors;
    int num_triangles;
    static Mesh createTriangleMesh();
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

class Model {
    public:
    Model(Mesh _m) : mesh(_m) {}
    Mesh mesh;
    void draw(FrameBuffer &fb);

    private:
    //float3 vertex_shader(float3 position);
    //float3 fragment_shader(float3 position);
};


struct Varyings {
    float3 position;
    float3 color;
    Varyings() = default;
};

Varyings vertex_shader(float3 position, float3 color) {
    // attributes required
        // position
        // color

    // uniforms required
        // none

    // varyings output
        // position
    Varyings vout;
    vout.position = position;
    vout.color = color;
    return vout;
}

// The position fragment shader receives is interpolated
float3 fragment_shader(Varyings frag_in) {
    return frag_in.color;
}

float edge_function(float3 a, float3 b, float3 c) {
    float w = (a.x - b.x)*(c.y - b.y) - (a.y - b.y)*(c.x - b.x);
    return w;
}

void Model::draw(FrameBuffer &fb) {
    for (int i = 0; i < mesh.num_triangles; i++) {
        std::vector<Varyings> vertex_outs;
        for (int vid = 0; vid < 3; vid++) {
            // run vertex shading
            float3 pos_in = mesh.vertices[i*3 + vid];
            float3 color_in = mesh.colors[i*3 + vid];
            Varyings vertex_out = vertex_shader(pos_in, color_in);
            vertex_outs.push_back(vertex_out);
        }

        // TODO: Clipping
        // FIXME:Some missing steps here after vertex shader is run...

        for (int x = 0; x < fb.width; x++) {
            for (int y = 0; y < fb.height; y++) {
                Coord2D pixel(x, y);
                float3 p;
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
                    std::cout << "p = (" << p.x << "," << p.y << ")\n"; 

                    // Compute barycentrics
                    float w0 = e01/area;
                    float w1 = e12/area;
                    float w2 = e20/area;

                    // Interpolate varyings
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

class Scene {
    public:
    Scene(std::vector<Model> _models) : models(_models){};
    std::vector<Model> models;

    // Constructors
    //static Scene CreateSceneFromGLTF(std::string filename);
    static Scene CreateTriangleScene();
};

Scene Scene::CreateTriangleScene() {
    Mesh mesh = Mesh::createTriangleMesh();
    Model model = Model(mesh);
    std::vector<Model> models;
    models.push_back(model);
    return Scene(models);
}

class Renderer {
    public:
    static void Render(FrameBuffer &fb, Scene &scn);
};


void Renderer::Render(FrameBuffer &fb, Scene &scn) {
    std::cout << "Rendering started\n";
    for (int i = 0; i < scn.models.size(); i++) {
        Model &m = scn.models[i];
        m.draw(fb);
    }
    std::cout << "Rendering ended\n";
}

int main() {
    int width = 256;
    int height = 256;

    // Create Scene
    Scene scn = Scene::CreateTriangleScene();

    // Create FrameBuffer
    FrameBuffer fb = FrameBuffer(width, height);

    // Draw Scene into FrameBuffer
    Renderer::Render(fb, scn);

    // Dump FrameBuffer into an image file
    fb.DumpAsPPMFile("image.ppm");
    return 0;
}