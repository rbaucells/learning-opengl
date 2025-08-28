#include "myComponent.h"

#include "renderer.h"
#include "../object.h"

myComponent::myComponent(Object* owner, float speed) : Component(owner) {
    this->speed = speed;
}


void myComponent::start() {
    std::printf("Hello World! \n");
    w_event.subscribe(this, &myComponent::onUpInput);
    up_event.subscribe(this, &myComponent::onUpInput);

    s_event.subscribe(this, &myComponent::onDownInput);
    down_event.subscribe(this, &myComponent::onDownInput);

    renderer = object->GetComponent<Renderer>();
}

void myComponent::update(const double deltaTime) {
    if (wKeyHeld) {
        renderer -> alpha += 0.05f;
    }
    if (sKeyHeld) {
        renderer -> alpha -= 0.05f;
    }

    renderer->alpha = clamp<float>(renderer->alpha, 0, 1);
}

void myComponent::fixedUpdate(const double fixedDeltaTime) {

}

void myComponent::onDownInput(bool pressed) {
    sKeyHeld = pressed;
}

void myComponent::onUpInput(bool pressed) {
    wKeyHeld = pressed;
}

