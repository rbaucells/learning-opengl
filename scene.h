#pragma once
#include <vector>

class Object;

class Scene {
public:
    explicit Scene(bool main = true);

    void update(float deltaTime) const;
    void fixedUpdate(float fixedDeltaTime) const;
    void lateUpdate(float lateDeltaTime) const;

    void manageStarts() const;
    void manageDestructions();

    void setMain();

    static Scene* mainScene;

private:
    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<std::shared_ptr<Object>> objectsToDestroy_;

    friend class Object;
};
