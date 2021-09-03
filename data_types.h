#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <math.h>

struct float2 {
    float x;
    float y;
    float2() = default;
    float2(float _x, float _y) : x(_x), y(_y) {}
};

float2 operator*(float lhs, const float2 &rhs);
float2 operator/(const float2 &num, float den);
float2 operator+(const float2 &lhs, const float2 &rhs);
float2 operator-(const float2 &lhs, const float2 &rhs);
float dot(const float2 &lhs, const float2 &rhs);
float magnitude(const float2 &vec);
float2 normal(const float2 &vec);
std::ostream& operator<<(std::ostream& os, const float2 &vec2);

struct float3 {
    float x;
    float y;
    float z;
    float3() = default;
    float3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

float3 operator*(const float &lhs, const float3 &rhs);
float3 operator/(const float3 &num, const float &den);
float3 operator+(const float3 &lhs, const float3 &rhs);
float3 operator-(const float3 &lhs, const float3 &rhs);
float dot(const float3 &lhs, const float3 &rhs);
float3 cross(const float3 &a, const float3 &b);
float magnitude(const float3 &vec);
float3 normal(const float3 &vec);
std::ostream& operator<<(std::ostream& os, const float3 &vec3);

struct float4 {
    float x;
    float y;
    float z;
    float w;
    float4() = default;
    float4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

std::ostream& operator<<(std::ostream& os, float4& vec);
float4 operator*(const float &lhs, const float4 &rhs);
float4 operator/(const float4 &num, const float &den);
float4 operator+(const float4 &lhs, const float4 &rhs);
float4 operator-(const float4 &lhs, const float4 &rhs);
float dot(const float4 &lhs, const float4 &rhs);
float magnitude(const float4 &vec);
float4 normal(const float4 &vec);

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

std::ostream& operator<<(std::ostream& os, float4x4 M);
float4 operator*(float4x4 lhs, float4 rhs);
float4x4 operator/(float4x4 M, float scalar);
float4x4 operator*(const float4x4 &a, const float4x4 &b);
float4x4 identity();
float4x4 translationMatrix(float x, float y, float z);
float4x4 scalingMatrix(float x, float y, float z);
float4x4 rotationMatrix();


float4x4 perspectiveProjectionMatrix(const float near_plane, const float far_plane,
                          const float fov_horiz, const float fov_vert);
float4x4 orthographicProjectionMatrix(float w, float h, float zn, float zf);
float4x4 lookAtMatrix(float3 eye, float3 at, float3 up);

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
std::ostream& operator<<(std::ostream &os, Color &c);

struct Coord2D {
    int x;
    int y;
    Coord2D() = default;
    Coord2D(int _x, int _y) : x(_x), y(_y) {}
};