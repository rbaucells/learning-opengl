#include "glad/gl.h"
#include "stb_image.h"
#include "components/component.h"
#include "object.h"

#include <vector>

Object::Object(const Transform &transform) {
    this -> transform = transform;

    allObjects.push_back(this);
}

Object::~Object() {
    std::erase(allObjects, this);
}

void Object::update(const double deltaTime) const {
    for (const auto& component: components) {
        component -> update(deltaTime);
    }
}
void Object::fixedUpdate(const double fixedDeltaTime) const {
    for (const auto& component: components) {
        component -> fixedUpdate(fixedDeltaTime);
    }
}
void Object::lateUpdate(const double deltaTime) const {
    for (const auto& component: components) {
        component -> lateUpdate(deltaTime);
    }
}