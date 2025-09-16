#include "rotateComponent.h"

#include "../object.h"
#include "renderer/renderer.h"

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
    // sequence->add(std::make_unique<CallbackTween>([this]() {
    //     std::printf("This is a callback \n");
    //
    //     const auto renderer = this->object->getComponent<RendererBase>();
    //
    //     if (const auto rendererPtr = renderer.lock())
    //         rendererPtr->changeSpriteTexture("/Users/ricardito/CLionProjects/OpenGL/res/textures/anotherDvd.png", false, GL_REPEAT);
    // }));
    // sequence->add(object->transform.localScaleTween({2, 1.7}, 2, Curve::quartInOut));
    //
    // sequence->start();

    queue.push_back(std::move(std::make_unique<TimedQueueEntry>([this]() {
        static int i = 0;
        const auto rendererPtr = object->getComponent<SpriteSheetRenderer>();

        if (const auto renderer = rendererPtr.lock()) {
            renderer->moveTo(i);

            if (i < 230)
                ++i;
            else
                i = 1;
        }
    }, 0.01, 100000)));
}

void RotateComponent::update(float deltaTime) {}

void RotateComponent::awake() {
    // std::printf("Awake \n");
}

void RotateComponent::onEnable() {
    // std::printf("onEnable \n");
}

void RotateComponent::lateUpdate(float deltaTime) {
    // std::printf("lateUpdate \n");
}

void RotateComponent::fixedUpdate(float fixedDeltaTime) {
    // std::printf("fixedUpdate \n");
}

void RotateComponent::onDisable() {
    // std::printf("onDisable \n");
}

void RotateComponent::onDestroy() {
    // std::printf("onDestroy \n");
}
