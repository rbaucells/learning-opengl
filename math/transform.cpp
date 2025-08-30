#include "mathematics.h"
#include <cmath>

struct Decomposed2D {
    vector2 position;
    float rotation; // degrees
    vector2 scale;
};

Decomposed2D decompose2D(const ColumnMatrix4x4& m) {
    // TODO: what did chatgpt do in here?
    Decomposed2D out;

    // Translation
    out.position = { m.data[3][0], m.data[3][1] };

    // Raw basis vectors
    vector2 col0 = { m.data[0][0], m.data[0][1] };
    vector2 col1 = { m.data[1][0], m.data[1][1] };

    // Scale from raw lengths
    float scaleX = col0.Magnitude();
    float scaleY = col1.Magnitude();

    // Normalize basis vectors
    if (scaleX != 0) col0 /= scaleX;
    if (scaleY != 0) col1 /= scaleY;

    // Remove shear: make col1 perpendicular to col0
    float shear = col0.Dot(col1);
    col1 -= col0 * shear;

    // Recompute scaleY after removing shear
    scaleY = col1.Magnitude();
    if (scaleY != 0) col1 /= scaleY;

    out.scale = { scaleX, scaleY };

    // Rotation from orthonormalized X axis
    float radians = std::atan2(col0.y, col0.x);
    float degrees = radians * (180.f / static_cast<float>(M_PI));
    if (degrees < 0) degrees += 360.f;
    out.rotation = degrees;

    return out;
}

Transform::Transform(const vector2 pos, const float rot, const vector2 scale) {
    setGlobalPosition(pos);
    setGlobalRotation(rot);
    setGlobalScale(scale);
}

Transform::Transform(vector2 pos, float rot, vector2 scale, const Transform *parent) {
    this->parent = parent;
    this->localPosition = pos;
    this->localRotation = rot;
    this->localScale = scale;
}

vector2 Transform::getGlobalPosition() const {
    return decompose2D(localToWorldMatrix()).position;
}

float Transform::getGlobalRotation() const {
    if (parent != nullptr) {
        return fmod(parent->getGlobalRotation() + localRotation + 360.0f, 360.0f);
    }
    return fmod(localRotation + 360.0f, 360.0f);
}

vector2 Transform::getGlobalScale() const {
    const ColumnMatrix4x4 m = localToWorldMatrix();

    // get global rotation and then turn it into radians
    const float rotationRadians = getGlobalRotation() * (static_cast<float>(M_PI) / 180.0f);
    const float cosR = std::cos(rotationRadians);
    const float sinR = std::sin(rotationRadians);

    // undo rotation of columns to extract scale
    const float sx =  m.data[0][0] * cosR + m.data[0][1] * sinR;
    const float sy = -m.data[1][0] * sinR + m.data[1][1] * cosR;

    return { sx, sy };
}

void Transform::setGlobalPosition(const vector2 pos) {
    if (parent != nullptr) {
        const vector2 localPos = parent->worldToLocalMatrix() * pos;
        localPosition = localPos;
    }
    else {
        localPosition = pos;
    }
}

void Transform::setGlobalRotation(const float rot) {
    if (parent != nullptr) {
        const float parentGlobalRotation = parent->getGlobalRotation();
        const float newGlobalRotation = rot - parentGlobalRotation;
        localRotation = newGlobalRotation;
    }
    else {
        localRotation = rot;
    }
}
void Transform::setGlobalScale(const vector2 desiredGlobalScale) {
    // TODO: Figure out wth chatgpt did in this function because i cant understand any of it
    if (!parent) {
        // Always store positive magnitudes (no mirroring)
        localScale = { std::abs(desiredGlobalScale.x), std::abs(desiredGlobalScale.y) };
        return;
    }

    // 1) Parent linear (2x2) columns in column-major storage
    const ColumnMatrix4x4 pm = parent->localToWorldMatrix();
    const vector2 pCol0 { pm.data[0][0], pm.data[0][1] }; // first column
    const vector2 pCol1 { pm.data[1][0], pm.data[1][1] }; // second column

    // 2) Child local rotation matrix components
    const float deg2rad = static_cast<float>(M_PI) / 180.0f;
    const float c = std::cos(localRotation * deg2rad);
    const float s = std::sin(localRotation * deg2rad);

    // Columns of Lp * Rc:
    // u = Lp * [c, s], v = Lp * [-s, c]  (linear combo of columns)
    const vector2 u = pCol0 * c + pCol1 * s;
    const vector2 v = pCol0 * (-s) + pCol1 * c;

    // 3) Child global rotation (independent of scale magnitude)
    const float Rg = getGlobalRotation() * deg2rad;
    const float cg = std::cos(Rg);
    const float sg = std::sin(Rg);

    // 4) Denominators that your getter uses implicitly
    float denomX = u.x * cg + u.y * sg;          // = dot(u, x̂_global)
    float denomY = -v.x * sg + v.y * cg;         // = dot(v, ŷ_global)

    // 5) Avoid division blow-ups
    const float eps = 1e-8f;
    if (std::fabs(denomX) < eps) denomX = (denomX < 0 ? -eps : eps);
    if (std::fabs(denomY) < eps) denomY = (denomY < 0 ? -eps : eps);

    // 6) Solve for local scale, store as positive magnitudes (no negative scale)
    float lx = desiredGlobalScale.x / denomX;
    float ly = desiredGlobalScale.y / denomY;

    localScale = { std::fabs(lx), std::fabs(ly) };
}

ColumnMatrix4x4 Transform::localToWorldMatrix() const {
    ColumnMatrix4x4 transformationMatrix = ColumnMatrix4x4::identity();

    transformationMatrix = transformationMatrix.translate(localPosition.x, localPosition.y, 0);
    transformationMatrix = transformationMatrix.rotate_z(localRotation);
    transformationMatrix = transformationMatrix.scale_anisotropic(localScale.x, localScale.y, 1);

    if (parent != nullptr) {
        return parent->localToWorldMatrix().multiply(transformationMatrix);
    }

    return transformationMatrix;
}

ColumnMatrix4x4 Transform::worldToLocalMatrix() const {
    ColumnMatrix4x4 localToWorld = localToWorldMatrix();
    ColumnMatrix4x4 inverse = localToWorld.inverse();
    return inverse;
}

