#include "myComponent.h"
#include "../object.h"

void myComponent::start() {
    std::printf("Hello World! \n");
    actionsToListenToo.push_back(space);
}

void myComponent::update(const double deltaTime) {
    object -> transform.position = mousePosToScreenPos(mousePos);
}

void myComponent::fixedUpdate(const double fixedDeltaTime) {

}

void myComponent::onInput(const InputContext &context) {
    // mousePos = context.extraVector2;
    OnMyEvent.invoke(1.0f);
}

