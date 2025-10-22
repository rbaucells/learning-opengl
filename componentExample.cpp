#include "componentExample.h"

#include "json++/json.h"
#include "zero/zero.h"

REGISTER_COMPONENT("ComponentExample", ComponentExample);

std::shared_ptr<Component> ComponentExample::deserialize(Object* owner, const JsonObject& jsonComponent) {
    std::string id = jsonComponent.getStringField("id");
    std::shared_ptr<ComponentExample> componentExample = std::make_shared<ComponentExample>(ComponentParams(owner, id));
    componentExample->testNumber = jsonComponent.getNumberField("testNumber");
    return componentExample;
}
