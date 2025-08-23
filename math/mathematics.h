#pragma once

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

struct rowMatrix4x4 {
    rowMatrix4x4() = default;
    rowMatrix4x4(const rowMatrix4x4& other);

    static rowMatrix4x4 identity();

    rowMatrix4x4 operator*(const rowMatrix4x4& other) const;
    rowMatrix4x4 operator*(float value) const;
    rowMatrix4x4 operator+(const rowMatrix4x4& other) const;
    rowMatrix4x4 operator-(const rowMatrix4x4& other) const;

    rowMatrix4x4 multiply(const rowMatrix4x4& other) const;
    rowMatrix4x4 scale(float value) const;
    rowMatrix4x4 scale_anisotropic(float x, float y, float z) const;
    rowMatrix4x4 add(const rowMatrix4x4& other) const;
    rowMatrix4x4 subtract(const rowMatrix4x4& other) const;

    rowMatrix4x4 transpose() const;
    rowMatrix4x4 translate(float x, float y, float z) const;

    rowMatrix4x4 rotate_x(float angle) const;
    rowMatrix4x4 rotate_y(float angle) const;
    rowMatrix4x4 rotate_z(float angle) const;

    static rowMatrix4x4 ortho(float left, float right, float bottom, float top, float near, float far);

    float data[4][4] = {{0}, {0}, {0}, {0}};
};

struct columnMatrix4x4 {
    columnMatrix4x4() = default;
    columnMatrix4x4(const columnMatrix4x4& other);

    static columnMatrix4x4 identity();

    columnMatrix4x4 operator*(const columnMatrix4x4 &other) const;
    columnMatrix4x4 operator*(float value) const;
    columnMatrix4x4 operator+(const columnMatrix4x4 &other) const;
    columnMatrix4x4 operator-(const columnMatrix4x4 &other) const;

    columnMatrix4x4 multiply(const columnMatrix4x4 &other) const;
    columnMatrix4x4 scale(float value) const;
    columnMatrix4x4 scale_anisotropic(float x, float y, float z) const;
    columnMatrix4x4 add(const columnMatrix4x4 &other) const;
    columnMatrix4x4 subtract(const columnMatrix4x4 &other) const;

    columnMatrix4x4 transpose() const;
    columnMatrix4x4 translate(float x, float y, float z) const;

    columnMatrix4x4 rotate_x(float angle) const;
    columnMatrix4x4 rotate_y(float angle) const;
    columnMatrix4x4 rotate_z(float angle) const;
    columnMatrix4x4 ortho(float left, float right, float bottom, float top, float near, float far) const;

    bool compareTo(const columnMatrix4x4& other) const;

    static columnMatrix4x4 wrap(float* other);

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