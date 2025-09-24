#include "componentExample.h"

#include "../object.h"
#include "../systems/tweens/tween.h"
#include "renderer/renderer.h"

void ComponentExample::start() {
    auto tween = std::make_unique<WaitTween>(5);

    tween->onStart.subscribe([] {
        printf("Tween Started\n");
    });

    tween->onComplete.subscribe([] {
        printf("Tween Completed\n");
    });

    tween->onCancel.subscribe([] {
        printf("Tween Cancelled\n");
    });

    auto* waitTween = addTween(tween);
}
