#include <iomanip>
#include <iostream>
#include <sstream>

#include "../math/mathematics.h"
#include "../dependencies/include/linmath/linmath.h"

std::string getStringFromMatrix(const columnMatrix4x4 &matrix) {
    std::stringstream result;
    result << std::fixed << std::setprecision(2);

    for (int r = 0; r < 4; r++) {
        result << "[ ";

        for (int c = 0; c < 4; c++) {
            const float value = matrix.data[c][r];

            result << value;
            result << " ";
        }

        result << " ] " << std::endl;
    }

    return result.str();
}
std::string getStringFromBadMatrix(const mat4x4 &matrix) {
    std::stringstream result;
    result << std::fixed << std::setprecision(2);

    for (int r = 0; r < 4; r++) {
        result << "[ ";

        for (int c = 0; c < 4; c++) {
            const float value = matrix[c][r];

            result << value;
            result << " ";
        }

        result << " ] \n";
    }

    return result.str();
}

int main() {
    columnMatrix4x4 myMatrix = columnMatrix4x4::identity();

    mat4x4 notMyMatrix;
    mat4x4_identity(notMyMatrix);

    static_assert(myMatrix.compareTo(columnMatrix4x4::wrap(notMyMatrix)));

    std::cout << "My Identity Matrix \n" << getStringFromMatrix(myMatrix) << std::endl;
    std::cout << "Not My Identity Matrix \n" << getStringFromBadMatrix(notMyMatrix) << std::endl;

    columnMatrix4x4 translatedMatrix = columnMatrix4x4::identity();
    translatedMatrix = translatedMatrix.translate(5, 5, 5);

    mat4x4 translatedNotMyMatrix;
    mat4x4_identity(translatedNotMyMatrix);
    mat4x4_translate(translatedNotMyMatrix, 5, 5, 5);

    static_assert(translatedMatrix.compareTo(columnMatrix4x4::wrap(translatedNotMyMatrix)));

    std::cout << "My Translated Matrix \n" << getStringFromMatrix(translatedMatrix) << std::endl;
    std::cout << "Not My Translated Matrix \n" << getStringFromBadMatrix(translatedNotMyMatrix) << std::endl;

    columnMatrix4x4 transposedMatrix = translatedMatrix.transpose();

    mat4x4 transposedNotMyMatrix;
    mat4x4_transpose(transposedNotMyMatrix, translatedNotMyMatrix);

    static_assert(transposedMatrix.compareTo(columnMatrix4x4::wrap(transposedNotMyMatrix)));

    std::cout << "My Transposed Matrix \n" << getStringFromMatrix(transposedMatrix) << std::endl;
    std::cout << "Not My Transposed Matrix \n" << getStringFromBadMatrix(transposedNotMyMatrix) << std::endl;

    columnMatrix4x4 rotatedMatrix = transposedMatrix.rotate_x(15);

    mat4x4 rotatedNotMyMatrix;
    mat4x4_rotate_X(rotatedNotMyMatrix, transposedNotMyMatrix, 15);

    static_assert(rotatedMatrix.compareTo(columnMatrix4x4::wrap(rotatedNotMyMatrix)));

    std::cout << "My Rotated Matrix \n" << getStringFromMatrix(rotatedMatrix) << std::endl;
    std::cout << "Not My Rotated Matrix \n" << getStringFromBadMatrix(rotatedNotMyMatrix) << std::endl;

    columnMatrix4x4 orhoMatrix = columnMatrix4x4::identity();
    orhoMatrix = orhoMatrix.ortho(-5, 5, -2.5, 2.5, -1, 10);

    mat4x4 orthoNotMyMatrix;
    mat4x4_identity(orthoNotMyMatrix);
    mat4x4_ortho(orthoNotMyMatrix, -5, 5, -2.5, 2.5, -1, 10);

    static_assert(orhoMatrix.compareTo(columnMatrix4x4::wrap(orthoNotMyMatrix)));

    std::cout << "My ortho Matrix \n" << getStringFromMatrix(orhoMatrix) << std::endl;
    std::cout << "Not My ortho Matrix \n" << getStringFromBadMatrix(orthoNotMyMatrix) << std::endl;

    return 0;
}