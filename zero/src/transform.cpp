#include "transform.h"

Transform::Transform(const ComponentParams& params) : Component(params) {}

bool Transform::hasParent() const {
    return !parent_.expired();
}

void Transform::setGlobalPosition(const Vector2& position) {
    if (auto parent = parent_.lock()) {
        localPosition = parent->worldToLocalMatrix() * position;
    }
    else {
        localPosition = position;
    }
}

void Transform::setGlobalRotation(const float rotation) {
    if (auto parent = parent_.lock()) {

    }
    else {
        localRotation = rotation;
    }
}

void Transform::setGlobalScale(const Vector2& scale) {
    if (!hasParent()) {
        localScale = scale;
    }
}

Vector2 Transform::getGlobalPosition() const {
    if (auto parent = parent_.lock()) {
        auto localToWorld = localToWorldMatrix();
        const Vector2 pos = {localToWorldMatrix()[0][3], localToWorld[1][3]};
        return pos;
    }
    else {
        return localPosition;
    }
}

float Transform::getGlobalRotation() const {
    if (auto parent = parent_.lock()) {
        auto localToWorld = localToWorldMatrix();
        const float rot = atan2f(localToWorld[1][0], localToWorld[0][0]);
        return rot;
    }
    else {
        return localRotation;
    }
}

Vector2 Transform::getGlobalScale() const {
    if (auto parent = parent_.lock()) {
        auto localToWorld = localToWorldMatrix();
        // get the magnitude of x
        const float scaleX = sqrt(std::pow(localToWorld[0][0], 2) + std::pow(localToWorld[1][0], 2));
        // get the magnitude of y
        const float scaleY = sqrt(std::pow(localToWorld[0][1], 2) + std::pow(localToWorld[1][1], 2));

        return {scaleX, scaleY};
    }
    else {
        return localScale;
    }
}

Matrix<4, 4> Transform::localToWorldMatrix() const {
    Matrix<4, 4> transformationMatrix = Matrix<4, 4>::identity();

    transformationMatrix = transformationMatrix.translate(localPosition.x, localPosition.y, 0);
    transformationMatrix = transformationMatrix.rotateZ(localRotation);
    transformationMatrix = transformationMatrix.scaleAnisotropic(localScale.x, localScale.y, 1);

    if (const auto parent = parent_.lock()) {
        return parent->localToWorldMatrix() * transformationMatrix;
    }
    else {
        return transformationMatrix;
    }
}

Matrix<4, 4> Transform::worldToLocalMatrix() const {
    return localToWorldMatrix().inverse();
}