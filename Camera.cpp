#include <cstdio>

#include "camera.h"

Camera::Camera(const vector2& startPos, const float startRot) {
    viewMatrix = ColumnMatrix4x4::identity();
    position = vector3(startPos.x, startPos.y, 1);
    rotation = startRot;
    UpdateViewMatrix();
}

void Camera::SetCameraPosition(const vector2& newPos) {
    position = vector3(newPos.x, newPos.y, position.z);
    UpdateViewMatrix();
}

void Camera::SetCameraRotation(const float newRot) {
    rotation = newRot;
    UpdateViewMatrix();
}

void Camera::MoveCamera(const vector2& change) {
    position += vector3(change.x, change.y, 0);
    UpdateViewMatrix();
}

void Camera::RotateCamera(const float change) {
    rotation += change;
    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix() {
    ColumnMatrix4x4 translationMatrix = ColumnMatrix4x4::identity();
    translationMatrix = translationMatrix.translate(-position.x, -position.y, -position.z);

    ColumnMatrix4x4 rotationMatrix = ColumnMatrix4x4::identity();
    rotationMatrix = rotationMatrix.rotate_z(rotation);

    ColumnMatrix4x4 scaleMatrix = ColumnMatrix4x4::identity();
    scaleMatrix = scaleMatrix.scale_anisotropic(zooooom, zooooom, 1.0f);

    viewMatrix = scaleMatrix * rotationMatrix * translationMatrix;
}

void Camera::setZoom(const float value) {
    if (value > 0) {
        zooooom = value;
        std::printf("new camera zoom %f", value);
        UpdateViewMatrix();
    }
    else {
        zooooom = 0;
        std::printf("new camera zoom %d", 0);
        UpdateViewMatrix();
    }
}

void Camera::zoom(const float change) {
    setZoom(zooooom + change);
}

