#pragma once
#include "component.h"

class Moving : public Component {
public:
    // im not specifying my own constructor, use the Component constructor
    using Component::Component;
    void start() override;
    void update() override;
private:
    float speed = 1;
};
