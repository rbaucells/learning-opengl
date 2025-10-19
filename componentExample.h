#pragma once
#include <memory>

#include "zero.h"

class ComponentExample final : public Component {
public:
    using Component::Component;
    static std::shared_ptr<Component> deserialize(Object* owner, const JsonObject&);
    float testNumber = 0.0;
};