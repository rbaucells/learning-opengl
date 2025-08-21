#pragma once

#include <vector>
#include "Math.h"
#include "linmath/linmath.h"
static int SCREEN_WIDTH = 600;
static int SCREEN_HEIGHT = 400;
// structs
struct Buffers {
    unsigned int vertexBuffer;
    unsigned int indexBuffer;
};

struct Transform {
    Vector2 position;
    float rotation;
    Vector2 scale;
};

// functions

Buffers definePrimative(Vertex vertices[], const int vertexCount, unsigned int indices[], const int indexCount, unsigned int usage, unsigned int texture);
void drawPrimative(unsigned int indexBuffer, const int indicesCount, unsigned int mode, unsigned int vao, unsigned int texture);

class Drawable {
public:
    unsigned int shaders = 0;
    unsigned int vertexArrayObject = 0;
    std::vector<Vertex> vertices = {};
    std::vector<unsigned int> indices = {};
    int indicesCount = 0;
    int verticesCount = 0;
    unsigned int texture = 0;
    Transform transform;

    Drawable(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int shaders, Transform transform);
    void Define(unsigned int usage, std::string textureFilePath);
    void redefineObject(Vertex newVertices[], int newVerticesCount, unsigned int newIndices[], int newIndicesCount, unsigned int usage, std::string filePath);
    void Draw(unsigned int mode, mat4x4 view, mat4x4 projection) const;
private:
    Buffers buffers = {};
};

class Camera {
public:
    mat4x4 viewMatrix;
    Camera(Vector2 startPos, float startRot);
    void SetCameraPosition(Vector2 newPos);
    void SetCameraRotation(float newRot);
    void MoveCamera(Vector2 change);
    void RotateCamera(float change);
private:
    Vector3 position;
    float rotation;

    void UpdateViewMatrix();
};