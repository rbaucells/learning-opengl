#include "mathematics.h"
#include <cmath>
#include <cstring>
#include <float.h>

ColumnMatrix4X4::ColumnMatrix4X4(const ColumnMatrix4X4 &other) {
    // mem copy
    memcpy(data, other.data, sizeof(float) * 16);
}

ColumnMatrix4X4 ColumnMatrix4X4::identity() {
    /// 1, 0, 0, 0
    /// 0, 1, 0, 0
    /// 0, 0, 1, 0
    /// 0, 0, 0, 1

    ColumnMatrix4X4 result;

    for (int c = 0; c < 4; c++) {
        result.data[c][c] = 1;
    }

    return result;
}

ColumnMatrix4X4 ColumnMatrix4X4::operator*(const ColumnMatrix4X4 &other) const {
    return multiply(other);
}

ColumnMatrix4X4 ColumnMatrix4X4::operator*(const float value) const {
    return scale(value);
}

ColumnMatrix4X4 ColumnMatrix4X4::operator+(const ColumnMatrix4X4 &other) const {
    return add(other);
}

ColumnMatrix4X4 ColumnMatrix4X4::operator-(const ColumnMatrix4X4 &other) const {
    return subtract(other);
}

ColumnMatrix4X4 ColumnMatrix4X4::add(const ColumnMatrix4X4 &other) const {
    ColumnMatrix4X4 result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result.data[c][r] = data[c][r] + other.data[c][r];
        }
    }

    return result;
}

ColumnMatrix4X4 ColumnMatrix4X4::subtract(const ColumnMatrix4X4 &other) const {
    ColumnMatrix4X4 result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result.data[c][r] = data[c][r] - other.data[c][r];
        }
    }

    return result;
}

ColumnMatrix4X4 ColumnMatrix4X4::multiply(const ColumnMatrix4X4 &other) const {
    ColumnMatrix4X4 result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            for (int x = 0; x < 4; x++) {
                result.data[c][r] += data[x][r] * other.data[c][x];
            }
        }
    }

    return result;
}

ColumnMatrix4X4 ColumnMatrix4X4::scale(const float value) const {
    ColumnMatrix4X4 result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result.data[c][r] = data[c][r] * value;
        }
    }

    return result;
}

ColumnMatrix4X4 ColumnMatrix4X4::scale_anisotropic(const float x, const float y, const float z) const {
    ColumnMatrix4X4 scaleMatrix = identity();

    scaleMatrix.data[0][0] = x;
    scaleMatrix.data[1][1] = y;
    scaleMatrix.data[2][2] = z;

    return multiply(scaleMatrix);
}

ColumnMatrix4X4 ColumnMatrix4X4::transpose() const {
    ColumnMatrix4X4 result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result.data[c][r] = data[r][c];
        }
    }

    return result;
}

ColumnMatrix4X4 ColumnMatrix4X4::translate(const float x, const float y, const float z) const {
    /// 1, 0, 0, x
    /// 0, 1, 0, y
    /// 0, 0, 1, z
    /// 0, 0, 0, 1
    ColumnMatrix4X4 translationMatrix = ColumnMatrix4X4::identity();

    translationMatrix.data[3][0] = x;
    translationMatrix.data[3][1] = y;
    translationMatrix.data[3][2] = z;

    return multiply(translationMatrix);
}

ColumnMatrix4X4 ColumnMatrix4X4::rotate_x(const float angle) const {
    const float sin = std::sinf(angle * (static_cast<float>(M_PI) / 180.0f));
    const float cos = std::cosf(angle * (static_cast<float>(M_PI) / 180.0f));

    ColumnMatrix4X4 rotationMatrix = ColumnMatrix4X4::identity();

    rotationMatrix.data[1][1] = cos;
    rotationMatrix.data[1][2] = sin;
    rotationMatrix.data[2][1] = -sin;
    rotationMatrix.data[2][2] = cos;

    return multiply(rotationMatrix);
}

ColumnMatrix4X4 ColumnMatrix4X4::rotate_y(const float angle) const {
    const float sin = std::sinf(angle * (static_cast<float>(M_PI) / 180.0f));
    const float cos = std::cosf(angle * (static_cast<float>(M_PI) / 180.0f));

    ColumnMatrix4X4 rotationMatrix = ColumnMatrix4X4::identity();

    rotationMatrix.data[0][0] = cos;
    rotationMatrix.data[0][2] = -sin;
    rotationMatrix.data[2][0] = sin;
    rotationMatrix.data[2][2] = cos;

    return multiply(rotationMatrix);
}

ColumnMatrix4X4 ColumnMatrix4X4::rotate_z(const float angle) const {
    const float sin = std::sinf(angle * (static_cast<float>(M_PI) / 180.0f));
    const float cos = std::cosf(angle * (static_cast<float>(M_PI) / 180.0f));

    ColumnMatrix4X4 rotationMatrix = ColumnMatrix4X4::identity();

    rotationMatrix.data[0][0] = cos;
    rotationMatrix.data[0][1] = sin;
    rotationMatrix.data[1][0] = -sin;
    rotationMatrix.data[1][1] = cos;

    return multiply(rotationMatrix);
}

ColumnMatrix4X4 ColumnMatrix4X4::ortho(const float left, const float right, const float bottom, const float top, const float near, const float far) const {
    // identity
    ColumnMatrix4X4 transformation = identity();
    // transformation
    transformation.data[0][0] = 2.0f / (right - left);
    transformation.data[1][1] = 2.0f / (top - bottom);
    transformation.data[2][2] = -2.0f / (far - near);
    transformation.data[3][0] = -(right + left) / (right - left);
    transformation.data[3][1] = -(top + bottom) / (top - bottom);
    transformation.data[3][2] = -(far + near) / (far - near);
    // return
    return transformation;
}

bool ColumnMatrix4X4::compareTo(const ColumnMatrix4X4 &other) const {
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            if (std::abs(other.data[c][r] - data[c][r]) > FLT_EPSILON)
                return false;
        }
    }

    return true;
}

ColumnMatrix4X4 ColumnMatrix4X4::wrap(mat4x4 other) {
    ColumnMatrix4X4 newColumnMatrix4x4;
    memcpy(newColumnMatrix4x4.data, other, sizeof(float) * 16);
    return newColumnMatrix4x4;
}

Vector4 ColumnMatrix4X4::operator*(const Vector4& other) const {
    Vector4 result;

    result.x = data[0][0] * other.x + data[1][0] * other.y + data[2][0] * other.z + data[3][0] * other.w;
    result.y = data[0][1] * other.x + data[1][1] * other.y + data[2][1] * other.z + data[3][1] * other.w;
    result.z = data[0][2] * other.x + data[1][2] * other.y + data[2][2] * other.z + data[3][2] * other.w;
    result.w = data[0][3] * other.x + data[1][3] * other.y + data[2][3] * other.z + data[3][3] * other.w;

    return result;
}

Vector2 ColumnMatrix4X4::operator*(const Vector2 &other) const {
    const Vector4 result = operator*(Vector4{other.x, other.y, 0, 1});

    return Vector2{result.x, result.y};
}

float* ColumnMatrix4X4::operator[](const int index) {
    return &data[index][0];
}

ColumnMatrix4X4::operator const float *() const {
    return &data[0][0];
}

ColumnMatrix4X4::operator float *() {
    return &data[0][0];
}


ColumnMatrix4X4 ColumnMatrix4X4::inverse() const {
    float s[6];
    float c[6];

    s[0] = data[0][0] * data[1][1] - data[1][0] * data[0][1];
    s[1] = data[0][0] * data[1][2] - data[1][0] * data[0][2];
    s[2] = data[0][0] * data[1][3] - data[1][0] * data[0][3];
    s[3] = data[0][1] * data[1][2] - data[1][1] * data[0][2];
    s[4] = data[0][1] * data[1][3] - data[1][1] * data[0][3];
    s[5] = data[0][2] * data[1][3] - data[1][2] * data[0][3];

    c[0] = data[2][0] * data[3][1] - data[3][0] * data[2][1];
    c[1] = data[2][0] * data[3][2] - data[3][0] * data[2][2];
    c[2] = data[2][0] * data[3][3] - data[3][0] * data[2][3];
    c[3] = data[2][1] * data[3][2] - data[3][1] * data[2][2];
    c[4] = data[2][1] * data[3][3] - data[3][1] * data[2][3];
    c[5] = data[2][2] * data[3][3] - data[3][2] * data[2][3];

    /* Assumes it is invertible */
    const float idet = 1.0f / (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]);

    ColumnMatrix4X4 T;

    T[0][0] = (data[1][1] * c[5] - data[1][2] * c[4] + data[1][3] * c[3]) * idet;
    T[0][1] = (-data[0][1] * c[5] + data[0][2] * c[4] - data[0][3] * c[3]) * idet;
    T[0][2] = (data[3][1] * s[5] - data[3][2] * s[4] + data[3][3] * s[3]) * idet;
    T[0][3] = (-data[2][1] * s[5] + data[2][2] * s[4] - data[2][3] * s[3]) * idet;

    T[1][0] = (-data[1][0] * c[5] + data[1][2] * c[2] - data[1][3] * c[1]) * idet;
    T[1][1] = (data[0][0] * c[5] - data[0][2] * c[2] + data[0][3] * c[1]) * idet;
    T[1][2] = (-data[3][0] * s[5] + data[3][2] * s[2] - data[3][3] * s[1]) * idet;
    T[1][3] = (data[2][0] * s[5] - data[2][2] * s[2] + data[2][3] * s[1]) * idet;

    T[2][0] = (data[1][0] * c[4] - data[1][1] * c[2] + data[1][3] * c[0]) * idet;
    T[2][1] = (-data[0][0] * c[4] + data[0][1] * c[2] - data[0][3] * c[0]) * idet;
    T[2][2] = (data[3][0] * s[4] - data[3][1] * s[2] + data[3][3] * s[0]) * idet;
    T[2][3] = (-data[2][0] * s[4] + data[2][1] * s[2] - data[2][3] * s[0]) * idet;

    T[3][0] = (-data[1][0] * c[3] + data[1][1] * c[1] - data[1][2] * c[0]) * idet;
    T[3][1] = (data[0][0] * c[3] - data[0][1] * c[1] + data[0][2] * c[0]) * idet;
    T[3][2] = (-data[3][0] * s[3] + data[3][1] * s[1] - data[3][2] * s[0]) * idet;
    T[3][3] = (data[2][0] * s[3] - data[2][1] * s[1] + data[2][2] * s[0]) * idet;

    return T;
}
