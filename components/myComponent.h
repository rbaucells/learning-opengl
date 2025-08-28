#pragma once
#include "component.h"
#include "../event.h"

class Renderer;

class myComponent : public Component {
private:
    float speed;

    bool wKeyHeld;
    bool aKeyHeld;
    bool sKeyHeld;
    bool dKeyHeld;

    Renderer* renderer;

    void onUpInput(bool pressed);
    void onDownInput(bool pressed);
public:
    // im not specifying my own constructor, use the Component constructor
    // using Component::Component;
    explicit myComponent(Object* owner, float speed);
    void start() override;
    void update(double deltaTime) override;
    void fixedUpdate(double fixedDeltaTime) override;
};
