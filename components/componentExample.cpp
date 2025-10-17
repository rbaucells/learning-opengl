#include "componentExample.h"

#include "../object.h"
#include "../serializationAndDeserialization/componentRegistry.h"
#include "../serializationAndDeserialization/deserializer.h"
#include "../systems/input.h"

REGISTER_COMPONENT("ComponentExample", ComponentExample)

ComponentExample::ComponentExample(Object* owner) : Component(owner) {
    printf("Constructor\n");
}

void ComponentExample::awake() {
    printf("Awake\n");

    leftClickSubscription_ = leftClickEvent->subscribe(this, &ComponentExample::onLeftClick);
}

void ComponentExample::start() {
    printf("Start\n");
}

void ComponentExample::onEnable() {
    printf("OnEnable\n");
}

void ComponentExample::update(float deltaTime) {
    // printf("Update\n");
}

void ComponentExample::lateUpdate(float deltaTime) {
    // printf("LateUpdate\n");
}

void ComponentExample::fixedUpdate(float fixedDeltaTime) {
    // printf("FixedUpdate\n");
}

void ComponentExample::onDisable() {
    printf("OnDisable\n");
}

void ComponentExample::onDestroy() {
    printf("OnDestroy\n");
}

void ComponentExample::onLeftClick(const bool state) const {
    object->transform.localPosition.x += 1;
}

ComponentExample::~ComponentExample() {
    printf("Destructor\n");
}

JsonObject ComponentExample::serialize() const {
    JsonObject jsonComponent;

    jsonComponent.putNumberField("f", f);
    jsonComponent.putStringField("uuid", id);
}

std::shared_ptr<Component> ComponentExample::deserialize(Object* owner, const JsonObject& data) {
    std::shared_ptr<ComponentExample> componentExample = std::make_shared<ComponentExample>(owner);

    const float f = static_cast<float>(data.getNumberField("testNumber"));

    componentExample->f = f;

    return componentExample;
}
