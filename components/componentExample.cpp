#include "componentExample.h"

#include "../object.h"
#include "../systems/betterEvents.h"
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
        if (const auto positionTween = localPositionTweenWeakPtr_.lock()) {
            positionTween->forceCancel();
        }
        else {
            printf("kms\n");
            object->removeComponent<ComponentExample>();
        }
    }
}
