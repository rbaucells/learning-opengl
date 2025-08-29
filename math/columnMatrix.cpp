#include "mathematics.h"
#include <cmath>
#include <cstring>
#include <float.h>

ColumnMatrix::ColumnMatrix(const ColumnMatrix &other) {
    // mem copy
    memcpy(data, other.data, sizeof(float) * 16);
}

static ColumnMatrix ColumnMatrix::identity(int dimensions);
    /// 1, 0, 0, 0
    /// 0, 1, 0, 0
    /// 0, 0, 1, 0
    /// 0, 0, 0, 1

    ColumnMatrix result;

    for (int c = 0; c < 4; c++) {
        result.data[c][c] = 1;
    }

    return result;
}

ColumnMatrix ColumnMatrix::operator*(const ColumnMatrix &other) const {
    return multiply(other);
}

ColumnMatrix ColumnMatrix::operator*(const float value) const {
    return scale(value);
}

ColumnMatrix ColumnMatrix::operator+(const ColumnMatrix &other) const {
    return add(other);
}

ColumnMatrix ColumnMatrix::operator-(const ColumnMatrix &other) const {
    return subtract(other);
}

ColumnMatrix ColumnMatrix::add(const ColumnMatrix &other) const {
    ColumnMatrix result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result.data[c][r] = data[c][r] + other.data[c][r];
        }
    }

    return result;
}

ColumnMatrix ColumnMatrix::subtract(const ColumnMatrix &other) const {
    ColumnMatrix result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result.data[c][r] = data[c][r] - other.data[c][r];
        }
    }

    return result;
}

ColumnMatrix ColumnMatrix::multiply(const ColumnMatrix &other) const {
    // ColumnMatrix result;

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

    // ColumnMatrix result;

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

    ColumnMatrix result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            for (int x = 0; x < 4; x++) {
                result.data[c][r] += data[x][r] * other.data[c][x];
            }
        }
    }

    return result;
}

ColumnMatrix ColumnMatrix::scale(const float value) const {
    ColumnMatrix result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result.data[c][r] = data[c][r] * value;
        }
    }

    return result;
}

ColumnMatrix ColumnMatrix::scale_anisotropic(const float x, const float y, const float z) const {
    ColumnMatrix scaleMatrix = identity();

    scaleMatrix.data[0][0] = x;
    scaleMatrix.data[1][1] = y;
    scaleMatrix.data[2][2] = z;

    return multiply(scaleMatrix);
}

ColumnMatrix ColumnMatrix::transpose() const {
    ColumnMatrix result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result.data[c][r] = data[r][c];
        }
    }

    return result;
}

ColumnMatrix ColumnMatrix::translate(const float x, const float y, const float z) const {
    /// 1, 0, 0, x
    /// 0, 1, 0, y
    /// 0, 0, 1, z
    /// 0, 0, 0, 1
    ColumnMatrix translationMatrix = ColumnMatrix::identity();

    translationMatrix.data[3][0] = x;
    translationMatrix.data[3][1] = y;
    translationMatrix.data[3][2] = z;

    return translationMatrix;
}

ColumnMatrix ColumnMatrix::rotate_x(const float angle) const {
    const float sin = std::sinf(angle);
    const float cos = std::cosf(angle);

    ColumnMatrix rotationMatrix = ColumnMatrix::identity();

    rotationMatrix.data[1][1] = cos;
    rotationMatrix.data[1][2] = sin;
    rotationMatrix.data[2][1] = -sin;
    rotationMatrix.data[2][2] = cos;

    return multiply(rotationMatrix);
}

ColumnMatrix ColumnMatrix::rotate_y(const float angle) const {
    const float sin = std::sinf(angle);
    const float cos = std::cosf(angle);

    ColumnMatrix rotationMatrix = ColumnMatrix::identity();

    rotationMatrix.data[0][0] = cos;
    rotationMatrix.data[0][2] = -sin;
    rotationMatrix.data[2][0] = sin;
    rotationMatrix.data[2][2] = cos;

    return multiply(rotationMatrix);
}

ColumnMatrix ColumnMatrix::rotate_z(const float angle) const {
    const float sin = std::sinf(angle);
    const float cos = std::cosf(angle);

    ColumnMatrix rotationMatrix = ColumnMatrix::identity();

    rotationMatrix.data[0][0] = cos;
    rotationMatrix.data[0][1] = sin;
    rotationMatrix.data[1][0] = -sin;
    rotationMatrix.data[1][1] = cos;

    return multiply(rotationMatrix);
}

ColumnMatrix ColumnMatrix::ortho(const float left, const float right, const float bottom, const float top, const float near, const float far) const {
    // identity
    ColumnMatrix transformation = identity();
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

bool ColumnMatrix::compareTo(const ColumnMatrix &other) const {
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            if (std::abs(other.data[c][r] - data[c][r]) > FLT_EPSILON)
                return false;
        }
    }

    return true;
}

ColumnMatrix ColumnMatrix::wrap(mat4x4 other) {
    ColumnMatrix newColumnMatrix;
    memcpy(newColumnMatrix.data, other, sizeof(float) * 16);
    return newColumnMatrix;
}

vector4 ColumnMatrix::operator*(const vector4& other) const {
    vector4 result;

    result.x = data[0][0] * other.x + data[1][0] * other.y + data[2][0] * other.z + data[3][0] * other.w;
    result.y = data[0][1] * other.x + data[1][1] * other.y + data[2][1] * other.z + data[3][1] * other.w;
    result.z = data[0][2] * other.x + data[1][2] * other.y + data[2][2] * other.z + data[3][2] * other.w;
    result.w = data[0][3] * other.x + data[1][3] * other.y + data[2][3] * other.z + data[3][3] * other.w;

    return result;
}

vector2 ColumnMatrix::operator*(const vector2 &other) const {
    const vector4 result = operator*(vector4{other.x, other.y, 0, 1});

    return vector2{result.x, result.y};
}

float* ColumnMatrix::operator[](const int index) {
    return &data[index][0];
}

ColumnMatrix ColumnMatrix::inverse() const {
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

    ColumnMatrix T;

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
