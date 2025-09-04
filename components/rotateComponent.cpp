#include "rotateComponent.h"
#include "../workQueue.h"
#include <iostream>

#include "renderer.h"
#include "../object.h"

RotateComponent::RotateComponent(Object* owner, float speed) : Component(owner) {
    this->speed = speed;
}

void RotateComponent::start() {
    std::printf("Hello World! \n");
}

void RotateComponent::update(const double deltaTime) {
    object->transform.localRotation += speed * static_cast<float>(deltaTime);

    if (object->transform.localRotation < -180 || object->transform.localRotation > 90) {
        speed = -speed;
    }

    // std::cout << object->name << " rotation in degrees: " << object->transform.localRotation << "\n";
}