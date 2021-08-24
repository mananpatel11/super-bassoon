#include "data_types.h"

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

std::ostream& operator<<(std::ostream &os, Color &c) {
    os << uint(c.r) << " " << uint(c.g) << " " << uint(c.b) << " ";
    return os;
}