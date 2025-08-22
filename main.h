#pragma once
#include "math/Math.h"
#include <vector>
#include "components/component.h"

// variables
inline int SCREEN_WIDTH = 600;
inline int SCREEN_HEIGHT = 400;

// structs
struct Buffers {
    unsigned int vertexBuffer;
    unsigned int indexBuffer;
};

// classes
class Camera {
public:
    mat4x4 viewMatrix;
    Camera(vector2 startPos, float startRot);
    void SetCameraPosition(vector2 newPos);
    void SetCameraRotation(float newRot);
    void MoveCamera(vector2 change);
    void RotateCamera(float change);
private:
    vector3 position;
    float rotation;

    void UpdateViewMatrix();
};

template <typename T>
concept IsComponent = std::is_base_of_v<component, T>;

class Object {
public:
    std::vector<std::unique_ptr<component>> components;
    Transform transform;

    Object(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int shaders, Transform transform);
    void Define(unsigned int usage, const std::string& textureFilePath);
    void Draw(unsigned int mode, mat4x4 view, mat4x4 projection) const;

    void Update();
    void FixedUpdate();
    void LateUpdate();

    template<IsComponent T>
    T* AddComponent() {
        auto newComponent = std::make_unique<T>();

        newComponent -> setMaster(this);
        newComponent -> Start();

        T* componentPtr = newComponent.get();
        components.push_back(std::move(newComponent));

        return componentPtr;
    }

    template<IsComponent T>
    T* GetComponent() {
        // loop until
        for (const std::unique_ptr<component>& comp : components) {
            // one of them is the correct type
            if (T* specificComp = dynamic_cast<T*>(comp.get())) {
                return specificComp;
            }
        }
        // nothing was found
        return nullptr;
    }

    template <IsComponent T>
    std::vector<T*> GetComponents() {
        // same as GetComponent only it adds it to a vector then returns it
        std::vector<T*> foundComponents;
        for (const std::unique_ptr<component>& comp : components) {
            if (T* specificComp = dynamic_cast<T*>(comp.get())) {
                foundComponents.push_back(specificComp);
            }
        }
        return foundComponents;
    }

    ~Object();
private:
    Buffers buffers = {};
    std::vector<Vertex> vertices = {};
    std::vector<unsigned int> indices = {};
    unsigned int shaders = 0;
    unsigned int vertexArrayObject = 0;
    unsigned int texture = 0;
};

inline std::vector<Object*> allObjects;

// functions
Buffers definePrimitive(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int usage);
void drawPrimitive(unsigned int indexBuffer, int indicesCount, unsigned int mode, unsigned int vao, unsigned int texture);