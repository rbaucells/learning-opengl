#include "Math.h"

matrix4x4::matrix4x4(const matrix4x4& other) {
    copy(other);
}

matrix4x4::matrix4x4(const float other[4][4]) {
    copy(other);
}


float* matrix4x4::operator[](int i) {
    return data[i];
}

const float* matrix4x4::operator[](int i) const {
    return data[i];
}

/// sets the matrix to the identity matrix
void matrix4x4::identity() {
    // generates the identity matrix
    // [1, 0, 0, 0]
    // [0, 1, 0, 0]
    // [0, 0, 1, 0]
    // [0, 0, 0, 1]

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (x == y)
                data[x][y] = 1;
            else
                data[x][y] = 0;
        }
    }
}

/// copies other's data into matrix
void matrix4x4::copy(const matrix4x4& other) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            data[x][y] = other.data[x][y];
        }
    }
}

/// copies other's data into matrix
void matrix4x4::copy(const float other[4][4]) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            data[x][y] = other[x][y];
        }
    }
}

/// transposes the matrix. puts columns in rows and rows in columns
void matrix4x4::transpose() {
    // make a copy
    const matrix4x4 temp(this -> data);
    // loop through it and copy data
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            data[x][y] = temp.data[y][x];
        }
    }

    copy(temp);
}

/// multiplies the matrix by other
void matrix4x4::multiplicity(const matrix4x4& other) {
    float temp[4][4];

    for (int row = 0; row < 4; row++) {
        for (int column = 0; column < 4; column++) {
            for (int k = 0; k < 4; k++) {
                temp[row][column] += data[row][k] * other[k][column];
            }
        }
    }

    copy(temp);
}

/// multiplies the matrix by other
void matrix4x4::multiplicity(const float other[4][4]) {
    float temp[4][4];

    for (int row = 0; row < 4; row++) {
        for (int column = 0; column < 4; column++) {
            for (int k = 0; k < 4; k++) {
                temp[row][column] += data[row][k] * other[k][column];
            }
        }
    }

    copy(temp);
}

/// sets the matrix's value to other * other_other
void matrix4x4::multiplicity(const float other[4][4], const float other_other[4][4]) {
    float temp[4][4];

    for (int row = 0; row < 4; row++) {
        for (int column = 0; column < 4; column++) {
            for (int k = 0; k < 4; k++) {
                temp[row][column] += other[row][k] * other_other[k][column];
            }
        }
    }

    copy(temp);
}

/// sets the matrix's value to other * other_other
void matrix4x4::multiplicity(const matrix4x4 &other, const matrix4x4 &other_other) {
    float temp[4][4];

    for (int row = 0; row < 4; row++) {
        for (int column = 0; column < 4; column++) {
            for (int k = 0; k < 4; k++) {
                temp[row][column] += other[row][k] * other_other[k][column];
            }
        }
    }

    copy(temp);
}

/// scales the matrix anisotropically
void matrix4x4::scale_anisotropic(const float x, const float y, const float z) {
    float scalingMatrix[4][4];

    scalingMatrix[0][0] = x;
    scalingMatrix[1][1] = y;
    scalingMatrix[2][2] = z;
    scalingMatrix[3][3] = 1;

    multiplicity(scalingMatrix);
}

/// translates the matrix to x, y, z
void matrix4x4::translate(const float x, const float y, const float z) {
    identity();

    data[3][0] = x;
    data[3][1] = y;
    data[3][2] = z;
}

/// rotates matrix by angle along z axis
void matrix4x4::rotate_z(const float angle) {
    float sinOfAngle = sinf(angle);
    float cosOfAngle = cosf(angle);

    float rotationMatrix[4][4] = {
        {   cosOfAngle,   sinOfAngle, 0, 0},
        {  -sinOfAngle,   cosOfAngle, 0, 0},
        { 0, 0, 1, 0},
        { 0, 0, 0, 1}
    };

    multiplicity(rotationMatrix);
}

/// orthogonalizes the matrix
void matrix4x4::orthogonalize(const float left, const float right, const float bottom, const float top, const float near, const float far) {
    data[0][0] = 2/ (right-left);
    data[0][1] = data[0][2] = data[0][3] = 0;

    data[1][1] = 2 / (top-bottom);
    data[1][0] = data[1][2] = data[1][3] = 0;

    data[2][2] = -2 / (far-near);
    data[2][0] = data[2][1] = data[2][3] = 0;

    data[3][0] = -(right+left) / (right-left);
    data[3][1] = -(top+bottom) / (top-bottom);
    data[3][2] = -(far+near) / (far-near);
    data[3][3] = 1;
}





