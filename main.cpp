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
    Mesh(int _num_triangles, std::vector<float3> _vertices) : vertices(_vertices), num_triangles(_num_triangles) {}
    std::vector<float3> vertices;
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
    return Mesh(1, vertices);
}

class Model {
    public:
    Model(Mesh _m) : mesh(_m) {}
    Mesh mesh;
    void draw(FrameBuffer &fb);

    private:
    float3 vertex_shader(float3 position);
    float3 fragment_shader(float3 position);
};

float3 Model::vertex_shader(float3 position) {
    // attributes required
        // position

    // uniforms required
        // none

    // varyings output
        // position
    
    return position;
}

// The position fragment shader receives is interpolated
float3 Model::fragment_shader(float3 position) {
    return float3(1.0, 0.0, 0.0);
}

std::vector<Coord2D> rasterize(float3 v0, float3 v1, float3 v2, FrameBuffer &fb) {
    std::vector<Coord2D> fragments;
    for (int x = 0; x < fb.width; x++) {
        for (int y = 0; y < fb.height; y++) {
            Coord2D pixel(x, y);
            float3 p;
            // Find pixel center
            p.x = (pixel.x * 2.0/fb.width) - 1.0 + (1.0 / fb.width);
            p.y = (pixel.y * 2.0/fb.height) - 1.0 + (1.0 / fb.height);

            std::cout << "p = (" << p.x << "," << p.y << ")\n";  

            float e01 = (p.x - v0.x)*(v1.y - v0.y) - (p.y - v0.y)*(v1.x - v0.x);
            float e12 = (p.x - v1.x)*(v2.y - v1.y) - (p.y - v1.y)*(v2.x - v1.x);
            float e20 = (p.x - v2.x)*(v0.y - v2.y) - (p.y - v2.y)*(v0.x - v2.x);
            std::cout << "e01 = " << e01 << "\n";
            std::cout << "e10 = " << e12 << "\n";
            std::cout << "e20 = " << e20 << "\n";
            if ((e01 >= 0) && (e12 >= 0) && (e20 > 0)) {
                fragments.push_back(pixel);
            }
        }
    }
    return fragments;
}

void Model::draw(FrameBuffer &fb) {
    for (int i = 0; i < mesh.num_triangles; i++) {
        std::vector<float3> pos_outs;
        for (int vid = 0; vid < 3; vid++) {
            // run vertex shading
            float3 pos_in = mesh.vertices[i*3 + vid];
            float3 pos_out = vertex_shader(pos_in);
            pos_outs.push_back(pos_out);
        }

        // TODO: Clipping
        // FIXME:Some missing steps here after vertex shader is run...

        // Rasterize
        std::vector<Coord2D> fragments = rasterize(pos_outs[0], pos_outs[1], pos_outs[2], fb);
        
        // TODO: Perform depth testing to discard fragments behind other geometry
        // TODO: Varying interpolation

        // For each fragment run fragment shader and write color to FB
        for (int i = 0; i < fragments.size(); i++) {
            Coord2D fragment = fragments[i];
            float3 color = fragment_shader(float3());
            fb.writeColor(fragment, color);
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