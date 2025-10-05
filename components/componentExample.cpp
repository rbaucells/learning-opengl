#include "componentExample.h"

#include "../object.h"

ComponentExample::ComponentExample(Object* owner) : Component(owner) {
    printf("Constructor\n");
}

void ComponentExample::awake() {
    printf("Awake\n");
}

void ComponentExample::start() {
    printf("Start\n");

    object->destroy();
}

void ComponentExample::onEnable() {
    printf("OnEnable\n");
}

void ComponentExample::update(float deltaTime) {
    printf("Update\n");
}

void ComponentExample::lateUpdate(float deltaTime) {
    printf("LateUpdate\n");
}

void ComponentExample::fixedUpdate(float fixedDeltaTime) {
    printf("FixedUpdate\n");
}

void ComponentExample::onDisable() {
    printf("OnDisable\n");
}

void ComponentExample::onDestroy() {
    printf("OnDestroy\n");
}

ComponentExample::~ComponentExample() {
    printf("Destructor\n");
}
