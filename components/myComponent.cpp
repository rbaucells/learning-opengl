#include "myComponent.h"
#include "../object.h"
#include "../main.h"
#include "../camera.h"

bool wKeyHeld;
bool sKeyHeld;
bool aKeyHeld;
bool dKeyHeld;

void myComponent::start() {
    std::printf("Hello World! \n");
    actionsToListenToo.push_back(s);
    actionsToListenToo.push_back(w);
    actionsToListenToo.push_back(a);
    actionsToListenToo.push_back(d);
}

void myComponent::update(const double deltaTime) {
    if (wKeyHeld) {
        // object -> transform.position.y += 0.5f * deltaTime;
        camera -> zoom(0.1f);
    }
    if (sKeyHeld) {
        // object -> transform.position.y -= 0.5f * deltaTime;
        camera -> zoom(-0.1f);
    }
    if (aKeyHeld)
        object -> transform.position.x -= 0.5f * deltaTime;
    if (dKeyHeld)
        object -> transform.position.x += 0.5f * deltaTime;
}

void myComponent::fixedUpdate(const double fixedDeltaTime) {

}

void myComponent::onInput(const InputContext &context) {
    // button is pressed
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
    }
    // button is depresssed
    else {
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

