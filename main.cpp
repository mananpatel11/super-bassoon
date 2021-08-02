#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct float3 {
    float x;
    float y;
    float z;
    float3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

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
    Model(Mesh _m) : m(_m) {}
    Mesh m;
};

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

struct Coord2D {
    int x;
    int y;
    Coord2D(int _x, int _y) : x(_x), y(_y) {}
};

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    Color() : r(128), g(0), b(0) {}
    Color(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}
};

std::ostream& operator<<(std::ostream &os, Color &c) {
    os << uint(c.r) << " " << uint(c.g) << " " << uint(c.b) << " ";
    return os;
}

class FrameBuffer {
    public:
    FrameBuffer(int _width, int _height) : width(_width), height(_height), color_buffer(std::vector<Color>(_width * _height)) {}
    void DumpAsPPMFile(std::string filename);
    // void writeColor(Coord2D coord, Color c);
    // Color readColor(Coord2D coord);
    // void writeDepth(Coord2D coord, uint8_t depth);
    // uint8_t readDepth(Coord2D coord);
    
    private:
    int width;
    int height;
    int max_component_value = 255;
    std::vector<Color> color_buffer;
};

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

class Renderer {
    public:
    static void Render(FrameBuffer &fb, Scene &scn);
};

void Renderer::Render(FrameBuffer &fb, Scene &scn) {
    std::cout << "Rendering started\n";
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