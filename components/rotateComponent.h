#pragma once
#include "../systems/component.h"

class RotateComponent final : public Component {
private:
    float speed;
public:
    explicit RotateComponent(Object* owner, float speed);
    void start() override;
    void update(double deltaTime) override;
    void awake() override;
    void onEnable() override;
    void lateUpdate(double deltaTime) override;
    void fixedUpdate(double fixedDeltaTime) override;
    void onDisable() override;
    void onDestroy() override;
};
