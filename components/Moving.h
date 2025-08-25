#pragma once
#include "component.h"

class Moving : public Component {
public:
    // im not specifying my own constructor, use the Component constructor
    using Component::Component;
    void start() override;
    void update(double deltaTime) override;
    void fixedUpdate(double fixedDeltaTime) override;
    void onInput(const InputContext &context) override;
private:
    float speed = 0.1;
};
