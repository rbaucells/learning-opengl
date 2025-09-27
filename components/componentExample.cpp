#include "componentExample.h"

#include "../object.h"
#include "../systems/betterEvents.h"
#include "../systems/input.h"
#include "../systems/tweens/tween.h"
#include "renderer/renderer.h"

void testFunction(const int i) {
    printf("Value: %d\n", i);
}

void ComponentExample::start() {
    // subscribe to the left click event
    left_click_event.subscribe(this, &ComponentExample::onMouseInput);
    // left_click_event.unSubscribe(this, &ComponentExample::onMouseInput);

    EventDispatcher<int> dispatcher;
    constexpr int i = 5;

    listener_.bind(&dispatcher, testFunction);

    dispatcher.invoke(i);

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

    runningPositionTweenWeakPtr_ = addTween(positionTweenUniquePtr);

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
        else {
            object->removeComponent<ComponentExample>();
        }
    }
}
