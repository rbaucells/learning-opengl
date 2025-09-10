#include "rotateComponent.h"

#include "renderer.h"
#include "../object.h"
#include "../systems/tweens/tween.h"
#include "systems/input.h"

RotateComponent::RotateComponent(Object* owner, const float speed) : Component(owner) {
    this->speed = speed;
}

void RotateComponent::start() {
    std::printf("Start \n");

    // Normal Sequenctial Tweens

    // auto* posTween = addTween(object->transform.localPosTween({0, 0}, {500, -300}, 3, Curve::sineInOut));
    // auto* rotTween = addTween(object->transform.localRotationTween(90, 5, Curve::bounceInOut));
    // auto* scaleTween = addTween(object->transform.localScaleTween({1.3, 0.7}, {2, 1.7}, 2, Curve::linear));
    //
    // posTween->onComplete.subscribe([]() {
    //     std::printf("local pos tween is done \n");
    // });
    //
    // rotTween->onComplete.subscribe([]() {
    //     std::printf("local rot tween is done \n");
    // });
    //
    // scaleTween->onComplete.subscribe([]() {
    //     std::printf("local scale tween is done \n");
    // });

    // Sequence Tweens

    auto* sequence = addTween(std::make_unique<SequenceTween>());

    sequence->add(object->transform.localPosTween({500, -300}, 3, Curve::sineInOut));
    sequence->join(object->transform.localRotationTween(90, 5, Curve::bounceInOut));
    sequence->add(object->transform.localScaleTween({2, 1.7}, 2, Curve::linear));

    sequence->onComplete.subscribe([]() {
        std::printf("The sequence is complete \n");
    });
    sequence->onCancel.subscribe([]() {
        std::printf("The sequence is canceled \n");
    });

    sequence->start();
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
