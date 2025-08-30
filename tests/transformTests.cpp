#include "../math/mathematics.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>

std::string getStringFromMatrix(const ColumnMatrix4x4 &matrix) {
    std::stringstream result;
    result << std::fixed << std::setprecision(4);

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

int main() {
    Transform parent = Transform({-7.5, 2.5}, -45, {2, 0.5});
    Transform child = Transform({2.5, -2.5}, 225, {1.5, 6}, &parent);
    // Transform child = Transform({2.5, -2.5}, -45, {1, 1});

    auto childGlobalPosition = child.getGlobalPosition();
    std::printf("Child local position {%f, %f} Child global position {%f, %f} \n", child.localPosition.x, child.localPosition.y, childGlobalPosition.x, childGlobalPosition.y);

    auto childGlobalRotation = child.getGlobalRotation();
    std::printf("Child local rotation {%f} Child global rotation {%f} \n", child.localRotation, childGlobalRotation);

    auto childGlobalScale = child.getGlobalScale();
    std::printf("Child local scale {%f, %f} Child global scale {%f, %f} \n", child.localScale.x, child.localScale.y, childGlobalScale.x, childGlobalScale.y);

    std::cout << "local to world matrix: \n" << getStringFromMatrix(child.localToWorldMatrix());
    std::cout << "world to local matrix: \n" << getStringFromMatrix(child.worldToLocalMatrix());

    const vector2 newGlobalPosition = {15, 9.5};
    child.setGlobalPosition(newGlobalPosition);
    const vector2 curGlobalPosition = child.getGlobalPosition();

    std::printf("Child new global position {%f, %f} and Child cur global position {%f, %f} \n", newGlobalPosition.x, newGlobalPosition.y, curGlobalPosition.x, curGlobalPosition.y);

    assert(newGlobalPosition == curGlobalPosition);

    const vector2 newGlobalScale = {2.54, 3.45};
    child.setGlobalScale(newGlobalScale);
    const vector2 curGlobalScale = child.getGlobalScale();

    std::printf("Child new global scale {%f, %f} and Child cur global scale {%f, %f} \n", newGlobalScale.x, newGlobalScale.y, curGlobalScale.x, curGlobalScale.y);

    assert(newGlobalScale == curGlobalScale);

    const float newGlobalRotation = 55;
    child.setGlobalRotation(newGlobalRotation);
    const float curGlobalRotation = child.getGlobalRotation();

    std::printf("Child new global rotation {%f} and Child cur global rotation {%f} \n", newGlobalRotation, curGlobalRotation);

    assert(newGlobalRotation == curGlobalRotation);

    return 0;
}
