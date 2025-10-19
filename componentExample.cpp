#include "componentExample.h"
#include "zero.h"

REGISTER_COMPONENT("ComponentExample", ComponentExample);

std::shared_ptr<Component> ComponentExample::deserialize(Object* owner, const JsonObject&) {
    return {};
}
