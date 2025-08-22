#pragma once

struct vector2 {
    float x = {};
    float y = {};

    // operator overloads
    vector2 operator+(const vector2& other) const;
    vector2 operator-(const vector2& other) const;
    vector2 operator*(float scalar) const;
    vector2 operator/(float scalar) const;
    vector2& operator+=(const vector2& other);
    vector2& operator-=(const vector2& other);
    vector2& operator*=(float scalar);
    vector2& operator/=(float scalar);

    // functions
    float Magnitude() const;
    vector2 Normalized() const;
    float Dot(const vector2& other) const;
};
struct vector3 {
    float x = {};
    float y = {};
    float z = {};

    // operator overloads
    vector3 operator+(const vector3& other) const;
    vector3 operator-(const vector3& other) const;
    vector3 operator*(float scalar) const;
    vector3 operator/(float scalar) const;
    vector3& operator+=(const vector3& other);
    vector3& operator-=(const vector3& other);
    vector3& operator*=(float scalar);
    vector3& operator/=(float scalar);

    float magnitude() const;
    vector3 normalized() const;
    float dot(const vector3& other) const;
    vector3 cross(const vector3& other) const;
};

struct matrix4x4 {
    matrix4x4(const matrix4x4& other);
    matrix4x4(const float other[4][4]);
    float data[4][4] {};

    float* operator[](int i);
    const float* operator[](int i) const;

    void multiplicity(const matrix4x4& other);
    void multiplicity(const matrix4x4& other, const matrix4x4& other_other);
    void multiplicity(const float other[4][4]);
    void multiplicity(const float other[4][4], const float other_other[4][4]);

    void identity();
    void scale_anisotropic(float x, float y, float z);
    void transpose();
    void copy(const matrix4x4& other);
    void copy(const float other[4][4]);
    void translate(float x, float y, float z);
    void rotate_z(float angle);
    void orthogonalize(float left, float right, float bottom, float top, float near, float far);
};


struct Color {
    float r, g, b, a;
};
struct Vertex {
    vector2 position;
    Color color;
    vector2 uv;
};
struct Transform {
    vector2 position;
    float rotation;
    vector2 scale;
};