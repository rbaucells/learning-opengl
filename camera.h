#pragma once
#include "math/mathematics.h"

class Camera {
public:
    ColumnMatrix4x4 viewMatrix;
    Camera(const vector2& startPos, float startRot);
    void SetCameraPosition(const vector2& newPos);
    void SetCameraRotation(float newRot);
    void MoveCamera(const vector2& change);
    void RotateCamera(float change);

    void setZoom(float value);
    void zoom(float change);
private:
    vector3 position;
    float rotation;
    float zooooom = 1.0f;
    void UpdateViewMatrix();
};
