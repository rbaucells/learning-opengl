#pragma once
#include "../systems/component.h"
#include "../systems/event.h"

class Renderer;

class RotateComponent : public Component {
private:
    float speed;
public:
    explicit RotateComponent(Object* owner, float speed);
    void start() override;
    void update(double deltaTime) override;
};
