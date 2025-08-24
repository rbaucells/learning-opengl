#include "Moving.h"

#include "../object.h"

void Moving::start() {
    std::printf("Hello World! \n");
}

void Moving::update(const double deltaTime) {
    object->transform.position.x += speed;
    std::printf("Delta Time: %f \n", deltaTime);
}

void Moving::fixedUpdate(const double fixedDeltaTime) {
    std::printf("Fixed Delta Time: %f \n", fixedDeltaTime);
}


