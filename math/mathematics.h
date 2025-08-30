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

    bool operator==(const vector2 &other) const;

    // functions
    float Magnitude() const;

    vector2 Normalized() const;

    vector2 hadamard(const vector2 &other) const;

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

struct vector4 {
    float x = {};
    float y = {};
    float z = {};
    float w = {};

    vector4 operator+(const vector4 &other) const;

    vector4 operator-(const vector4 &other) const;

    vector4 operator*(float scalar) const;

    vector4 operator/(float scalar) const;

    vector4 &operator+=(const vector4 &other);

    vector4 &operator-=(const vector4 &other);

    vector4 &operator*=(float scalar);

    vector4 &operator/=(float scalar);

    float magnitude() const;

    vector4 normalized() const;

    float dot(const vector4 &other) const;
};

struct RowMatrix4x4 {
    RowMatrix4x4() = default;

    RowMatrix4x4(const RowMatrix4x4 &other);

    static RowMatrix4x4 identity();

    RowMatrix4x4 operator*(const RowMatrix4x4 &other) const;

    RowMatrix4x4 operator*(float value) const;

    RowMatrix4x4 operator+(const RowMatrix4x4 &other) const;

    RowMatrix4x4 operator-(const RowMatrix4x4 &other) const;

    RowMatrix4x4 multiply(const RowMatrix4x4 &other) const;

    RowMatrix4x4 scale(float value) const;

    RowMatrix4x4 scale_anisotropic(float x, float y, float z) const;

    RowMatrix4x4 add(const RowMatrix4x4 &other) const;

    RowMatrix4x4 subtract(const RowMatrix4x4 &other) const;

    RowMatrix4x4 transpose() const;

    RowMatrix4x4 translate(float x, float y, float z) const;

    RowMatrix4x4 rotate_x(float angle) const;

    RowMatrix4x4 rotate_y(float angle) const;

    RowMatrix4x4 rotate_z(float angle) const;

    static RowMatrix4x4 ortho(float left, float right, float bottom, float top, float near, float far);

    float data[4][4] = {{0}, {0}, {0}, {0}};
};

// template<int n, int m>
// class ColumnMatrix {
// private:
//     float data[n][m] {{}};
// public:
//     const int columns;
//     const int rows;
//
//     ColumnMatrix() = default;
//
//     ColumnMatrix(const ColumnMatrix &other);
//
//     template<int n1>
//     static ColumnMatrix<n1, n1> identity() {
//         /// 1, 0, 0, 0
//         /// 0, 1, 0, 0
//         /// 0, 0, 1, 0
//         /// 0, 0, 0, 1
//         ColumnMatrix result;
//         for (int c = 0; c < n1; c++) {
//             result.data[c][c] = 1;
//         }
//         return result;
//     }
//
//     template<int n1, int m1>
//     ColumnMatrix<n, m1> multiply(const ColumnMatrix<n1, m1> &other) const {
//         // allocate result
//         ColumnMatrix<n, other.columns> result;
//         // loop columns
//         for (int c = 0; c < other.columns; c++) {
//             for (int r = 0; r < rows; r++) {
//                 for (int x = 0; x < columns; x++) {
//                     result.data[c][r] += data[x][r] * other.data[c][x];
//                 }
//             }
//         }
//         return result;
//     }
// };

struct ColumnMatrix4x4 {

    ColumnMatrix4x4() = default;

    ColumnMatrix4x4(const ColumnMatrix4x4 &other);

    static ColumnMatrix4x4 identity();

    ColumnMatrix4x4 operator*(const ColumnMatrix4x4 &other) const;

    ColumnMatrix4x4 operator*(float value) const;

    vector4 operator*(const vector4 &other) const;

    vector2 operator*(const vector2 &other) const;

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

    ColumnMatrix4x4 inverse() const;

    float *operator[](int index);

    explicit operator const float*() const;
    explicit operator float*();

    bool compareTo(const ColumnMatrix4x4 &other) const;

    static ColumnMatrix4x4 wrap(mat4x4 other);

    float data[4][4] = {{0}, {0}, {0}, {0}};
};

class Transform {
private:
    const Transform *parent = nullptr;

public:
    Transform(vector2 pos, float rot, vector2 scale, const Transform *parent);

    Transform(vector2 pos, float rot, vector2 scale);

    // ColumnMatrix4x4 localTransformationMatrix() const;
    // ColumnMatrix4x4 globalTransformationMatrix() const;

    ColumnMatrix4x4 localToWorldMatrix() const;
    ColumnMatrix4x4 worldToLocalMatrix() const;

    void setGlobalPosition(vector2 pos);

    void setGlobalRotation(float rot);

    void setGlobalScale(vector2 scale);

    vector2 getGlobalPosition() const;

    float getGlobalRotation() const;

    vector2 getGlobalScale() const;

    vector2 localPosition{};
    float localRotation{};
    vector2 localScale{};
};

struct Vertex {
    vector2 position;
    vector2 uv;
};

struct Buffers {
    unsigned int vertexBuffer;
    unsigned int indexBuffer;
};

template<typename T>
concept number = std::is_arithmetic_v<T>;

template<number T>
T clamp(T value, T min, T max) {
    if (value > max)
        return max;

    if (value < min)
        return min;

    return value;
}
