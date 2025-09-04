#include "camera.h"

Camera::Camera(const Vector2 &startPos, const float startRot) {
    viewMatrix = ColumnMatrix4X4::identity();
    position = Vector3(startPos.x, startPos.y, 1);
    rotation = startRot;
    updateViewMatrix();
}

void Camera::setCameraPosition(const Vector2 &newPos) {
    position = Vector3(newPos.x, newPos.y, position.z);
    updateViewMatrix();
}

void Camera::setCameraRotation(const float newRot) {
    rotation = newRot;
    updateViewMatrix();
}

void Camera::moveCamera(const Vector2 &change) {
    position += Vector3(change.x, change.y, 0);
    updateViewMatrix();
}

void Camera::rotateCamera(const float change) {
    rotation += change;
    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    ColumnMatrix4X4 translationMatrix = ColumnMatrix4X4::identity();
    translationMatrix = translationMatrix.translate(-position.x, -position.y, -position.z);

    ColumnMatrix4X4 rotationMatrix = ColumnMatrix4X4::identity();
    rotationMatrix = rotationMatrix.rotate_z(rotation);

    ColumnMatrix4X4 scaleMatrix = ColumnMatrix4X4::identity();
    scaleMatrix = scaleMatrix.scale_anisotropic(zooooom, zooooom, 1.0f);

    viewMatrix = scaleMatrix * rotationMatrix * translationMatrix;
}

void Camera::setZoom(const float value) {
    if (value > 0) {
        zooooom = value;
        std::printf("new camera zoom %f", value);
        updateViewMatrix();
    }
    else {
        zooooom = 0;
        std::printf("new camera zoom %d", 0);
        updateViewMatrix();
    }
}

void Camera::zoom(const float change) {
    setZoom(zooooom + change);
}
