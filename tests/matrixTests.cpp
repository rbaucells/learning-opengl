#include <iomanip>
#include <iostream>
#include <sstream>
#include <cassert>

#include "../math/mathematics.h"
#include "../dependencies/include/linmath/linmath.h"

std::string getStringFromMatrix(const ColumnMatrix4x4 &matrix) {
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
    // Identity Test //
    ColumnMatrix4x4 identityMatrix = ColumnMatrix4x4::identity();
    mat4x4 linmathIdentityMatrix;
    mat4x4_identity(linmathIdentityMatrix);

    std::cout << "Identity: \n" << getStringFromMatrix(identityMatrix) << std::endl;
    std::cout << "linmath identity: \n" << getStringFromBadMatrix(linmathIdentityMatrix) << std::endl;

    assert(identityMatrix.compareTo(ColumnMatrix4x4::wrap(linmathIdentityMatrix)));
    std::cout << "Passed Identity Matrix Test" << std::endl;

    // Arithmetic Test //
    ColumnMatrix4x4 a = ColumnMatrix4x4::identity().scale(3);
    ColumnMatrix4x4 b = ColumnMatrix4x4::identity().scale(2);
    ColumnMatrix4x4 sum = a + b;

    mat4x4 linmathA;
    mat4x4_identity(linmathA);
    mat4x4_scale(linmathA, linmathA, 3);

    mat4x4 linmathB;
    mat4x4_identity(linmathB);
    mat4x4_scale(linmathB, linmathB, 2);

    mat4x4 linmathSum;
    mat4x4_add(linmathSum, linmathA, linmathB);

    std::cout << "Sum: \n" << getStringFromMatrix(sum) << std::endl;
    std::cout << "linmath sum: \n" << getStringFromBadMatrix(linmathSum) << std::endl;

    assert(sum.compareTo(ColumnMatrix4x4::wrap(linmathSum)));
    std::cout << "Passed Addition Matrix Test" << std::endl;

    ColumnMatrix4x4 diff = b - a;

    mat4x4 linmathDiff;
    mat4x4_sub(linmathDiff, linmathB, linmathA);

    std::cout << "Difference: \n" << getStringFromMatrix(diff) << std::endl;
    std::cout << "linmath difference: \n" << getStringFromBadMatrix(linmathDiff) << std::endl;

    assert (diff.compareTo(ColumnMatrix4x4::wrap(linmathDiff)));
    std::cout << "Passed Subtraction Matrix Test" << std::endl;

    // Transformation Test //
    ColumnMatrix4x4 base = ColumnMatrix4x4::identity();
    mat4x4 linmathBase;
    mat4x4_identity(linmathBase);

    ColumnMatrix4x4 translated = base.translate(5, 10, 15);
    mat4x4 linmathTranslated;
    mat4x4_translate(linmathTranslated, 5, 10, 15);

    std::cout << "Translated: \n" << getStringFromMatrix(translated) << std::endl;
    std::cout << "linmath translated: \n" << getStringFromBadMatrix(linmathTranslated) << std::endl;

    assert(translated.compareTo(ColumnMatrix4x4::wrap(linmathTranslated)));
    std::cout << "Passed translate Matrix Test" << std::endl;

    ColumnMatrix4x4 rotatedX = translated.rotate_x(10);
    mat4x4 linmathRotatedX;
    mat4x4_rotate_X(linmathRotatedX, linmathTranslated, 10);

    std::cout << "Rotated X: \n" << getStringFromMatrix(rotatedX) << std::endl;
    std::cout << "linmath rotated X: \n" << getStringFromBadMatrix(linmathRotatedX) << std::endl;

    assert (rotatedX.compareTo(ColumnMatrix4x4::wrap(linmathRotatedX)));
    std::cout << "Passed rotate x Matrix Test" << std::endl;

    ColumnMatrix4x4 rotatedY = rotatedX.rotate_y(20);
    mat4x4 linmathRotatedY;
    mat4x4_rotate_Y(linmathRotatedY, linmathRotatedX, 20);

    std::cout << "Rotated Y: \n" << getStringFromMatrix(rotatedY) << std::endl;
    std::cout << "linmath rotated Y: \n" << getStringFromBadMatrix(linmathRotatedY) << std::endl;

    assert (rotatedY.compareTo(ColumnMatrix4x4::wrap(linmathRotatedY)));
    std::cout << "Passed rotate y Matrix Test" << std::endl;

    ColumnMatrix4x4 rotatedZ = rotatedY.rotate_z(30);
    mat4x4 linmathRotatedZ;
    mat4x4_rotate_Z(linmathRotatedZ, linmathRotatedY, 30);

    std::cout << "Rotated Z: \n" << getStringFromMatrix(rotatedZ) << std::endl;
    std::cout << "linmath rotated Z: \n" << getStringFromBadMatrix(linmathRotatedZ) << std::endl;

    assert (rotatedZ.compareTo(ColumnMatrix4x4::wrap(linmathRotatedZ)));
    std::cout << "Passed rotate Z Matrix Test" << std::endl;

    ColumnMatrix4x4 scaled = rotatedZ.scale(45);
    mat4x4 linmathScaled;
    mat4x4_scale(linmathScaled, linmathRotatedZ, 45);

    std::cout << "Scaled: \n" << getStringFromMatrix(scaled) << std::endl;
    std::cout << "linmath scaled: \n" << getStringFromBadMatrix(linmathScaled) << std::endl;

    assert(scaled.compareTo(ColumnMatrix4x4::wrap(linmathScaled)));
    std::cout << "Passed scaling Matrix Test" << std::endl;

    ColumnMatrix4x4 anisotropicScaled = scaled.scale_anisotropic(1, 2, 3);
    mat4x4 linmathAnisotropicScaled;
    mat4x4_scale_aniso(linmathAnisotropicScaled, linmathScaled, 1, 2, 3);

    std::cout << "Anisotropic Scaled: \n" << getStringFromMatrix(anisotropicScaled) << std::endl;
    std::cout << "linmath cnisotropic scaled: \n" << getStringFromBadMatrix(linmathAnisotropicScaled) << std::endl;

    assert(anisotropicScaled.compareTo(ColumnMatrix4x4::wrap(linmathAnisotropicScaled)));
    std::cout << "Passed anisotropic scaling Matrix Test" << std::endl;

    ColumnMatrix4x4 transposed = anisotropicScaled.transpose();
    mat4x4 linmathTransposed;
    mat4x4_transpose(linmathTransposed, linmathAnisotropicScaled);

    std::cout << "Transposed: \n" << getStringFromMatrix(transposed) << std::endl;
    std::cout << "linmath transposed: \n" << getStringFromBadMatrix(linmathTransposed) << std::endl;

    assert(transposed.compareTo(ColumnMatrix4x4::wrap(linmathTransposed)));
    std::cout << "Passed transpose Matrix Test" << std::endl;

    ColumnMatrix4x4 ortho = transposed.ortho(-5, 5, -10, 10, 1, -10);
    mat4x4 linmathOrtho;
    mat4x4_ortho(linmathOrtho, -5, 5, -10, 10, 1, -10);

    std::cout << "orho: \n" << getStringFromMatrix(ortho) << std::endl;
    std::cout << "linmath ortho: \n" << getStringFromBadMatrix(linmathOrtho) << std::endl;

    assert(ortho.compareTo(ColumnMatrix4x4::wrap(linmathOrtho)));
    std::cout << "Passed ortho Matrix test" << std::endl;

    ColumnMatrix4x4 inverse = ortho.inverse();
    mat4x4 linmathInverse;
    mat4x4_invert(linmathInverse, linmathOrtho);

    std::cout << "inverse: \n" << getStringFromMatrix(inverse) << std::endl;
    std::cout << "linmath inverse: \n" << getStringFromBadMatrix(linmathInverse) << std::endl;

    assert(inverse.compareTo(ColumnMatrix4x4::wrap(linmathInverse)));

    std::cout << "Passed all Tests" << std::endl;
}