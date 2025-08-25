#pragma once
#include "component.h"

class myComponent : public Component {
private:
    vector2 mousePos;
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
