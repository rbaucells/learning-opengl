#include "Moving.h"

#include "../main.h"
#include "../object.h"

void Moving::start() {
    std::printf("Hello World! \n");
    actionsToListenToo.push_back(space);
}

void Moving::update(const double deltaTime) {
    object->transform.position.x += speed * deltaTime;
    // std::printf("Delta Time: %f \n", deltaTime);
}

void Moving::fixedUpdate(const double fixedDeltaTime) {
    // std::printf("Fixed Delta Time: %f \n", fixedDeltaTime);
}

void Moving::onInput(const InputContext &context) {
    std::printf("Received Input \"%d\" and state \"%d\" ", context.action, context.state);
}

