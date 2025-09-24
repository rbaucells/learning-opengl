#pragma once
#include "../systems/component.h"

class ComponentExample final : public Component {
public:
    using Component::Component;

    void start() override;
};
