#include "myComponent.h"

#include "renderer.h"
#include "../object.h"

bool wKeyHeld;
bool aKeyHeld;
bool sKeyHeld;
bool dKeyHeld;

Renderer* renderer;

myComponent::myComponent(Object* owner, float speed) : Component(owner) {
    this->speed = speed;
}


void myComponent::start() {
    std::printf("Hello World! \n");
    actionsToListenToo.push_back(w);
    actionsToListenToo.push_back(a);
    actionsToListenToo.push_back(s);
    actionsToListenToo.push_back(d);

    renderer = object->GetComponent<Renderer>();
}

void myComponent::update(const double deltaTime) {
    if (wKeyHeld) {
        if (renderer->alpha < 1)
            renderer -> alpha += 0.05f;
    }

    if (sKeyHeld) {
        if (renderer->alpha > 0)
            renderer -> alpha -= 0.05f;
    }
    // if (wKeyHeld) {
    //     object->transform.position.y += speed * deltaTime;
    // }
    //
    // if (aKeyHeld) {
    //     object->transform.position.x -= speed * deltaTime;
    // }
    //
    // if (sKeyHeld) {
    //     object->transform.position.y -= speed * deltaTime;
    // }
    //
    // if (dKeyHeld) {
    //     object->transform.position.x += speed * deltaTime;
    // }
}

void myComponent::fixedUpdate(const double fixedDeltaTime) {
}

void myComponent::onInput(const InputContext &context) {
    if (context.state) {
        switch (context.action) {
            case w:
                wKeyHeld = true;
                break;
            case a:
                aKeyHeld = true;
                break;
            case s:
                sKeyHeld = true;
                break;
            case d:
                dKeyHeld = true;
                break;
            default:
                break;
        }
    } else {
        switch (context.action) {
            case w:
                wKeyHeld = false;
                break;
            case a:
                aKeyHeld = false;
                break;
            case s:
                sKeyHeld = false;
                break;
            case d:
                dKeyHeld = false;
                break;
            default:
                break;
        }
    }
}
