#pragma once
#include <vector>

#include "object.h"
#include "transform.h"
#include "math/vector2.h"

class Object;

class Scene {
public:
    explicit Scene(bool main = true);

    void update(float deltaTime) const;
    void fixedUpdate(float fixedDeltaTime) const;
    void lateUpdate(float lateDeltaTime) const;

    void manageStarts() const;
    void manageDestructions();

#pragma region Object things
    std::shared_ptr<Object> addObject(const std::string& objectName, int objectTag, Vector2 pos, float rot, Vector2 scale);
    std::shared_ptr<Object> addObject(const std::string& objectName, int objectTag, Vector2 pos, float rot, Vector2 scale, Transform* parent);

    void removeObject(const std::shared_ptr<Object>& objectPtr);
    void removeObjectByTag(int tag);
    void removeObjectByName(const std::string& name);
    void removeObjectBy(const std::function<bool(const Object&)>& predicate);
#pragma endregion
    void setMain();

    static Scene* mainScene;

private:
    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<std::shared_ptr<Object>> objectsToDestroy_;

    // friend class Object;
};
