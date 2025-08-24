#pragma once
#include "linmath/linmath.h"

struct vector2 {
    float x = {};
    float y = {};

    // operator overloads
    vector2 operator+(const vector2 &other) const;

    vector2 operator-(const vector2 &other) const;

    vector2 operator*(float scalar) const;

    vector2 operator/(float scalar) const;

    vector2 &operator+=(const vector2 &other);

    vector2 &operator-=(const vector2 &other);

    vector2 &operator*=(float scalar);

    vector2 &operator/=(float scalar);

    // functions
    float Magnitude() const;

    vector2 Normalized() const;

    float Dot(const vector2 &other) const;
};
struct vector3 {
    float x = {};
    float y = {};
    float z = {};

    // operator overloads
    vector3 operator+(const vector3 &other) const;

    vector3 operator-(const vector3 &other) const;

    vector3 operator*(float scalar) const;

    vector3 operator/(float scalar) const;

    vector3 &operator+=(const vector3 &other);

    vector3 &operator-=(const vector3 &other);

    vector3 &operator*=(float scalar);

    vector3 &operator/=(float scalar);

    float magnitude() const;

    vector3 normalized() const;

    float dot(const vector3 &other) const;

    vector3 cross(const vector3 &other) const;
};

struct RowMatrix4x4 {
    RowMatrix4x4() = default;
    RowMatrix4x4(const RowMatrix4x4& other);

    static RowMatrix4x4 identity();

    RowMatrix4x4 operator*(const RowMatrix4x4& other) const;
    RowMatrix4x4 operator*(float value) const;
    RowMatrix4x4 operator+(const RowMatrix4x4& other) const;
    RowMatrix4x4 operator-(const RowMatrix4x4& other) const;

    RowMatrix4x4 multiply(const RowMatrix4x4& other) const;
    RowMatrix4x4 scale(float value) const;
    RowMatrix4x4 scale_anisotropic(float x, float y, float z) const;
    RowMatrix4x4 add(const RowMatrix4x4& other) const;
    RowMatrix4x4 subtract(const RowMatrix4x4& other) const;

    RowMatrix4x4 transpose() const;
    RowMatrix4x4 translate(float x, float y, float z) const;

    RowMatrix4x4 rotate_x(float angle) const;
    RowMatrix4x4 rotate_y(float angle) const;
    RowMatrix4x4 rotate_z(float angle) const;

    static RowMatrix4x4 ortho(float left, float right, float bottom, float top, float near, float far);

    float data[4][4] = {{0}, {0}, {0}, {0}};
};

struct ColumnMatrix4x4 {
    ColumnMatrix4x4() = default;
    ColumnMatrix4x4(const ColumnMatrix4x4& other);

    static ColumnMatrix4x4 identity();

    ColumnMatrix4x4 operator*(const ColumnMatrix4x4 &other) const;
    ColumnMatrix4x4 operator*(float value) const;
    ColumnMatrix4x4 operator+(const ColumnMatrix4x4 &other) const;
    ColumnMatrix4x4 operator-(const ColumnMatrix4x4 &other) const;

    ColumnMatrix4x4 multiply(const ColumnMatrix4x4 &other) const;
    ColumnMatrix4x4 scale(float value) const;
    ColumnMatrix4x4 scale_anisotropic(float x, float y, float z) const;
    ColumnMatrix4x4 add(const ColumnMatrix4x4 &other) const;
    ColumnMatrix4x4 subtract(const ColumnMatrix4x4 &other) const;

    ColumnMatrix4x4 transpose() const;
    ColumnMatrix4x4 translate(float x, float y, float z) const;

    ColumnMatrix4x4 rotate_x(float angle) const;
    ColumnMatrix4x4 rotate_y(float angle) const;
    ColumnMatrix4x4 rotate_z(float angle) const;
    ColumnMatrix4x4 ortho(float left, float right, float bottom, float top, float near, float far) const;

    bool compareTo(const ColumnMatrix4x4& other) const;

    static ColumnMatrix4x4 wrap(mat4x4 other);

    operator float*();

    float data[4][4] = {{0}, {0}, {0}, {0}};
};

struct Transform {
    vector2 position;
    float rotation;
    vector2 scale;
};

struct Color {
    float r, g, b, a;
};

struct Vertex {
    vector2 position;
    Color color;
    vector2 uv;
};

struct Buffers {
    unsigned int vertexBuffer;
    unsigned int indexBuffer;
};