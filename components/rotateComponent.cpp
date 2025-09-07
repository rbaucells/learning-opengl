#include "rotateComponent.h"

#include "renderer.h"
#include "../object.h"

RotateComponent::RotateComponent(Object *owner, const float speed) : Component(owner) {
    this->speed = speed;
}

void RotateComponent::start() {
    std::printf("Start \n");

    const auto rendererPtr = object->getComponent<Renderer>();

    if (const auto renderer = rendererPtr.lock()) {
        renderer->tweenAlpha(0, 5, linear);
    }
}

void RotateComponent::update(const double deltaTime) {
    std::printf("Update \n");
}

void RotateComponent::awake() {
    std::printf("Awake \n");
}

void RotateComponent::onEnable() {
    std::printf("onEnable \n");
}

void RotateComponent::lateUpdate(double deltaTime) {
    std::printf("lateUpdate \n");
}

void RotateComponent::fixedUpdate(double fixedDeltaTime) {
    std::printf("fixedUpdate \n");
}

void RotateComponent::onDisable() {
    std::printf("onDisable \n");
}

void RotateComponent::onDestroy() {
    std::printf("onDestroy \n");
}
