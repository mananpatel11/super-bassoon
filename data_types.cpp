#include "data_types.h"

float2 operator*(float lhs, const float2 &rhs) {
    return float2(lhs*rhs.x, lhs*rhs.y);
}

float2 operator/(const float2 &num, float den) {
    return float2(num.x/den, num.y/den);
}

float2 operator+(const float2 &lhs, const float2 &rhs) {
    return float2(lhs.x + rhs.x, lhs.y + rhs.y);
}

float2 operator-(const float2 &lhs, const float2 &rhs) {
    return float2(lhs.x - rhs.x, lhs.y - rhs.y);
}

float dot(const float2 &lhs, const float2 &rhs) {
    return float(lhs.x*rhs.x + lhs.y*rhs.y);
}

float magnitude(const float2 &vec) {
    return sqrt(vec.x*vec.x + vec.y*vec.y);
}

float2 normal(const float2 &vec) {
    return vec/magnitude(vec);
}

std::ostream& operator<<(std::ostream& os, const float2 &vec2) {
    os << "(" << vec2.x << ", " << vec2.y << ")";
    return os;
}

float3 operator*(const float &lhs, const float3 &rhs) {
    return float3(lhs*rhs.x, lhs*rhs.y, lhs*rhs.z);
}

float3 operator/(const float3 &num, const float &den) {
    return float3(num.x/den, num.y/den, num.z/den);
}

float3 operator+(const float3 &lhs, const float3 &rhs) {
    return float3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

float3 operator-(const float3 &lhs, const float3 &rhs) {
    return float3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

float dot(const float3 &lhs, const float3 &rhs) {
    return (lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z);
}

float3 cross(const float3 &a, const float3 &b) {
    return float3((a.y*b.z - a.z*b.y), -(a.x*b.z - a.z*b.x), (a.x*b.y - a.y*b.x));
}

float magnitude(const float3 &vec) {
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

float3 normal(const float3 &vec) {
    return (vec/magnitude(vec));
}

std::ostream& operator<<(std::ostream& os, const float3& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

float4 operator*(const float &lhs, const float4 &rhs) {
    return float4(lhs*rhs.x, lhs*rhs.y, lhs*rhs.z, lhs*rhs.w);
}

float4 operator/(const float4 &num, const float &den) {
    return float4(num.x/den, num.y/den, num.z/den, num.w/den);
}

float4 operator+(const float4 &lhs, const float4 &rhs) {
    return float4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

float4 operator-(const float4 &lhs, const float4 &rhs) {
    return float4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

float dot(const float4 &lhs, const float4 &rhs) {
    return (lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z + lhs.w*rhs.w);
}

float magnitude(const float4 &vec) {
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z + vec.w*vec.w);
}

float4 normal(const float4 &vec) {
    return (vec/magnitude(vec));
}

std::ostream& operator<<(std::ostream& os, float4& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    return os;
}

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

float4x4 operator*(const float4x4 &a, const float4x4 &b) {
    float4x4 result;

    float4 bcol0 = float4(b.row0.x, b.row1.x, b.row2.x, b.row3.x);
    float4 bcol1 = float4(b.row0.y, b.row1.y, b.row2.y, b.row3.y);
    float4 bcol2 = float4(b.row0.z, b.row1.z, b.row2.z, b.row3.z);
    float4 bcol3 = float4(b.row0.w, b.row1.w, b.row2.w, b.row3.w);

    result.row0.x = dot(a.row0, float4(bcol0));
    result.row0.y = dot(a.row0, float4(bcol1));
    result.row0.z = dot(a.row0, float4(bcol2));
    result.row0.w = dot(a.row0, float4(bcol3));

    result.row1.x = dot(a.row1, float4(bcol0));
    result.row1.y = dot(a.row1, float4(bcol1));
    result.row1.z = dot(a.row1, float4(bcol2));
    result.row1.w = dot(a.row1, float4(bcol3));

    result.row2.x = dot(a.row2, float4(bcol0));
    result.row2.y = dot(a.row2, float4(bcol1));
    result.row2.z = dot(a.row2, float4(bcol2));
    result.row2.w = dot(a.row2, float4(bcol3));

    result.row3.x = dot(a.row3, float4(bcol0));
    result.row3.y = dot(a.row3, float4(bcol1));
    result.row3.z = dot(a.row3, float4(bcol2));
    result.row3.w = dot(a.row3, float4(bcol3));

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

float4x4 scalingMatrix(float x, float y, float z) {
    return float4x4(float4(x, y, z, 1.0));
}

float4x4 rotationMatrix() {
    return identity();
}

//https://docs.microsoft.com/en-us/windows/win32/direct3d9/projection-transform#a-w-friendly-projection-matrix
// Page 92 - Real-time rendering
float4x4 perspectiveProjectionMatrix(const float near_plane, const float far_plane,
                          const float fov_horiz, const float fov_vert) {
    float    h, w, Q;

    w = (float)1/tan(fov_horiz*0.5);  // 1/tan(x) == cot(x)
    h = (float)1/tan(fov_vert*0.5);   // 1/tan(x) == cot(x)
    Q = far_plane/(far_plane - near_plane);

    float4x4 ret = float4x4();
    ret.row0.x = w;
    ret.row1.y = h;
    ret.row2.z = Q;
    ret.row2.w = -Q*near_plane;
    ret.row3.z = 1;
    return ret;
}

// https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixortholh
// Page 92 - Real-time rendering
float4x4 orthographicProjectionMatrix(float w, float h, float zn, float zf) {
    float4x4 ret = float4x4();
    ret.row0.x = 2.0/w;
    ret.row1.y = 2.0/h;
    ret.row2.z = 1.0/(zf-zn);
    ret.row2.w = -zn/(zf-zn);
    ret.row3.w = 1.0;
    return ret;
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

std::ostream& operator<<(std::ostream &os, Color &c) {
    os << uint(c.r) << " " << uint(c.g) << " " << uint(c.b) << " ";
    return os;
}