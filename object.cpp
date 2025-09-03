#include "components/component.h"
#include "object.h"
#include "main.h"

#include <vector>

#include "workQueue.h"

/**
 * @brief constructs a new Object.
 * @param objectName the name of the object.
 * @param objectTag the tag of the object.
 * @param transform the initial transform of the object.
 */
Object::Object(const std::string &objectName, const int objectTag, const Transform &transform) : name(objectName), tag(objectTag), transform(transform) {
    allObjects.push_back(this);

    // subscribe to the main game loop events
    updateEvent.subscribe(this, &Object::update);
    lateUpdateEvent.subscribe(this, &Object::lateUpdate);
    fixedUpdateEvent.subscribe(this, &Object::fixedUpdate);
}

void Object::onEnable() const {
    for (const auto &component : components) {
        component->onEnable();
    }
}

void Object::onDisable() const {
    for (const auto &component : components) {
        component->onDisable();
    }
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
 * @brief marks the object for destruction, at the end of the frame it will be destroyed
 *
 * it will clear all components, remove the object from the global list of objects,
 * and then delete the object instance.
 */
void Object::markForDestroy() {
    markedForDestruction = true;
    markedForDestructionObjects.push_back(this);
}

/**
 * @brief removes all attached components, of whatever type
 */
void Object::removeAllComponents() {
    for (const auto component : components) {
        delete component;
    }

    components.clear();
}

bool Object::getActive() const {
    return activated;
}

/**
 * @brief sets the activated state of the object. calling the appropriate on### function on its components
 * @param state the value to set activated to
 */
void Object::setActive(const bool state) {
    // if we were activated but arent gonna be anymore
    if (activated && !state) {
        onDisable();
    }
    else if (!activated && state) {
        onEnable();
    }

    activated = state;
}

/**
 * @brief tells all the components its time to go bye bye and kills himself
 *
 * also removes any queue items owned by the components
 */
void Object::onDestroy() {
    transform.removeAllChildren();
    // only call on destroy if its activated
    if (activated) {
        setActive(false);
        for (Component* component : components) {
            component->onDestroy();
        }
    }
    else {
        setActive(false);
    }

    std::erase(allObjects, this);

    for (const Component* component : components) {
        // if something in the queue is owned by that compoennt in one of the queues remove it
        WorkQueue::removeAllTimedQueue(component);
        WorkQueue::removeAllConditionalQueue(component);
        WorkQueue::removeAllNextFrameQueue(component);

        delete component;
    }

    components.clear();
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