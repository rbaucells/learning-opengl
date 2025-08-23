#include "mathematics.h"
#include <cmath>
#include <cstring>

columnMatrix4x4::columnMatrix4x4(const columnMatrix4x4 &other) {
    // mem copy
    memcpy(data, other.data, sizeof(float) * 16);
}

columnMatrix4x4 columnMatrix4x4::identity() {
    /// 1, 0, 0, 0
    /// 0, 1, 0, 0
    /// 0, 0, 1, 0
    /// 0, 0, 0, 1

    columnMatrix4x4 result;

    for (int c = 0; c < 4; c++) {
        result.data[c][c] = 1;
    }

    return result;
}

columnMatrix4x4 columnMatrix4x4::operator*(const columnMatrix4x4 &other) const {
    return multiply(other);
}

columnMatrix4x4 columnMatrix4x4::operator*(const float value) const {
    return scale(value);
}

columnMatrix4x4 columnMatrix4x4::operator+(const columnMatrix4x4 &other) const {
    return add(other);
}

columnMatrix4x4 columnMatrix4x4::operator-(const columnMatrix4x4 &other) const {
    return subtract(other);
}

columnMatrix4x4 columnMatrix4x4::add(const columnMatrix4x4 &other) const {
    columnMatrix4x4 result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result.data[c][r] = data[c][r] + other.data[c][r];
        }
    }

    return result;
}

columnMatrix4x4 columnMatrix4x4::subtract(const columnMatrix4x4 &other) const {
    columnMatrix4x4 result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result.data[c][r] = data[c][r] - other.data[c][r];
        }
    }

    return result;
}

columnMatrix4x4 columnMatrix4x4::multiply(const columnMatrix4x4 &other) const {
    // ColumnMatrix4x4 result;

    // for (int r = 0; r < 4; r++) {
    //     float left[4];
    //     for (int c = 0; c < 4; c++) {
    //         left[c] = data[c][r];
    //         const float *right = &other.data[c][0];
    //         float value = 0.0;
    //         // dot mult
    //         for (int i = 0; i < 4; i++) {
    //             value += left[i] * right[i];
    //         }
    //         result.data[c][r] = value;
    //     }
    // }
    // return result;

    // ColumnMatrix4x4 result;

    // for (int r = 0; r < 4; r++) {
    //     float row[4];
    //     for (int c = 0; c < 4; c++) {
    //         const float* column = other.data[0];
    //         row[c] = data[c][r];
    //         float value = 0;
    //
    //         for (int i = 0; i < 4; i++) {
    //             value += row[i] * column[i];
    //         }
    //
    //         result.data[c][r] = value;
    //     }
    // }

    // return result;

    /// 1, 0, 0, 0
    /// 0, 1, 0, 0
    /// 0, 0, 1, 0
    /// 0, 0, 0, 1

    columnMatrix4x4 result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            for (int x = 0; x < 4; x++) {
                result.data[c][r] += data[x][r] * other.data[c][x];
            }
        }
    }

    return result;
}

columnMatrix4x4 columnMatrix4x4::scale(const float value) const {
    columnMatrix4x4 result;
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result.data[c][r] = data[c][r] * value;
        }
    }
    return result;
}

columnMatrix4x4 columnMatrix4x4::scale_anisotropic(const float x, const float y, const float z) const {
    columnMatrix4x4 scaleMatrix = columnMatrix4x4::identity();

    scaleMatrix.data[0][0] = x;
    scaleMatrix.data[1][1] = y;
    scaleMatrix.data[2][2] = z;

    return multiply(scaleMatrix);
}

columnMatrix4x4 columnMatrix4x4::transpose() const {
    columnMatrix4x4 result;

    for (int column = 0; column < 4; column++) {
        for (int row = 0; row < 4; row++) {
            result.data[column][row] = data[row][column];
        }
    }

    return result;
}

columnMatrix4x4 columnMatrix4x4::translate(const float x, const float y, const float z) const {
    /// 1, 0, 0, x
    /// 0, 1, 0, y
    /// 0, 0, 1, z
    /// 0, 0, 0, 1
    columnMatrix4x4 translationMatrix = columnMatrix4x4::identity();

    translationMatrix.data[3][0] = x;
    translationMatrix.data[3][1] = y;
    translationMatrix.data[3][2] = z;

    return multiply(translationMatrix);
}

columnMatrix4x4 columnMatrix4x4::rotate_x(const float angle) const {
    const float sin = std::sin(angle);
    const float cos = std::cos(angle);

    columnMatrix4x4 rotationMatrix = columnMatrix4x4::identity();

    rotationMatrix.data[1][1] = cos;
    rotationMatrix.data[1][2] = sin;
    rotationMatrix.data[2][1] = -sin;
    rotationMatrix.data[2][2] = cos;

    return multiply(rotationMatrix);
}

columnMatrix4x4 columnMatrix4x4::rotate_y(const float angle) const {
    const float sin = std::sin(angle);
    const float cos = std::cos(angle);

    columnMatrix4x4 rotationMatrix = columnMatrix4x4::identity();

    rotationMatrix.data[0][0] = cos;
    rotationMatrix.data[0][2] = -sin;
    rotationMatrix.data[2][0] = sin;
    rotationMatrix.data[2][2] = cos;

    return multiply(rotationMatrix);
}

columnMatrix4x4 columnMatrix4x4::rotate_z(const float angle) const {
    const float sin = std::sin(angle);
    const float cos = std::cos(angle);

    columnMatrix4x4 rotationMatrix = columnMatrix4x4::identity();

    rotationMatrix.data[0][0] = cos;
    rotationMatrix.data[0][1] = sin;
    rotationMatrix.data[1][0] = -sin;
    rotationMatrix.data[1][1] = cos;

    return multiply(rotationMatrix);
}

columnMatrix4x4 columnMatrix4x4::ortho(const float left, const float right, const float bottom, const float top, const float near, const float far) const {
    // identity
    columnMatrix4x4 transformation = identity();
    // transformation
    transformation.data[0][0] = 2.0f / (right - left);
    transformation.data[1][1] = 2.0f / (top - bottom);
    transformation.data[2][2] = -2.0f / (far - near);
    transformation.data[3][0] = -(right + left) / (right - left);
    transformation.data[3][1] = -(top + bottom) / (top - bottom);
    transformation.data[3][2] = -(far + near) / (far - near);
    // multiply this * transformation
    return multiply(transformation);
}

bool columnMatrix4x4::compareTo(const columnMatrix4x4 &other) const {
    return memcmp(data, other.data, sizeof(float) * 16) == 0;
}

columnMatrix4x4 columnMatrix4x4::wrap(float* other) {
    columnMatrix4x4 newColumnMatrix4x4;
    memcpy(other, newColumnMatrix4x4.data, sizeof(float) * 16);
    return newColumnMatrix4x4;
}
