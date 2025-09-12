#include "rotateComponent.h"

#include "renderer/renderer.h"
#include "../object.h"
#include "../systems/tweens/tween.h"
#include "systems/input.h"

RotateComponent::RotateComponent(Object* owner, const float speed) : Component(owner) {
    this->speed = speed;
}

void RotateComponent::start() {
    std::printf("Start \n");

    // Normal Sequenctial Tweens

    // auto* localPosTween = addTween(object->transform.localPosTween({0, 0}, {500, -300}, 3, Curve::sineInOut));
    // auto* localRotTween = addTween(object->transform.localRotationTween(90, 5, Curve::bounceInOut));
    // auto* localScaleTween = addTween(object->transform.localScaleTween({1.3, 0.7}, {2, 1.7}, 2, Curve::linear));

    // auto* globalPosTween = addTween(object->transform.globalPosTween({100, 200}, {300, 400}, 5.6, Curve::sineInOut));
    // auto* globalRotTween = addTween(object->transform.globalRotationTween(90,  7.2, Curve::bounceIn));
    // auto* globalScaleTween = addTween(object->transform.globalScaleTween({1, 2},  3.4, Curve::linear));

    // Sequence Tweens
    //
    // auto* sequence = addTween(std::make_unique<SequenceTween>());
    //
    // sequence->add(object->transform.localPosTween({500, -300}, 3, Curve::sineIn));
    // sequence->join(object->transform.localRotationTween(90, 5, Curve::expoOut));
    // sequence->add(object->transform.localScaleTween({2, 1.7}, 2, Curve::quartInOut));
    //
    // sequence->start();
}

void RotateComponent::update(const double deltaTime) {
    // std::printf("Update \n");
}

void RotateComponent::awake() {
    // std::printf("Awake \n");
}

void RotateComponent::onEnable() {
    // std::printf("onEnable \n");
}

void RotateComponent::lateUpdate(const double deltaTime) {
    // std::printf("lateUpdate \n");
}

void RotateComponent::fixedUpdate(const double fixedDeltaTime) {
    // std::printf("fixedUpdate \n");
}

void RotateComponent::onDisable() {
    // std::printf("onDisable \n");
}

void RotateComponent::onDestroy() {
    // std::printf("onDestroy \n");
}
