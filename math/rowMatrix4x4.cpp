#include "mathematics.h"
#include <cmath>

rowMatrix4x4::rowMatrix4x4(const rowMatrix4x4 &other) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            data[r][c] = other.data[r][c];
        }
    }
}


rowMatrix4x4 rowMatrix4x4::identity() {
    /// 1, 0, 0, 0
    /// 0, 1, 0, 0
    /// 0, 0, 1, 0
    /// 0, 0, 0, 1

    rowMatrix4x4 result;

    for (int r = 0; r < 4; r++) {
        result.data[r][r] = 1;
    }

    return result;
}
rowMatrix4x4 rowMatrix4x4::operator*(const rowMatrix4x4 &other) const {
    return multiply(other);
}
rowMatrix4x4 rowMatrix4x4::operator*(const float value) const {
    return scale(value);
}
rowMatrix4x4 rowMatrix4x4::operator+(const rowMatrix4x4 &other) const {
    return add(other);
}
rowMatrix4x4 rowMatrix4x4::operator-(const rowMatrix4x4 &other) const {
    return subtract(other);
}

rowMatrix4x4 rowMatrix4x4::add(const rowMatrix4x4 &other) const {
    rowMatrix4x4 result;

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            result.data[r][c] = data[r][c] + other.data[r][c];
        }
    }

    return result;
}
rowMatrix4x4 rowMatrix4x4::subtract(const rowMatrix4x4 &other) const {
    rowMatrix4x4 result;

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            result.data[r][c] = data[r][c] - other.data[r][c];
        }
    }

    return result;
}

rowMatrix4x4 rowMatrix4x4::multiply(const rowMatrix4x4 &other) const {
    /// 1, 0, 0, 0
    /// 0, 1, 0, 0
    /// 0, 0, 1, 0
    /// 0, 0, 0, 1

    rowMatrix4x4 result;

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            for (int x = 0; x < 4; x++) {
                result.data[r][c] += data[r][x] * other.data[x][c];
            }
        }
    }

    return result;
}
rowMatrix4x4 rowMatrix4x4::scale(const float value) const {
    rowMatrix4x4 result;

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            result.data[r][c] = data[r][c] * value;
        }
    }

    return result;
}
rowMatrix4x4 rowMatrix4x4::scale_anisotropic(const float x, const float y, const float z) const {
    rowMatrix4x4 scaleMatrix = rowMatrix4x4::identity();

    scaleMatrix.data[0][0] = x;
    scaleMatrix.data[1][1] = y;
    scaleMatrix.data[2][2] = z;

    return multiply(scaleMatrix);
}

rowMatrix4x4 rowMatrix4x4::transpose() const {
    rowMatrix4x4 result;

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            result.data[r][c] = data[c][r];
        }
    }

    return result;
}
rowMatrix4x4 rowMatrix4x4::translate(const float x, const float y, const float z) const {
    /// 1, 0, 0, 0
    /// 0, 1, 0, 0
    /// 0, 0, 1, 0
    /// x, y, z, 1
    rowMatrix4x4 translationMatrix = rowMatrix4x4::identity();

    translationMatrix.data[3][0] = x;
    translationMatrix.data[3][1] = y;
    translationMatrix.data[3][2] = z;

    return multiply(translationMatrix);
}

rowMatrix4x4 rowMatrix4x4::rotate_x(const float angle) const {
    const float sin = std::sin(angle);
    const float cos = std::cos(angle);

    rowMatrix4x4 rotationMatrix = rowMatrix4x4::identity();

    rotationMatrix.data[1][1] = cos;
    rotationMatrix.data[1][2] = -sin;
    rotationMatrix.data[2][1] = sin;
    rotationMatrix.data[2][2] = cos;

    return multiply(rotationMatrix);
}
rowMatrix4x4 rowMatrix4x4::rotate_y(const float angle) const {
    const float sin = std::sin(angle);
    const float cos = std::cos(angle);

    rowMatrix4x4 rotationMatrix = rowMatrix4x4::identity();

    rotationMatrix.data[0][0] = cos;
    rotationMatrix.data[0][2] = sin;
    rotationMatrix.data[2][0] = -sin;
    rotationMatrix.data[2][2] = cos;

    return multiply(rotationMatrix);
}
rowMatrix4x4 rowMatrix4x4::rotate_z(const float angle) const {
    const float sin = std::sin(angle);
    const float cos = std::cos(angle);

    rowMatrix4x4 rotationMatrix = rowMatrix4x4::identity();

    rotationMatrix.data[0][0] = cos;
    rotationMatrix.data[0][1] = -sin;
    rotationMatrix.data[1][0] = sin;
    rotationMatrix.data[1][1] = cos;

    return multiply(rotationMatrix);
}

rowMatrix4x4 rowMatrix4x4::ortho(const float left, const float right, const float bottom, const float top, const float near, const float far) {
    rowMatrix4x4 orthoMatrix = rowMatrix4x4::identity();

    orthoMatrix.data[0][0] = (2.0f / right - left);
    orthoMatrix.data[1][1] = (2.0f / top - bottom);
    orthoMatrix.data[2][2] = (-2.0f / far - near);

    const float tx = -((right + left) / (right - left));
    const float ty = -((top + bottom) / (top - bottom));
    const float tz = -((far + near) / (far - near));

    orthoMatrix.data[3][0] = tx;
    orthoMatrix.data[3][1] = ty;
    orthoMatrix.data[3][2] = tz;

    return orthoMatrix;
}