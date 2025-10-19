#pragma once
#include "zero.h"

class ComponentExample {
public:
    std::function<std::shared_ptr<Component>(Object* owner, const JsonObject&)> deserialize;
};