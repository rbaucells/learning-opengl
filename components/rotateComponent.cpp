#include "rotateComponent.h"

#include "renderer.h"
#include "../object.h"
#include "systems/tween.h"

RotateComponent::RotateComponent(Object *owner, const float speed) : Component(owner) {
    this->speed = speed;
}

void RotateComponent::start() {
    std::printf("Start \n");

    object->transform.registerTween(object->transform.localPosTween({500, 200}, 5, Curve::sineInOut));
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

void RotateComponent::lateUpdate(const double deltaTime) {
    std::printf("lateUpdate \n");
}

void RotateComponent::fixedUpdate(const double fixedDeltaTime) {
    std::printf("fixedUpdate \n");
}

void RotateComponent::onDisable() {
    std::printf("onDisable \n");
}

void RotateComponent::onDestroy() {
    std::printf("onDestroy \n");
}
