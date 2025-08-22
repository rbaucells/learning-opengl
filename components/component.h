#pragma once
#include <iostream>

// Forward declare the Object class.
class Object;

class component {
protected:
    Object* object = nullptr;
public:
    virtual ~component() = default;

    virtual void Start() {};
    virtual void Update() {};
    virtual void LateUpdate() {};
    virtual void FixedUpdate() {};

    void setMaster(Object* owner) {
        object = owner;
    }
};