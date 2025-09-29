#include "componentExample.h"

#include <ranges>

#include "../object.h"
#include "../systems/betterEvents.h"
#include "../systems/input.h"
#include "../systems/tweens/tween.h"
#include "renderer/renderer.h"

void ComponentExample::start() {
    leftClickEventSubscription_ = leftClickEvent->subscribe(this, &ComponentExample::onMouseInput);

    runningPositionTweenWeakPtr_ = addTween(object->transform.localPosTween({3, 0}, 5, Curve::expoOut));

    if (const auto runningPositionTween = runningPositionTweenWeakPtr_.lock()) {
        runningPositionTween->onComplete->subscribe([]() {
            printf("Running Position Tween is Complete\n");
        }).setForever();

        runningPositionTween->onStart->subscribe([]() {
            printf("Running Position Tween is Started\n");
        }).setForever();

        runningPositionTween->onCancel->subscribe([]() {
            printf("Running Position Tween is Cancelled\n");
        }).setForever();
    }

    // Renderers stuff
    spriteSheetRendererWeakPtr_ = object->getComponent<SpriteSheetRenderer>();
}

void ComponentExample::update(float deltaTime) {
    static int i = 1;

    if (i > 230)
        i = 1;

    if (const auto spriteSheetRenderer = spriteSheetRendererWeakPtr_.lock())
        spriteSheetRenderer->moveTo(i++);
}

void ComponentExample::onMouseInput(const bool state) const {
    // if (state) {
    //     // printf("Mouse Left Clicked Down\n");
    //     if (const auto runningPositionTween = runningPositionTweenWeakPtr_.lock()) {
    //         // printf("Cancelling Running Position Tween\n");
    //         runningPositionTween->forceCancel();
    //     }
    //     else {
    //         object->removeComponent<ComponentExample>();
    //     }
    // }
}
