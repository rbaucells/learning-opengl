#include "componentExample.h"

#include "../object.h"
#include "../systems/tweens/tween.h"
#include "renderer/renderer.h"
#include "../systems/input.h"

void ComponentExample::start() {
    // subscribe to the left click event
    left_click_event.subscribe(this, &ComponentExample::onMouseInput);

    auto positionTweenUniquePtr = object->transform.localPosTween({3, 0}, 5, Curve::expoOut);

    positionTweenUniquePtr->onStart.subscribe([] {
        printf("Tween Started\n");
    });

    positionTweenUniquePtr->onComplete.subscribe([] {
        printf("Tween Completed\n");
    });

    positionTweenUniquePtr->onCancel.subscribe([] {
        printf("Tween Cancelled\n");
    });

    // after this point the positionTweenUniquePtr ^ is invalid

    runningPositionTweenWeakPtr_ = addTween(positionTweenUniquePtr);

    // runningPositionTweenRawPtr_ will remain valid until either

    // (AUTO_KILL is on and the tween has naturally completed) or
    // (the tween has been killed by either forceComplete() or forceCancel())6

    // after which it will not be safe

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
    if (state) {
        printf("Mouse Left Clicked Down\n");
        if (const auto runningPositionTween = runningPositionTweenWeakPtr_.lock()) {
            printf("Cancelling Running Position Tween\n");
            runningPositionTween->forceCancel();
        }
    }
}
