#pragma once
#include <vector>

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

    std::shared_ptr<Object> addObject(const std::string& objectName, int objectTag, Vector2 pos, float rot, Vector2 scale);
    std::shared_ptr<Object> addObject(const std::string& objectName, int objectTag, Vector2 pos, float rot, Vector2 scale, Transform* parent);

    void setMain();

    static Scene* mainScene;

private:
    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<std::shared_ptr<Object>> objectsToDestroy_;

    friend class Object;
};
