#include "componentExample.h"

#include "../object.h"
#include "../systems/audio/audioSource.h"
#include "renderer/renderer.h"

#include "../systems/opengl wrappers/window.h"

void ComponentExample::awake() {
    // std::printf("Awake \n");
}

void ComponentExample::start() {
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

    // start playing the audio
    // const auto sourceWeakPtr = object->getComponent<AudioSource>();
    //
    // if (const auto source = sourceWeakPtr.lock()) {
    //     // source->setLooping(true);
    //     source->play();
    // }
    //
    // auto tween = object->transform.globalPosTween({-1000, 0}, {1000, 0}, 6, Curve::linear);
    //
    // addTween(tween);
}

void ComponentExample::onEnable() {
    // std::printf("onEnable \n");
}

void ComponentExample::update(float deltaTime) {
    // static int i = 0;
    // const auto rendererPtr = object->getComponent<SpriteSheetRenderer>();
    //
    // if (const auto renderer = rendererPtr.lock()) {
    //     renderer->moveTo(i);
    //
    //     if (i < 230)
    //         ++i;
    //     else
    //         i = 1;
    // }

    // Window::mainWindow->setCurrentWindowWidth(Window::mainWindow->getCurrentWindowWidth() - (5 * deltaTime));
    Window::mainWindow->setCoordinateSystemHeight(Window::mainWindow->getCoordinateSystemHeight() - 0.01f);
}

void ComponentExample::lateUpdate(float deltaTime) {
    // std::printf("lateUpdate \n");
}

void ComponentExample::fixedUpdate(float fixedDeltaTime) {
    // std::printf("fixedUpdate \n");
}

void ComponentExample::onDisable() {
    // std::printf("onDisable \n");
}

void ComponentExample::onDestroy() {
    // std::printf("onDestroy \n");
}
