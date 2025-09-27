#pragma once
#include <cassert>
#include <cfloat>
#include <sstream>
#include <stdexcept>
#include <string>
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

template<int ROWS, int COLUMNS>
class Matrix {
public:
    const int rows = ROWS;
    const int columns = COLUMNS;

    float data[COLUMNS][ROWS] = {};

    Matrix() = default;

    Matrix(const Matrix<ROWS, COLUMNS> &other) {
        assert(ROWS == other.rows && COLUMNS == other.columns);

        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                data[c][r] = other.data[c][r];
            }
        }
    }

    Matrix<ROWS, COLUMNS> &operator=(const Matrix &other) {
        if (this != &other) {
            for (int c = 0; c < COLUMNS; c++) {
                for (int r = 0; r < ROWS; r++) {
                    data[c][r] = other.data[c][r];
                }
            }
        }

        return *this;
    }

    /**
     * @brief Adds this matrix plus other (this + other)
     * @note this rows MUST equal other rows and this columns MUST equal other columns
     * @param other The matrix to add to this one
     * @return Matrix with each element in this one added to the corresponding element in the other one
     */
    [[nodiscard]] Matrix<ROWS, COLUMNS> add(const Matrix<ROWS, COLUMNS> &other) const {
        assert(other.rows == ROWS || other.columns == COLUMNS);

        Matrix result;

        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                result[c][r] = data[c][r] + other.data[c][r];
            }
        }

        return result;
    }

    Matrix<ROWS, COLUMNS> operator+(const Matrix<ROWS, COLUMNS> &other) const {
        return add(other);
    }

    /**
     *@brief Subtracts other from this (this - other)
     * @note this rows MUST equal other rows and this columns MUST equal other columns
     * @param other The matrix to subtract from this one
     * @return Matrix with each element in other one subtracted from the corresponding element in the this one
     */
    [[nodiscard]] Matrix<ROWS, COLUMNS> subtract(const Matrix<ROWS, COLUMNS> &other) const {
        assert(other.rows == ROWS || other.columns == COLUMNS);

        Matrix result;

        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                result[c][r] = data[c][r] - other.data[c][r];
            }
        }

        return result;
    }

    Matrix<ROWS, COLUMNS> operator-(const Matrix<ROWS, COLUMNS> &other) const {
        return subtract(other);
    }

    /**
   * @brief Multiplies this matrix by other. (this x other)
   * @param other Matrix to multiply this * other
   * @return The product of the two matrix. Having Number of rows as this, and number of columns as other
   */
    template<int OTHER_ROWS, int OTHER_COLUMNS>
    Matrix<ROWS, OTHER_COLUMNS> multiply(const Matrix<OTHER_ROWS, OTHER_COLUMNS> &other) const {
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

    template<int OTHER_ROWS, int OTHER_COLUMNS>
    Matrix<ROWS, OTHER_COLUMNS> operator*(const Matrix<OTHER_ROWS, OTHER_COLUMNS> &other) const {
        return multiply(other);
    }

    /**
     * @brief Compares this matrix with the other (this == other)
     * @param other The Matrix to compare against
     * @return Weather or not the two matrices have the same data
     */
    [[nodiscard]] bool compare(const Matrix &other) const {
        if (other.rows != ROWS || other.columns != COLUMNS)
            return false;

        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                if (std::abs(other.data[c][r] - data[c][r]) > FLT_EPSILON)
                    return false;
            }
        }

        return true;
    }

    bool operator==(const Matrix &other) const {
        return compare(other);
    }

    float *operator[](const int index) {
        return &data[index][0];
    }

    const float *operator[](const int index) const {
        return &data[index][0];
    }

    /**
     * @brief Swaps elements on row r and column c to row c and column r (Reflection across main diagonal; data[c][r] = data[r][c])
     * @note Matrix must be a square matrix (n x n)
     * @return Matrix with columns and rows swapped
     */
    [[nodiscard]] Matrix<ROWS, COLUMNS> transpose() const requires (ROWS == COLUMNS) {
        Matrix<ROWS, ROWS> result;

        for (int c = 0; c < COLUMNS; c++) {
            for (int r = 0; r < ROWS; r++) {
                result[c][r] = data[r][c];
            }
        }

        return result;
    }

    /**
     * @brief Finds the inverse of the matrix if invertible (1 / matrix)
     * @throws Runtime errors if matrix is singular (not invertible)
     * @return The inverse of the matrix
     */
    [[nodiscard]] Matrix<ROWS, COLUMNS> inverse() const requires (ROWS == COLUMNS) {
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
            const Matrix identityMatrix = Matrix::identity();
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
     * @warning if zero do not attempt to find inverse of this matrix
     * @return The determinant of this matrix
     */
    [[nodiscard]] float determinant() const requires (ROWS == COLUMNS) {
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

    /**
     * @brief Uniformly scales the matrix along all dimensions (this * scaleMatrix)
     * @param value The scalar to multiply with each element
     * @return The matrix scaled uniformly
     */
    [[nodiscard]] Matrix<4, 4> scale(const float value) const requires (ROWS == COLUMNS && COLUMNS == 4) {
        Matrix<4, 4> result;

        for (int c = 0; c < 4; c++) {
            for (int r = 0; r < 4; r++) {
                result.data[c][r] = data[c][r] * value;
            }
        }

        return result;
    }

    /**
     * @brief Scales matrix non-uniformly along the x, y, and z axis (this * scaleMatrix)
     * @param x The scale factor for the x-axis.
     * @param y The scale factor for the y-axis.
     * @param z The scale factor for the z-axis.
     * @return The matrix scaled along {x, y, z}
     */
    [[nodiscard]] Matrix<4, 4> scaleAnisotropic(const float x, const float y, const float z) const requires (ROWS == COLUMNS && COLUMNS == 4) {
        Matrix<4, 4> scaleMatrix = identity();

        scaleMatrix.data[0][0] = x;
        scaleMatrix.data[1][1] = y;
        scaleMatrix.data[2][2] = z;

        return multiply(scaleMatrix);
    }

    /**
     * @brief Translates matrix by x, y, and z (this * translationMatrix)
     * @param x How much to translate along the x
     * @param y How much to translate along the y
     * @param z How much to translate along the z
     * @return The matrix translated by x, y, and z
     */
    [[nodiscard]] Matrix<4, 4> translate(const float x, const float y, const float z) const requires (ROWS == COLUMNS && COLUMNS == 4) {
        Matrix<4, 4> translationMatrix = identity();

        translationMatrix.data[3][0] = x;
        translationMatrix.data[3][1] = y;
        translationMatrix.data[3][2] = z;

        return multiply(translationMatrix);
    }

    /**
     * @brief Rotates the matrix by angle along the x (this * rotationMatrix)
     * @param angle How many DEGREES to rotate along the x
     * @return Matrix rotated by angle
     */
    [[nodiscard]] Matrix<4, 4> rotateX(const float angle) const requires (ROWS == COLUMNS && COLUMNS == 4) {
        const float sin = std::sinf(angle * (static_cast<float>(M_PI) / 180.0f));
        const float cos = std::cosf(angle * (static_cast<float>(M_PI) / 180.0f));

        Matrix<4, 4> rotationMatrix = identity();

        rotationMatrix.data[1][1] = cos;
        rotationMatrix.data[1][2] = sin;
        rotationMatrix.data[2][1] = -sin;
        rotationMatrix.data[2][2] = cos;

        return multiply(rotationMatrix);
    }

    /**
     * @brief Rotates the matrix by angle along the y (this * rotationMatrix)
     * @param angle How many DEGREES to rotate along the y
     * @return Matrix rotated by angle
     */
    [[nodiscard]] Matrix<4, 4> rotateY(const float angle) const requires (ROWS == COLUMNS && COLUMNS == 4) {
        const float sin = std::sinf(angle * (static_cast<float>(M_PI) / 180.0f));
        const float cos = std::cosf(angle * (static_cast<float>(M_PI) / 180.0f));

        Matrix<4, 4> rotationMatrix = identity();

        rotationMatrix.data[0][0] = cos;
        rotationMatrix.data[0][2] = -sin;
        rotationMatrix.data[2][0] = sin;
        rotationMatrix.data[2][2] = cos;

        return multiply(rotationMatrix);
    }

    /**
     * @brief Rotates the matrix by angle along the y (this * rotationMatrix)
     * @param angle How many DEGREES to rotate along the y
     * @return Matrix rotated by angle
     */
    [[nodiscard]] Matrix<4, 4> rotateZ(const float angle) const requires (ROWS == COLUMNS && COLUMNS == 4) {
        const float sin = std::sinf(angle * (static_cast<float>(M_PI) / 180.0f));
        const float cos = std::cosf(angle * (static_cast<float>(M_PI) / 180.0f));

        Matrix<4, 4> rotationMatrix = identity();

        rotationMatrix.data[0][0] = cos;
        rotationMatrix.data[0][1] = sin;
        rotationMatrix.data[1][0] = -sin;
        rotationMatrix.data[1][1] = cos;

        return multiply(rotationMatrix);
    }

    /**
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

    /**
     * @brief Makes a matrix made up of this matrix without rowToRemove and without columnToRemove
     * @param rowToRemove What row shouldnt be included
     * @param columnToRemove What column shouldnt be included
     * @return A matrix of <ROWS - 1, COLUMNS - 1> without the rowToRemove and columnToRemove
     */
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

    // static methods
    /**
     * @brief Creates an orthographic projection matrix.
     * @param left The coordinate of the left vertical clipping plane
     * @param right The coordinate of the right vertical clipping plane
     * @param bottom The coordinate of the bottom horizontal clipping plane
     * @param top The coordinate of the top horizontal clipping plane
     * @param near The coordinate of the near depth clipping plane
     * @param far The coordinate of the far depth clipping plane
     * @return A 4x4 orthographic projection matrix
     */
    static Matrix<4, 4> ortho(const float left, const float right, const float bottom, const float top, const float near, const float far) {
        // identity
        Matrix<4, 4> transformation = identity();
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

    /**
     * @brief Generates the identity matrix of size this Rows x this Rows
     * @return Matrix with 1s on main diagonal, 0s everywhere else
     */
    static Matrix<ROWS, ROWS> identity() {
        Matrix<ROWS, ROWS> result;

        for (int i = 0; i < ROWS; i++) {
            result[i][i] = 1;
        }

        return result;
    }

    /**
     * @tparam R_SIZE How many rows
     * @tparam C_SIZE How many columns
     * @param matrix Where the data will be written
     * @param rowA Index of row a
     * @param rowB Index of row b
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

    explicit operator const float *() const {
        return &data[0][0];
    }

    explicit operator float *() {
        return &data[0][0];
    }

    Vector4 operator*(const Vector4 &other) const requires (ROWS == 4 && COLUMNS == 4) {
        return multiply(other);
    }

    Vector3 operator*(const Vector3 &other) const requires (ROWS == 4 && COLUMNS == 4) {
        return multiply(other);
    }

    Vector2 operator*(const Vector2 &other) const requires (ROWS == 4 && COLUMNS == 4) {
        return multiply(other);
    }

    /**
     * @brief Multiplies the 4x4 matrix by a 4D vector.
     * @param other The 4D vector to multiply by.
     * @return A new 4D vector that is the result of the multiplication.
     */
    [[nodiscard]] Vector4 multiply(const Vector4 &other) const requires (ROWS == 4 && COLUMNS == 4) {
        Vector4 result;

        result.x = data[0][0] * other.x + data[1][0] * other.y + data[2][0] * other.z + data[3][0] * other.w;
        result.y = data[0][1] * other.x + data[1][1] * other.y + data[2][1] * other.z + data[3][1] * other.w;
        result.z = data[0][2] * other.x + data[1][2] * other.y + data[2][2] * other.z + data[3][2] * other.w;
        result.w = data[0][3] * other.x + data[1][3] * other.y + data[2][3] * other.z + data[3][3] * other.w;

        return result;
    }

    /**
     * @brief Multiplies the 4x4 matrix by a 3D vector.
     * @param other The 3D vector to multiply by.
     * @return A new 3D vector that is the result of the multiplication.
     */
    [[nodiscard]] Vector3 multiply(const Vector3 &other) const requires (ROWS == 4 && COLUMNS == 4) {
        // Promote the 3D vector to a 4D vector with w=1 for a point
        const Vector4 tempVector4{other.x, other.y, other.z, 1.0f};

        // Perform the core 4x4 matrix-vector multiplication
        const Vector4 result = multiply(tempVector4);

        // Return the first three components as a 3D vector
        return Vector3{result.x, result.y, result.z};
    }

    /**
     * @brief Multiplies the 4x4 matrix by a 2D vector.
     * @param other The 2D vector to multiply by.
     * @return A new 2D vector that is the result of the multiply.
     */
    [[nodiscard]] Vector2 multiply(const Vector2 &other) const requires (ROWS == 4 && COLUMNS == 4) {
        // Promote the 2D vector to a 4D vector with z=0 and w=1 for a point in 2D space
        const Vector4 tempVector4{other.x, other.y, 0.0f, 1.0f};

        const Vector4 result = multiply(tempVector4);

        return Vector2{result.x, result.y};
    }
};