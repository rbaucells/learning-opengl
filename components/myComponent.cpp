#include "myComponent.h"

#include "renderer.h"
#include "../object.h"

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
        renderer -> alpha += 0.05f;
    }
    if (sKeyHeld) {
        renderer -> alpha -= 0.05f;
    }

    renderer->alpha = clamp<float>(renderer->alpha, 0, 1);
}

void myComponent::fixedUpdate(const double fixedDeltaTime) {

}

void myComponent::onInput(const InputContext &context) {
    if (context.state) {
        switch (context.action) {
            case w:
                std::printf("Set w key to held \n");
                wKeyHeld = true;
                break;
            case a:
                aKeyHeld = true;
                break;
            case s:
                std::printf("Set s key to held \n");
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
                std::printf("Set w key to not held \n");
                wKeyHeld = false;
                break;
            case a:
                aKeyHeld = false;
                break;
            case s:
                std::printf("Set w key to not held \n");
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
