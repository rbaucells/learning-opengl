#pragma once
#include "zero.h"

class ComponentExample {
public:
    static std::shared_ptr<Component> deserialize(Object* owner, const JsonObject&);
};