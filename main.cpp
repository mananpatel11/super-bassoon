#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

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

float3 operator/(float3 num, float den) {
    return float3(num.x/den, num.y/den, num.z/den);
}

float3 operator+(float3 lhs, float3 rhs) {
    return float3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

float3 operator-(float3 lhs, float3 rhs) {
    return float3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

float dot(float3 lhs, float3 rhs) {
    return (lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z);
}

float3 cross(float3 a, float3 b) {
    return float3((a.y*b.z - a.z*b.y), -(a.x*b.z - a.z*b.x), (a.x*b.y - a.y*b.x));
}

float magnitude(float3 vec) {
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

float3 normal(float3 vec) {
    return (vec/magnitude(vec));
}

struct float4 {
    float x;
    float y;
    float z;
    float w;
    float4() = default;
    float4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

std::ostream& operator<<(std::ostream& os, float4& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    return os;
}

float4 operator*(float lhs, float4 rhs) {
    return float4(lhs*rhs.x, lhs*rhs.y, lhs*rhs.z, lhs*rhs.w);
}

float4 operator/(float4 num, float den) {
    return float4(num.x/den, num.y/den, num.z/den, num.w/den);
}

float4 operator+(float4 lhs, float4 rhs) {
    return float4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

float4 operator-(float4 lhs, float4 rhs) {
    return float4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

float dot(float4 lhs, float4 rhs) {
    return (lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z + lhs.w*rhs.w);
}

float magnitude(float4 vec) {
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z + vec.w*vec.w);
}

float4 normal(float4 vec) {
    return (vec/magnitude(vec));
}

struct float4x4 {
    float4 row0;
    float4 row1;
    float4 row2;
    float4 row3;
    float4x4() = default;
    float4x4(float4 diagonal) : row0(float4()), row1(float4()), row2(float4()), row3(float4()) {
        row0.x = diagonal.x;
        row1.y = diagonal.y;
        row2.z = diagonal.z;
        row3.w = diagonal.w;
    }
    float4x4(float4 _r0, float4 _r1, float4 _r2, float4 _r3) : row0(_r0), row1(_r1), row2(_r2), row3(_r3) {}  
};

std::ostream& operator<<(std::ostream& os, float4x4 M) {
    os << "\n";
    os << M.row0 << "\n";
    os << M.row1 << "\n";
    os << M.row2 << "\n";
    os << M.row3 << "\n";
    os << "\n";
    return os;
}

float4 operator*(float4x4 lhs, float4 rhs) {
        return float4(dot(lhs.row0, rhs), dot(lhs.row1, rhs), dot(lhs.row2, rhs), dot(lhs.row3, rhs));
}

float4x4 operator/(float4x4 M, float scalar) {
    float4x4 A = M;
    A.row0 = M.row0/scalar;
    A.row1 = M.row1/scalar;
    A.row2 = M.row2/scalar;
    A.row3 = M.row3/scalar;
    return M;
}

float4x4 operator*(float4x4 a, float4x4 b) {
    float4x4 result;

    result.row0.x = dot(a.row0, float4(b.row0.x, b.row1.x, b.row2.x, b.row3.x));
    result.row0.y = dot(a.row0, float4(b.row0.y, b.row1.y, b.row2.y, b.row3.y));
    result.row0.z = dot(a.row0, float4(b.row0.z, b.row1.z, b.row2.z, b.row3.z));
    result.row0.w = dot(a.row0, float4(b.row0.w, b.row1.w, b.row2.w, b.row3.w));

    result.row1.x = dot(a.row1, float4(b.row0.x, b.row1.x, b.row2.x, b.row3.x));
    result.row1.y = dot(a.row1, float4(b.row0.y, b.row1.y, b.row2.y, b.row3.y));
    result.row1.z = dot(a.row1, float4(b.row0.z, b.row1.z, b.row2.z, b.row3.z));
    result.row1.w = dot(a.row1, float4(b.row0.w, b.row1.w, b.row2.w, b.row3.w));

    result.row2.y = dot(a.row2, float4(b.row0.y, b.row1.y, b.row2.y, b.row3.y));
    result.row2.z = dot(a.row2, float4(b.row0.z, b.row1.z, b.row2.z, b.row3.z));
    result.row2.x = dot(a.row2, float4(b.row0.x, b.row1.x, b.row2.x, b.row3.x));
    result.row2.w = dot(a.row2, float4(b.row0.w, b.row1.w, b.row2.w, b.row3.w));

    result.row3.x = dot(a.row3, float4(b.row0.x, b.row1.x, b.row2.x, b.row3.x));
    result.row3.y = dot(a.row3, float4(b.row0.y, b.row1.y, b.row2.y, b.row3.y));
    result.row3.z = dot(a.row3, float4(b.row0.z, b.row1.z, b.row2.z, b.row3.z));
    result.row3.w = dot(a.row3, float4(b.row0.w, b.row1.w, b.row2.w, b.row3.w));

    return result;
}

float4x4 identity() {
    float4x4 M(float4(1.0, 1.0, 1.0, 1.0));
    return M;
}

float4x4 translationMatrix(float x, float y, float z) {
    float4x4 T = identity();
    T.row0.w = x;
    T.row1.w = y;
    T.row2.w = z;
    return T;
}

//https://docs.microsoft.com/en-us/windows/win32/direct3d9/projection-transform#a-w-friendly-projection-matrix
float4x4 projectionMatrix(const float near_plane, const float far_plane,
                          const float fov_horiz, const float fov_vert) {
    float    h, w, Q;

    w = (float)1/tan(fov_horiz*0.5);  // 1/tan(x) == cot(x)
    h = (float)1/tan(fov_vert*0.5);   // 1/tan(x) == cot(x)
    Q = far_plane/(far_plane - near_plane);

    float4x4 ret = float4x4();

    ret.row0.x = w;
    ret.row1.y = h;
    ret.row2.z = Q;
    ret.row3.z = -Q*near_plane;
    ret.row2.w = 1;
    return ret;
}

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
    for (int y = height-1; y >= 0; y--) {
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

Mesh Mesh::createOrthographicsCubeMesh() {
    std::vector<float3> vertices;
    float3 v00(-0.5, 0.0, 0.8);
    float3 v01(0.0, 0.0, 0.8);
    float3 v02(0.0, -0.5, 0.8);
    float3 v03(-0.5, -0.5, 0.8);
    float3 v10(0.0, 0.5, -0.8);
    float3 v11(0.5, 0.5, -0.8);
    float3 v12(0.5, 0.0, -0.8);
    float3 v13(0.0, 0.0, -0.8);
    
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
    vout.position = projection_matrix*view_transform*model_transform*vout.position;
    vout.position = vout.position;
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
            float4x4 projection_matrix = projectionMatrix(0.01, 1.0, 2.6, 2.6);
            //float4x4 projection_matrix = identity();
            Varyings vertex_out = vertex_shader(pos_in, color_in, transform, view_transform, projection_matrix);
            vertex_outs.push_back(vertex_out);
        }

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
                    std::cout << "p = (" << p.x << "," << p.y << ")\n"; 

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
    float4 row0(xaxis.x, yaxis.x, zaxis.x, 0);
    float4 row1(xaxis.y, yaxis.y, zaxis.y, 0);
    float4 row2(xaxis.z, yaxis.z, zaxis.z, 0);
    float4 row3(-dot(xaxis, eye), -dot(yaxis, eye), -dot(zaxis, eye), 1);
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
    static Scene CreateOrthographicCubeScene();
};

Scene Scene::CreateTriangleScene() {
    Mesh mesh = Mesh::createTriangleMesh();
    Model model = Model(mesh, identity());
    std::vector<Model> models;
    models.push_back(model);
    return Scene(models);
}

Scene Scene::CreateQuadScene() {
    Mesh mesh = Mesh::createQuadMesh();
    Model model = Model(mesh, identity());
    std::vector<Model> models;
    models.push_back(model);
    return Scene(models);
}

Scene Scene::CreateCubeScene() {
    std::vector<Model> models;
    Mesh mesh = Mesh::createCubeMesh();
    //float4x4 model_matrix(float4(0.5, 0.5, 0.5, 1.0));
    //float4x4 model_matrix(float4(1.0, 1.0, 1.0, 1.0));
    float4x4 model1_matrix = translationMatrix(-1.0, 1.0, -1.0);
    float4x4 model2_matrix = translationMatrix(1.0, 1.0, 4.0);
    Model mode1 = Model(mesh, model1_matrix);
    Model mode2 = Model(mesh, model2_matrix);
    models.push_back(mode1);
    models.push_back(mode2);
    Scene s(models);
    float3 eye(10, 10, -10);
    float3 at(0, 0, 0);
    float3 up(-1, 1, 1);
    s.view_matrix = lookAtMatrix(eye, at, up);
    //s.view_matrix = identity();
    return s;
}

Scene Scene::CreateOrthographicCubeScene() {
    Mesh mesh = Mesh::createOrthographicsCubeMesh();
    Model model = Model(mesh, identity());
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
        std::cout << scn.view_matrix;
        m.draw(fb, scn.view_matrix);
    }
    std::cout << "Rendering ended\n";
}

int test_triangle() {
    int width = 16;
    int height = 16;

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

int test_orthographics_cube() {
    int width = 256;
    int height = 256;

    // Create Scene
    Scene scn = Scene::CreateOrthographicCubeScene();

    // Create FrameBuffer
    FrameBuffer fb = FrameBuffer(width, height);

    // Draw Scene into FrameBuffer
    Renderer::Render(fb, scn);

    // Dump FrameBuffer into an image file
    fb.DumpAsPPMFile("orthographics_cube.ppm");
    return 0;
}

void test_matrix4x4() {
    float4x4 A = float4x4(float4(1, 2, 3, 4), float4(1, 2, 3, 4), float4(1, 2, 3, 4), float4(1, 2, 3, 4));
    float4x4 B = float4x4(float4(1, 2, 3, 4), float4(1, 2, 3, 4), float4(1, 2, 3, 4), float4(1, 2, 3, 4));
    float4x4 C = A*B;
    std::cout << C;
}

int main() {
    test_triangle();
    test_quad();
    test_cube();
    test_orthographics_cube();
}