#include "systems/component.h"
#include "object.h"
#include "main.h"

#include <vector>

#include "systems/workQueue.h"

Object::Object(const std::string &objectName, const int objectTag, const Vector2 pos, const float rot, const Vector2 scale) : name(objectName), tag(objectTag), transform(this, pos, rot, scale) {
    allObjects.push_back(this);

    // subscribe to the main game loop events
    updateEvent.subscribe(this, &Object::update);
    lateUpdateEvent.subscribe(this, &Object::lateUpdate);
    fixedUpdateEvent.subscribe(this, &Object::fixedUpdate);
}

Object::Object(const std::string &objectName, const int objectTag, Vector2 pos, float rot, Vector2 scale, Transform *parent) : name(objectName), tag(objectTag), transform(this, pos, rot, scale) {
    allObjects.push_back(this);

    // subscribe to the main game loop events
    updateEvent.subscribe(this, &Object::update);
    lateUpdateEvent.subscribe(this, &Object::lateUpdate);
    fixedUpdateEvent.subscribe(this, &Object::fixedUpdate);
}

/**
 * @brief called every frame, if the object is activated.
 *
 * it calls the update method on all of its components.
 * @param deltaTime the time elapsed since the last frame.
 */
void Object::update(double deltaTime) const {
    if (!activated)
        return;

    for (const auto &component: components) {
        component->update(deltaTime);
    }
}

/**
 * @brief called at a fixed time interval, if the object is activated.
 *
 * it calls the fixedUpdate method on all of its components.
 * @param fixedDeltaTime the fixed time step.
 */
void Object::fixedUpdate(double fixedDeltaTime) const {
    if (!activated)
        return;

    for (const auto &component: components) {
        component->fixedUpdate(fixedDeltaTime);
    }
}

/**
 * @brief called every frame after all update methods have been called, if the object is activated.
 *
 * it calls the lateUpdate method on all of its components.
 * @param deltaTime the time elapsed since the last frame.
 */
void Object::lateUpdate(double deltaTime) const {
    if (!activated)
        return;

    for (const auto &component: components) {
        component->lateUpdate(deltaTime);
    }
}

/**
 * @brief Puts this object into the waiting line to be destroyed at the end of the frame
 */
void Object::destroy() {
    markedForDeath = true;
    waitingLineOfDeath.push_back(this);
}

/**
 * @brief Immediately destroys the object, no waiting for end of frame
 * @warning Can be dangerous if ran in update loop and then something else tries to access this
 */
void Object::destroyImmediately() {
    // we need to deactivate before destroying
    if (getActive()) {
        for (const auto& component : components) {
            component->onDisable();
        }
    }

    for (const auto& component : components) {
        component->onDestroy();
    }

    components.clear();
    std::erase(allObjects, this);
    removeAllQueueEntries(this);
    // don't worry about transform, destructor will clean up for us
}

bool Object::getActive() const {
    return activated;
}

void Object::setActive(const bool state) {
    // if activated but not anymore
    if (activated && !state) {
        for (const auto& component : components) {
            component->onDisable();
        }
    }
    // if not activated but are now
    else if (!activated && state) {
        for (const auto& component : components) {
            component->onEnable();
        }
    }

    activated = state;
}


// static methods

/**
 * @brief finds the first object in the scene with the specified name.
 * @param name the name of the object to find.
 * @return a pointer to the found object, or nullptr if no object is found.
 */
Object *Object::findObjectByName(const std::string &name) {
    for (Object *obj: allObjects) {
        if (obj->name == name)
            return obj;
    }

    return nullptr;
}

/**
 * @brief finds the first object in the scene with the specified tag.
 * @param tag the tag of the object to find.
 * @return a pointer to the found object, or nullptr if no object is found.
 */
Object *Object::findObjectByTag(const int tag) {
    for (Object *obj: allObjects) {
        if (obj->tag == tag)
            return obj;
    }

    return nullptr;
}

/**
 * @brief finds all objects in the scene with the specified name.
 * @param name the name of the objects to find.
 * @return a vector of pointers to the found objects.
 */
std::vector<Object *> Object::findObjectsByName(const std::string &name) {
    std::vector<Object *> data;

    for (Object *obj: allObjects) {
        if (obj->name == name)
            data.push_back(obj);
    }

    return data;
}

/**
 * @brief finds all objects in the scene with the specified tag.
 * @param tag the tag of the objects to find.
 * @return a vector of pointers to the found objects.
 */
std::vector<Object *> Object::findObjectsByTag(const int tag) {
    std::vector<Object *> data;

    for (Object *obj: allObjects) {
        if (obj->tag == tag)
            data.push_back(obj);
    }

    return data;
}
