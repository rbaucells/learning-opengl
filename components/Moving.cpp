#include "Moving.h"

#include "../main.h"
#include "../object.h"

bool wKeyHeld;
bool sKeyHeld;
bool aKeyHeld;
bool dKeyHeld;

void Moving::start() {
    std::printf("Hello World! \n");
    actionsToListenToo.push_back(s);
    actionsToListenToo.push_back(w);
    actionsToListenToo.push_back(a);
    actionsToListenToo.push_back(d);
}

void Moving::update(const double deltaTime) {
    if (wKeyHeld)
        object -> transform.position.y += 0.5f * deltaTime;
    if (sKeyHeld)
        object -> transform.position.y -= 0.5f * deltaTime;
    if (aKeyHeld)
        object -> transform.position.x -= 0.5f * deltaTime;
    if (dKeyHeld)
        object -> transform.position.x += 0.5f * deltaTime;
}

void Moving::fixedUpdate(const double fixedDeltaTime) {

}

void Moving::onInput(const InputContext &context) {
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

