#pragma once
#include "math/mathematics.h"

class Camera {
public:
    ColumnMatrix4X4 viewMatrix;
    Camera(const Vector2& startPos, float startRot);
    void SetCameraPosition(const Vector2& newPos);
    void SetCameraRotation(float newRot);
    void MoveCamera(const Vector2& change);
    void RotateCamera(float change);

    void setZoom(float value);
    void zoom(float change);
private:
    Vector3 position;
    float rotation;
    float zooooom = 1.0f;
    void UpdateViewMatrix();
};
