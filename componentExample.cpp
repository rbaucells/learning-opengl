#include "componentExample.h"
#include "zero.h"

REGISTER_COMPONENT("ComponentExample", ComponentExample);

std::shared_ptr<Component> ComponentExample::deserialize(Object* owner, const JsonObject& jsonComponent) {
    std::shared_ptr componentExample = std::make_unique<ComponentExample>(owner);
    componentExample->testNumber = jsonComponent.getNumberField("testNumber");
    return componentExample;
}
