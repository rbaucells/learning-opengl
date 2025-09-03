#include <iostream>

#include "../math/mathematics.h"

int main() {
    // test with an identity matrix, determinant should be 1
    Matrix<3, 3> identityMatrix;
    identityMatrix.data[0][0] = 1;
    identityMatrix.data[1][1] = 1;
    identityMatrix.data[2][2] = 1;
    std::cout << "Determinant of identity matrix: " << identityMatrix.determinant() << " (expected 1)\n";

    // test with a matrix whose determinant is known to be 0
    Matrix<3, 3> singularMatrix;
    singularMatrix.data[0][0] = 1; singularMatrix.data[1][0] = 6; singularMatrix.data[2][0] = 4;
    singularMatrix.data[0][1] = 2; singularMatrix.data[1][1] = 7; singularMatrix.data[2][1] = 3;
    singularMatrix.data[0][2] = 8; singularMatrix.data[1][2] = 9; singularMatrix.data[2][2] = 5;
    std::cout << "Determinant of singular matrix: " << singularMatrix.determinant() << " (expected -60)\n";

    // Matrix<2, 2> inversableMatrix;
    //
    // inversableMatrix[0][0] = 3;
    // inversableMatrix[1][0] = 4;
    // inversableMatrix[0][1] = 1;
    // inversableMatrix[1][1] = 2;

    Matrix<3, 3> inversableMatrix;

    inversableMatrix[0][0] = 1;
    inversableMatrix[0][1] = 2;
    inversableMatrix[0][2] = 1;

    inversableMatrix[1][0] = 5;
    inversableMatrix[1][1] = 9;
    inversableMatrix[1][2] = 1;

    inversableMatrix[2][0] = 2;
    inversableMatrix[2][1] = 1;
    inversableMatrix[2][2] = 0;

    auto inverse = inversableMatrix.inverse();

    std::cout << "Inverse of inversableMatrix matrix: \n" << inverse.toString();
}