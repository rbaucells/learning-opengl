#include <iostream>

#include "../math/mathematics.h"

int main() {
    Matrix<2, 2> matrixA;
    matrixA[0][0] = 4;
    matrixA[0][1] = 7;
    matrixA[1][0] = 2;
    matrixA[1][1] = 6;

    auto inverseA = matrixA.inverse();
    std::cout << "Inverse of Matrix A: \n" << inverseA.toString();

    Matrix<3, 3> matrixB;
    matrixB[0][0] = 1; matrixB[0][1] = 2; matrixB[0][2] = 1;
    matrixB[1][0] = 5; matrixB[1][1] = 9; matrixB[1][2] = 1;
    matrixB[2][0] = 2; matrixB[2][1] = 1; matrixB[2][2] = 0;

    auto inverseB = matrixB.inverse();
    std::cout << "Inverse of Matrix B: \n" << inverseB.toString();

    Matrix<3, 3> identityMatrix;
    identityMatrix[0][0] = 1; identityMatrix[0][1] = 0; identityMatrix[0][2] = 0;
    identityMatrix[1][0] = 0; identityMatrix[1][1] = 1; identityMatrix[1][2] = 0;
    identityMatrix[2][0] = 0; identityMatrix[2][1] = 0; identityMatrix[2][2] = 1;

    auto inverseIdentity = identityMatrix.inverse();
    std::cout << "Inverse of an Identity Matrix: \n" << inverseIdentity.toString();

    Matrix<3, 3> singularMatrix;
    singularMatrix[0][0] = 1; singularMatrix[0][1] = 2; singularMatrix[0][2] = 3;
    singularMatrix[1][0] = 4; singularMatrix[1][1] = 5; singularMatrix[1][2] = 6;
    singularMatrix[2][0] = 7; singularMatrix[2][1] = 8; singularMatrix[2][2] = 9;

    try {
        const auto inverseSingular = singularMatrix.inverse();
        std::cout << "Inverse of a Singular Matrix (should not be reached): \n" << inverseSingular.toString();
    }
    catch (const std::exception& e) {
        std::cerr << "Caught expected exception for singular matrix: \n" << e.what() << std::endl;
    }

    Matrix<2, 2> coefficients;

    coefficients[0][0] = 2;
    coefficients[0][1] = 3;
    coefficients[1][0] = -5;
    coefficients[1][1] = 1;

    Matrix<2, 1> result;

    result[0][0] = 15;
    result[0][1] = 31;

    Matrix solution = Matrix<2, 1>::solveSystemOfEquations(coefficients, result);

    std::cout << "Solution to system of equations: x = " << solution[0][0] << " y = " << solution[0][1] << std::endl;
}