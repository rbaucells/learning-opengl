#include "../math/mathematics.h"

#include <cassert>
#include <iostream>

int main() {
    Transform parent = Transform({5, 5}, 90, {2, 2});
    Transform child = Transform({2.5, -2.5}, -180, {0.5, 1.5}, &parent);

    auto childGlobalPosition = child.getGlobalPosition();
    std::printf("Child local position {%f, %f} Child global position {%f, %f} \n", child.localPosition.x, child.localPosition.y, childGlobalPosition.x, childGlobalPosition.y);

    auto childGlobalRotation = child.getGlobalRotation();
    std::printf("Child local rotation {%f} Child global rotation {%f} \n", child.localRotation, childGlobalRotation);

    auto childGlobalScale = child.getGlobalScale();
    std::printf("Child local scale {%f, %f} Child global scale {%f, %f} \n", child.localScale.x, child.localScale.y, childGlobalScale.x, childGlobalScale.y);

    return 0;
}
