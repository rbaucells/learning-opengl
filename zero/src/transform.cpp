#include "transform.h"

Transform::Transform(const ComponentParams& params) : Component(params) {}

bool Transform::hasParent() const {
    return !parent_.expired();
}

void Transform::setGlobalPosition(const Vector<2>& position) {
    if (auto parent = parent_.lock()) {
        localPosition = parent->worldToLocalMatrix() * position;
    }
    else {
        localPosition = position;
    }
}

constexpr int SELECTED_ROTATION_METHOD = 1;

void Transform::setGlobalRotation(const float rotation) {
    if (auto parent = parent_.lock()) {
        auto parentLocalToWorld = parent->localToWorldMatrix();

        Matrix<2, 2> parentLinearPart = {
            {parentLocalToWorld[0][0], parentLocalToWorld[0][1]},
            {parentLocalToWorld[1][0], parentLocalToWorld[1][1]}
        };

        Matrix<2, 2> targetRotation = {
            {cosf(rotation), -sinf(rotation)},
            {sinf(rotation), cosf(rotation)}
        };

        auto m = parentLinearPart.inverse() * targetRotation;

        if constexpr (SELECTED_ROTATION_METHOD == 0) {
            localRotation = atan2(m[1][0], m[0][0]);
        }
        else {
            localRotation
        }
    }
    else {
        localRotation = rotation;
    }
}

void Transform::setGlobalScale(const Vector<2>& scale) {
    if (auto parent = parent_.lock()) {

    }
    else {
        localScale = scale;
    }
}

Vector<2> Transform::getGlobalPosition() const {
    if (auto parent = parent_.lock()) {
        auto localToWorld = localToWorldMatrix();
        const Vector<2> pos = {localToWorld[0][3], localToWorld[1][3]};
        return pos;
    }
    else {
        return localPosition;
    }
}

float Transform::getGlobalRotation(const RotationType type) const {
    if (auto parent = parent_.lock()) {
        auto localToWorld = localToWorldMatrix();
        const float rot = atan2f(localToWorld[1][0], localToWorld[0][0]);

        if (type == RotationType::radians)
            return rot;
        else
            return rot * (180.0 / M_PI);
    }
    else {
        return localRotation;
    }
}

Vector<2> Transform::getGlobalScale() const {
    if (auto parent = parent_.lock()) {
        auto localToWorld = localToWorldMatrix();
        // get the magnitude of x
        const float scaleX = sqrt(std::pow(localToWorld[0][0], 2) + std::pow(localToWorld[1][0], 2));
        // get the magnitude of y
        const float scaleY = sqrt(std::pow(localToWorld[0][1], 2) + std::pow(localToWorld[1][1], 2));
        // return it as Vector<2>
        return {scaleX, scaleY};
    }
    else {
        return localScale;
    }
}

Matrix<4, 4> Transform::localToWorldMatrix() const {
    Matrix<4, 4> transformationMatrix = Matrix<4, 4>::identity();

    transformationMatrix = transformationMatrix.translate(localPosition[0], localPosition[1], 0);
    transformationMatrix = transformationMatrix.rotateZ(localRotation);
    transformationMatrix = transformationMatrix.scaleAnisotropic(localScale[0], localScale[1], 1);

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
