#pragma once
#include "../systems/component.h"

class RotateComponent final : public Component {
private:
    float speed;
public:
    explicit RotateComponent(Object* owner, float speed);
    void start() override;
    void update(float deltaTime) override;
    void awake() override;
    void onEnable() override;
    void lateUpdate(float deltaTime) override;
    void fixedUpdate(float fixedDeltaTime) override;
    void onDisable() override;
    void onDestroy() override;
};
