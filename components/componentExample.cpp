#include "componentExample.h"

#include "../object.h"
#include "../systems/events.h"
#include "../systems/input.h"
#include "../systems/tweens/tween.h"
#include "renderer/renderer.h"

void ComponentExample::start() {
    leftClickEventSubscription_ = leftClickEvent->subscribe(this, &ComponentExample::onMouseInput);

    localPositionTweenWeakPtr_ = object->transform.localPosTween({3, 0}, 5, Curve::expoOut);

    if (const auto positionTween = localPositionTweenWeakPtr_.lock()) {
        positionTween->onComplete->subscribe([] {
            printf("position tween complete\n");
        }).setForever();

        positionTween->onStart->subscribe([] {
            printf("position tween started\n");
        }).setForever();

        positionTween->onCancel->subscribe([] {
            printf("position tween cancelled\n");
        }).setForever();

        positionTween->start();
    }

    // Renderers stuff
    spriteSheetRendererWeakPtr_ = object->getComponent<SpriteSheetRenderer>();
}

void ComponentExample::onMouseInput(const bool state) const {
    if (state) {
        if (const auto positionTween = localPositionTweenWeakPtr_.lock()) {
            positionTween->forceCancel();
        }
        else {
            object->removeComponent<ComponentExample>();
        }
    }
}
