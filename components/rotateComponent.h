#pragma once
#include "component.h"

class Renderer;

class RotateComponent final : public Component {
private:
    float speed;
public:
    explicit RotateComponent(Object* owner, float speed);
    void awake() override;
    void onEnable() override;
    void start() override;
    void update(double deltaTime) override;
    void lateUpdate(double deltaTime) override;
    void fixedUpdate(double fixedDeltaTime) override;
    void onDisable() override;
    void onDestroy() override;
};
