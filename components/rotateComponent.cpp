#include "rotateComponent.h"

#include <iostream>

#include "renderer.h"
#include "../object.h"
#include "../workQueue.h"

RotateComponent::RotateComponent(Object* owner, float speed) : Component(owner) {
    this->speed = speed;
}

void RotateComponent::awake() {
    std::printf("awake() \n");
}

void RotateComponent::onEnable() {
    std::printf("onEnable() \n");
}

void RotateComponent::lateUpdate(double deltaTime) {
    std::printf("onLateUpdate() \n");
}

void RotateComponent::fixedUpdate(double fixedDeltaTime) {
    std::printf("onFixedUpdate() \n");
}

void RotateComponent::onDisable() {
    std::printf("onDisable() \n");
}

void RotateComponent::onDestroy() {
    std::printf("onDestroy() \n");
}

void RotateComponent::start() {
    // std::printf("Hello World! \n");
    std::printf("Start() \n");
    object->markForDestroy();
}

void RotateComponent::update(const double deltaTime) {
    std::printf("update() \n");
    object->transform.localRotation += speed * static_cast<float>(deltaTime);

    if (object->transform.localRotation < -180 || object->transform.localRotation > 90) {
        speed = -speed;
    }
}