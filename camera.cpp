#include "camera.h"

Camera::Camera(const Vector2 &startPos, const float startRot) {
    viewMatrix = Matrix<4, 4>::identity<4>();
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
    Matrix<4, 4> transformationMatrix = Matrix<4, 4>::identity<4>();

    transformationMatrix = transformationMatrix.translate(-position.x, -position.y, -position.z);
    transformationMatrix = transformationMatrix.rotateZ(rotation);
    transformationMatrix = transformationMatrix.scaleAnisotropic(zooooom, zooooom, 1.0f);

    viewMatrix = transformationMatrix;
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
