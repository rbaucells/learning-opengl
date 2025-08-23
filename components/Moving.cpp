#include "Moving.h"
#include "../object.h"

// Moving::Moving(Object *owner, float speed) : Component(owner) {
//     this->speed = speed;
// }

void Moving::start() {
    std::printf("Hello World!");
}

void Moving::update() {
    object->transform.position.x += speed;
}

