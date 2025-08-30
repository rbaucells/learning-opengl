#pragma once
#include "component.h"
#include "../event.h"

class Renderer;

class RotateComponent : public Component {
private:
    float speed;
public:
    explicit RotateComponent(Object* owner, float speed);
    void start() override;
    void update(double deltaTime) override;
};
