#pragma once
#include <cassert>
#include <cfloat>
#include <iostream>
#include <sstream>

#include "linmath/linmath.h"

struct Vector2 {
    float x = {};
    float y = {};

    // operator overloads
    Vector2 operator+(const Vector2 &other) const;

    Vector2 operator-(const Vector2 &other) const;

    Vector2 operator*(float scalar) const;

    Vector2 operator/(float scalar) const;

    Vector2 &operator+=(const Vector2 &other);

    Vector2 &operator-=(const Vector2 &other);

    Vector2 &operator*=(float scalar);

    Vector2 &operator/=(float scalar);

    bool operator==(const Vector2 &other) const;

    // functions
    float Magnitude() const;

    Vector2 Normalized() const;

    Vector2 hadamard(const Vector2 &other) const;

    float Dot(const Vector2 &other) const;
};

struct Vector3 {
    float x = {};
    float y = {};
    float z = {};

    // operator overloads
    Vector3 operator+(const Vector3 &other) const;

    Vector3 operator-(const Vector3 &other) const;

    Vector3 operator*(float scalar) const;

    Vector3 operator/(float scalar) const;

    Vector3 &operator+=(const Vector3 &other);

    Vector3 &operator-=(const Vector3 &other);

    Vector3 &operator*=(float scalar);

    Vector3 &operator/=(float scalar);

    float magnitude() const;

    Vector3 normalized() const;

    float dot(const Vector3 &other) const;

    Vector3 cross(const Vector3 &other) const;
};

struct Vector4 {
    float x = {};
    float y = {};
    float z = {};
    float w = {};

    Vector4 operator+(const Vector4 &other) const;

    Vector4 operator-(const Vector4 &other) const;

    Vector4 operator*(float scalar) const;

    Vector4 operator/(float scalar) const;

    Vector4 &operator+=(const Vector4 &other);

    Vector4 &operator-=(const Vector4 &other);

    Vector4 &operator*=(float scalar);

    Vector4 &operator/=(float scalar);

    float magnitude() const;

    Vector4 normalized() const;

    float dot(const Vector4 &other) const;
};

struct RowMatrix4X4 {
    RowMatrix4X4() = default;

    RowMatrix4X4(const RowMatrix4X4 &other);

    static RowMatrix4X4 identity();

    RowMatrix4X4 operator*(const RowMatrix4X4 &other) const;

    RowMatrix4X4 operator*(float value) const;

    RowMatrix4X4 operator+(const RowMatrix4X4 &other) const;

    RowMatrix4X4 operator-(const RowMatrix4X4 &other) const;

    RowMatrix4X4 multiply(const RowMatrix4X4 &other) const;

    RowMatrix4X4 scale(float value) const;

    RowMatrix4X4 scale_anisotropic(float x, float y, float z) const;

    RowMatrix4X4 add(const RowMatrix4X4 &other) const;

    RowMatrix4X4 subtract(const RowMatrix4X4 &other) const;

    RowMatrix4X4 transpose() const;

    RowMatrix4X4 translate(float x, float y, float z) const;

    RowMatrix4X4 rotate_x(float angle) const;

    RowMatrix4X4 rotate_y(float angle) const;

    RowMatrix4X4 rotate_z(float angle) const;

    static RowMatrix4X4 ortho(float left, float right, float bottom, float top, float near, float far);

    float data[4][4] = {{0}, {0}, {0}, {0}};
};

struct ColumnMatrix4X4 {
    ColumnMatrix4X4() = default;

    ColumnMatrix4X4(const ColumnMatrix4X4 &other);

    static ColumnMatrix4X4 identity();

    ColumnMatrix4X4 operator*(const ColumnMatrix4X4 &other) const;

    ColumnMatrix4X4 operator*(float value) const;

    Vector4 operator*(const Vector4 &other) const;

    Vector2 operator*(const Vector2 &other) const;

    [[nodiscard]] ColumnMatrix4X4 operator+(const ColumnMatrix4X4 &other) const;

    [[nodiscard]] ColumnMatrix4X4 operator-(const ColumnMatrix4X4 &other) const;

    [[nodiscard]] ColumnMatrix4X4 multiply(const ColumnMatrix4X4 &other) const;

    [[nodiscard]] ColumnMatrix4X4 scale(float value) const;

    [[nodiscard]] ColumnMatrix4X4 scale_anisotropic(float x, float y, float z) const;

    [[nodiscard]] ColumnMatrix4X4 add(const ColumnMatrix4X4 &other) const;

    [[nodiscard]] ColumnMatrix4X4 subtract(const ColumnMatrix4X4 &other) const;

    [[nodiscard]] ColumnMatrix4X4 transpose() const;

    [[nodiscard]] ColumnMatrix4X4 translate(float x, float y, float z) const;

    [[nodiscard]] ColumnMatrix4X4 rotate_x(float angle) const;

    [[nodiscard]] ColumnMatrix4X4 rotate_y(float angle) const;

    [[nodiscard]] ColumnMatrix4X4 rotate_z(float angle) const;

    [[nodiscard]] ColumnMatrix4X4 ortho(float left, float right, float bottom, float top, float near, float far) const;

    [[nodiscard]] ColumnMatrix4X4 inverse() const;

    float *operator[](int index);

    explicit operator const float *() const;
    explicit operator float *();

    [[nodiscard]] bool compareTo(const ColumnMatrix4X4 &other) const;

    static ColumnMatrix4X4 wrap(mat4x4 other);

    float data[4][4] = {{0}, {0}, {0}, {0}};

    /**
 *
 * @return The column-major matrix as a string for printing
 */
    [[nodiscard]] std::string toString() const {
        std::stringstream ss;
        ss.precision(2);

        for (int r = 0; r < 4; r++) {
            ss << " [";

            for (int c = 0; c < 4; c++) {
                ss << data[c][r];

                if (c < 4 - 1)
                    ss << ", ";
            }

            ss << "]";

            if (r < 4 - 1)
                ss << ",\n";
        }

        ss << "\n]";
        ss << "\n";
        return ss.str();
    }
};

struct Vertex {
    Vector2 position;
    Vector2 uv;
};

struct Buffers {
    unsigned int vertexBuffer;
    unsigned int indexBuffer;
};

struct SystemOfEquations;

template<int ROWS, int COLUMNS>
class Matrix {
public:
    const int rows = ROWS;
    const int columns = COLUMNS;

    float data[COLUMNS][ROWS] = {};

    [[nodiscard]] Matrix<ROWS, COLUMNS> add(const Matrix<ROWS, COLUMNS> &other) const {
        static_assert(other.rows == ROWS || other.columns == COLUMNS);

        Matrix result;

        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                result[c][r] = data[c][r] + other.data[c][r];
            }
        }

        return result;
    }

    [[nodiscard]] Matrix<ROWS, COLUMNS> subtract(const Matrix<ROWS, COLUMNS> &other) const {
        static_assert(other.rows == ROWS || other.columns == COLUMNS);

        Matrix result;

        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                result[c][r] = data[c][r] - other.data[c][r];
            }
        }

        return result;
    }

    Matrix<ROWS, COLUMNS> operator+(const Matrix<ROWS, COLUMNS> &other) const {
        return add(other);
    }

    Matrix<ROWS, COLUMNS> operator-(const Matrix<ROWS, COLUMNS> &other) const {
        return subtract(other);
    }

    /**
   * @brief Multiplies this matrix by other. returning a matrix of size this rows, and other columns
   * @param other Matrix to multiply this * other
   * @return The product of the two matrix. Having Number of rows as this, and number of columns as other
   */
    template<int OTHER_ROWS, int OTHER_COLUMNS>
    [[nodiscard]] Matrix<ROWS, OTHER_COLUMNS> multiply(const Matrix<OTHER_ROWS, OTHER_COLUMNS> &other) const {
        assert(COLUMNS == other.rows);

        Matrix<ROWS, OTHER_COLUMNS> result;

        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                for (int x = 0; x < COLUMNS; x++) {
                    result[c][r] += data[x][r] * other.data[c][x];
                }
            }
        }

        return result;
    }

    bool operator==(const Matrix &other) const {
        return compare(other);
    }

    bool compare(const Matrix &other) {
        static_assert(other.rows == ROWS || other.columns == COLUMNS);

        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                if (std::abs(other.data[c][r] - data[c][r]) > FLT_EPSILON)
                    return false;
            }
        }

        return true;
    }

    float *operator[](const int index) {
        return &data[index][0];
    }

    const float *operator[](const int index) const {
        return &data[index][0];
    }

    [[nodiscard]] Matrix<ROWS - 1, COLUMNS - 1> getSubMatrix(const int rowToRemove, const int columnToRemove) const {
        Matrix<ROWS - 1, COLUMNS - 1> subMatrix;
        int subMatrixR = 0;
        for (int r = 0; r < ROWS; r++) {
            if (r == rowToRemove)
                continue;

            int subMatrixC = 0;
            for (int c = 0; c < COLUMNS; c++) {
                if (c == columnToRemove)
                    continue;

                subMatrix.data[subMatrixC][subMatrixR] = data[c][r];
                subMatrixC++;
            }
            subMatrixR++;
        }

        return subMatrix;
    }

#if ROWS == COLUMNS // if its a square matrix
    /**
     * @brief Swaps elements on row r and column c to row c and column r (Reflection across main diagonal)
     * @return Matrix with columns and rows swapped
     */
    [[nodiscard]] Matrix<ROWS, COLUMNS> transpose() const {
        Matrix<ROWS, ROWS> result;

        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                result[c][r] = data[r][c];
            }
        }

        return result;
    }

    /**
     * @brief Creates augmented matrix with this matrix and identity matrix and turns this into identity with simple row operations
     * @return The left side of the augmented matrix. Think 1 / this matrix
     */
    [[nodiscard]] Matrix<ROWS, COLUMNS> inverse() const {
        // its a one by one, we can just return 1 / value
        if constexpr (ROWS == 1) {
            if (data[0][0] == 0) {
                std::__throw_runtime_error("Determinant cannot be zero");
            }
            Matrix<1, 1> result;

            result[0][0] = 1 / data[0][0];
            return result;
        }

        // its a two by two, we can do the special fast thing
        if constexpr (ROWS == 2) {
            float det = determinant();

            if (det == 0) {
                std::__throw_runtime_error("Determinant cannot be zero");
            }

            Matrix<2, 2> result;

            result[0][0] = data[1][1] / det;
            result[0][1] = -data[0][1] / det;
            result[1][0] = -data[1][0] / det;
            result[1][1] = data[0][0] / det;

            return result;
        }
        else {
            const Matrix identityMatrix = Matrix::identity<ROWS>();
            Matrix<ROWS, COLUMNS * 2> temp;

            for (int c = 0; c < COLUMNS; c++) {
                for (int r = 0; r < ROWS; r++) {
                    temp[c][r] = data[c][r];
                }
            }

            for (int c = 0; c < COLUMNS; c++) {
                for (int r = 0; r < ROWS; r++) {
                    temp[c + COLUMNS][r] = identityMatrix[c][r];
                }
            }

            for (int c = 0; c < COLUMNS; c++) {
                // if the pivot is zero
                if (temp[c][c] == 0) {
                    int biggestRow = 0;
                    int r;
                    for (r = 0; r < ROWS; r++) {
                        if (temp[c][r] != 0 && std::abs(temp[c][r]) > std::abs(temp[c][biggestRow])) {
                            biggestRow = r;
                        }
                    }

                    if (temp[c][biggestRow] == 0 || r == ROWS) {
                        std::__throw_runtime_error("Cannot get inverse of Matrix, determinant is zero");
                    }

                    // since c (column) = r (row), the dest row is at temp[row (aka, c}] the src row is the memory address of the start of the biggest row so &temp[c][biggestRow]
                    swapRows(temp, c, biggestRow);
                }

                // normalize pivot row to 1
                float value = temp[c][c];
                for (int cc = 0; cc < temp.columns; cc++) {
                    temp[cc][c] /= value;
                }

                for (int r = 0; r < ROWS; r++) {
                    // the current value is on the main diagonal. (expected to be a 1). Should be good
                    if (r == c) {
                        // we dont want to affect the pivot
                        continue;
                    }
                    // it isnt on the main diagonal, (expected to be a 0)
                    float k = temp[c][r];

                    for (int cc = 0; cc < temp.columns; cc++) {
                        temp[cc][r] += -k * temp[cc][c];
                    }
                }
            }

            Matrix<ROWS, COLUMNS> result;

            for (int c = 0; c < COLUMNS; ++c) {
                for (int r = 0; r < ROWS; r++) {
                    result[c][r] = temp[c + COLUMNS][r];
                }
            }

            return result;
        }
    }

    /**
     * @warning if zero do not attempt to find inverse
     * @return The determinant of this matrix
     */
    [[nodiscard]] float determinant() const {
        if constexpr (ROWS == 1) {
            return data[0][0];
        }
        else if constexpr (ROWS == 2) {
            return data[0][0] * data[1][1] - data[0][1] * data[1][0];
        }
        else {
            float result = 0;
            int sign = 1;

            for (int c = 0; c < COLUMNS; c++) {
                Matrix<ROWS - 1, COLUMNS - 1> insideMatrix = getSubMatrix(0, c);

                result += sign * data[c][0] * insideMatrix.determinant();
                sign *= -1;
            }

            return result;
        }
    }

    #if COLUMNS == 4 // if its a 4x4
    Matrix<ROWS, COLUMNS> scale(const float value) const {
        Matrix<ROWS, COLUMNS> result;

        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                result.data[c][r] = data[c][r] * value;
            }
        }

        return result;
    }

    Matrix<ROWS, COLUMNS> scale_anisotropic(const float x, const float y, const float z) const {
        Matrix<ROWS, COLUMNS> scaleMatrix = identity();

        scaleMatrix.data[0][0] = x;
        scaleMatrix.data[1][1] = y;
        scaleMatrix.data[2][2] = z;

        return multiply(scaleMatrix);
    }

    Matrix<ROWS, COLUMNS> translate(const float x, const float y, const float z) const {
        Matrix<ROWS, COLUMNS> translationMatrix = identity();

        translationMatrix.data[3][0] = x;
        translationMatrix.data[3][1] = y;
        translationMatrix.data[3][2] = z;

        return multiply(translationMatrix);
    }

    Matrix<ROWS, COLUMNS> rotate_x(const float angle) const {
        const float sin = std::sinf(angle * (static_cast<float>(M_PI) / 180.0f));
        const float cos = std::cosf(angle * (static_cast<float>(M_PI) / 180.0f));

        Matrix<ROWS, COLUMNS> rotationMatrix = Matrix::identity<ROWS>();

        rotationMatrix.data[1][1] = cos;
        rotationMatrix.data[1][2] = sin;
        rotationMatrix.data[2][1] = -sin;
        rotationMatrix.data[2][2] = cos;

        return multiply(rotationMatrix);
    }

    Matrix<ROWS, COLUMNS> rotate_y(const float angle) const {
        const float sin = std::sinf(angle * (static_cast<float>(M_PI) / 180.0f));
        const float cos = std::cosf(angle * (static_cast<float>(M_PI) / 180.0f));

        Matrix<ROWS, COLUMNS> rotationMatrix = Matrix::identity<ROWS>();

        rotationMatrix.data[0][0] = cos;
        rotationMatrix.data[0][2] = -sin;
        rotationMatrix.data[2][0] = sin;
        rotationMatrix.data[2][2] = cos;

        return multiply(rotationMatrix);
    }

    Matrix<ROWS, COLUMNS> rotate_z(const float angle) const {
        const float sin = std::sinf(angle * (static_cast<float>(M_PI) / 180.0f));
        const float cos = std::cosf(angle * (static_cast<float>(M_PI) / 180.0f));

        Matrix<ROWS, COLUMNS> rotationMatrix = Matrix::identity<ROWS>();

        rotationMatrix.data[0][0] = cos;
        rotationMatrix.data[0][1] = sin;
        rotationMatrix.data[1][0] = -sin;
        rotationMatrix.data[1][1] = cos;

        return multiply(rotationMatrix);
    }

    Matrix<ROWS, COLUMNS> ortho(const float left, const float right, const float bottom, const float top, const float near, const float far) const {
        // identity
        Matrix<ROWS, COLUMNS> transformation = Matrix::identity<ROWS>();
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
    #endif
#endif

    /**
     *
     * @return The column-major matrix as a string for printing
     */
    [[nodiscard]] std::string toString() const {
        std::stringstream ss;
        ss.precision(2);

        for (int r = 0; r < ROWS; r++) {
            ss << " [";

            for (int c = 0; c < COLUMNS; c++) {
                ss << data[c][r];

                if (c < COLUMNS - 1)
                    ss << ", ";
            }

            ss << "]";

            if (r < ROWS - 1)
                ss << ",\n";
        }

        ss << "\n]";
        ss << "\n";
        return ss.str();
    }

    // static methods
    /**
     *
     * @tparam SIZE How many rows/columns should the produced identity have
     * @return Matrix with 1s on main diagonal and 0s elsewhere of SIZExSIZE
     */
    template<int SIZE>
    static Matrix<SIZE, SIZE> identity() {
        Matrix<SIZE, SIZE> result;

        for (int i = 0; i < SIZE; i++) {
            result[i][i] = 1;
        }

        return result;
    }

    /**
 *
 * @tparam R_SIZE How many rows
 * @tparam C_SIZE How many columns
 * @param matrix Where the data will be written
 * @param rowA Index of row a
 * @param rowB
 */
    template<int R_SIZE, int C_SIZE>
    static void swapRows(Matrix<R_SIZE, C_SIZE> &matrix, const int rowA, const int rowB) {
        // int temp = a;
        // a = b;
        // b = temp;

        float temp[C_SIZE];

        for (int c = 0; c < C_SIZE; c++) {
            temp[c] = matrix[c][rowA];
        }

        for (int c = 0; c < C_SIZE; c++) {
            matrix[c][rowA] = matrix[c][rowB];
        }

        for (int c = 0; c < C_SIZE; c++) {
            matrix[c][rowB] = temp[c];
        }
    }

    static Matrix solveSystemOfEquations(const Matrix<2, 2> &coefficients, const Matrix<2, 1> &result) {
        // TODO: we need to return coefficients inverse times result and that will give variables
        const Matrix<2, 2> coefficientsInverse = coefficients.inverse();
        Matrix<2, 1> values = coefficientsInverse.multiply(result);

        return values;
    }
};
