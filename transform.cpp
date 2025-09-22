#include "transform.h"
#include "object.h"

struct Decomposed2D {
    Vector2 position{};
    float rotation{}; // degrees
    Vector2 scale{};
};

Decomposed2D decompose2D(const Matrix<4, 4> &m) {
    // TODO: what did chatgpt do in here?
    Decomposed2D out;

    // Translation
    out.position = {m.data[3][0], m.data[3][1]};

    // Raw basis vectors
    Vector2 col0 = {m.data[0][0], m.data[0][1]};
    Vector2 col1 = {m.data[1][0], m.data[1][1]};

    // Scale from raw lengths
    float scaleX = col0.magnitude();
    float scaleY = col1.magnitude();

    // Normalize basis vectors
    if (scaleX != 0)
        col0 /= scaleX;
    if (scaleY != 0)
        col1 /= scaleY;

    // Remove shear: make col1 perpendicular to col0
    float shear = col0.dot(col1);
    col1 -= col0 * shear;

    // Recompute scaleY after removing shear
    scaleY = col1.magnitude();
    if (scaleY != 0)
        col1 /= scaleY;

    out.scale = {scaleX, scaleY};

    // Rotation from orthonormalized X axis
    float radians = std::atan2(col0.y, col0.x);
    float degrees = radians * (180.f / static_cast<float>(M_PI));
    if (degrees < 0)
        degrees += 360.f;
    out.rotation = degrees;

    return out;
}

Transform::Transform(Object *owner, const Vector2 pos, const float rot, const Vector2 scale, Transform *parent) : Component(owner) {
    setParent(parent);
    this->localPosition = pos;
    this->localRotation = rot;
    this->localScale = scale;

    object = owner;
}

Transform::Transform(Object *owner, const Vector2 pos, const float rot, const Vector2 scale) : Component(owner) {
    setGlobalPosition(pos);
    setGlobalRotation(rot);
    setGlobalScale(scale);

    object = owner;
}

Transform::~Transform() {
    deleteAllChildren();
    setParent(nullptr);
}

Matrix<4, 4> Transform::localToWorldMatrix() const {
    Matrix<4, 4> transformationMatrix = Matrix<4, 4>::identity();

    transformationMatrix = transformationMatrix.translate(localPosition.x, localPosition.y, 0);
    transformationMatrix = transformationMatrix.rotateZ(localRotation);
    transformationMatrix = transformationMatrix.scaleAnisotropic(localScale.x, localScale.y, 1);

    if (parent_ != nullptr) {
        return parent_->localToWorldMatrix().multiply(transformationMatrix);
    }

    return transformationMatrix;
}

Matrix<4, 4> Transform::worldToLocalMatrix() const {
    const Matrix<4, 4> localToWorld = localToWorldMatrix();
    Matrix<4, 4> inverse = localToWorld.inverse();
    return inverse;
}

void Transform::setGlobalPosition(const Vector2 pos) {
    if (parent_ != nullptr) {
        const Vector2 localPos = parent_->worldToLocalMatrix() * pos;
        localPosition = localPos;
    }
    else {
        localPosition = pos;
    }
}

void Transform::setGlobalRotation(const float rot) {
    if (parent_ != nullptr) {
        const float parentGlobalRotation = parent_->getGlobalRotation();
        const float newGlobalRotation = rot - parentGlobalRotation;
        localRotation = newGlobalRotation;
    }
    else {
        localRotation = rot;
    }
}

void Transform::setGlobalScale(const Vector2 scale) {
    // TODO: Figure out wth chatgpt did in this function because i cant understand any of it
    if (!parent_) {
        // Always store positive magnitudes (no mirroring)
        localScale = {std::abs(scale.x), std::abs(scale.y)};
        return;
    }

    // 1) Parent linear (2x2) columns in column-major storage
    const Matrix<4, 4> pm = parent_->localToWorldMatrix();
    const Vector2 pCol0{pm.data[0][0], pm.data[0][1]}; // first column
    const Vector2 pCol1{pm.data[1][0], pm.data[1][1]}; // second column

    // 2) Child local rotation matrix components
    const float deg2rad = static_cast<float>(M_PI) / 180.0f;
    const float c = std::cos(localRotation * deg2rad);
    const float s = std::sin(localRotation * deg2rad);

    // Columns of Lp * Rc:
    // u = Lp * [c, s], v = Lp * [-s, c]  (linear combo of columns)
    const Vector2 u = pCol0 * c + pCol1 * s;
    const Vector2 v = pCol0 * (-s) + pCol1 * c;

    // 3) Child global rotation (independent of scale magnitude)
    const float Rg = getGlobalRotation() * deg2rad;
    const float cg = std::cos(Rg);
    const float sg = std::sin(Rg);

    // 4) Denominators that your getter uses implicitly
    float denomX = u.x * cg + u.y * sg; // = dot(u, x̂_global)
    float denomY = -v.x * sg + v.y * cg; // = dot(v, ŷ_global)

    // 5) Avoid division blow-ups
    const float eps = 1e-8f;
    if (std::fabs(denomX) < eps)
        denomX = (denomX < 0 ? -eps : eps);
    if (std::fabs(denomY) < eps)
        denomY = (denomY < 0 ? -eps : eps);

    // 6) Solve for local scale, store as positive magnitudes (no negative scale)
    float lx = scale.x / denomX;
    float ly = scale.y / denomY;

    localScale = {std::fabs(lx), std::fabs(ly)};
}

Vector2 Transform::getGlobalPosition() const {
    return decompose2D(localToWorldMatrix()).position;
}

float Transform::getGlobalRotation() const {
    if (parent_ != nullptr) {
        return fmod(parent_->getGlobalRotation() + localRotation + 360.0f, 360.0f);
    }
    return fmod(localRotation + 360.0f, 360.0f);
}

Vector2 Transform::getGlobalScale() const {
    const Matrix<4, 4> m = localToWorldMatrix();

    // get global rotation and then turn it into radians
    const float rotationRadians = getGlobalRotation() * (static_cast<float>(M_PI) / 180.0f);
    const float cosR = std::cos(rotationRadians);
    const float sinR = std::sin(rotationRadians);

    // undo rotation of columns to extract scale
    const float sx = m.data[0][0] * cosR + m.data[0][1] * sinR;
    const float sy = -m.data[1][0] * sinR + m.data[1][1] * cosR;

    return {sx, sy};
}

/**
 * @brief Simply adds child to list of children
 * @note Will not set childs parent, you must do that yourself
 * @param child Child* to add to children list
 */
void Transform::addChild(Transform *child) {
    children_.push_back(child);
}

/**
 * @brief Simply removes child from list of children
 * @note Will not set childs parent to nothing, you must do that yourself
 * @param child Child* to remove from children list
 */
void Transform::removeChild(Transform *child) {
    std::erase(children_, child);
}

/**
 * @brief Clears list of children and sets their parents to nothing
 * @note Will set child's parent to nothing
 */
void Transform::removeAllChildren() {
    for (Transform *child: children_) {
        child->setParent(nullptr);
    }

    children_.clear();
}

/**
 *
 * @return std::vector of Transform pointers. All children
 */
std::vector<Transform *> Transform::getChildren() {
    return children_;
}

void Transform::deleteAllChildren() {
    for (const Transform *child: children_) {
        child->object->destroy();
    }

    children_.clear();
}

/**
 * @brief Sets parent, will keep global position/rotation/scale
 * @param newParent The new parent
 */
void Transform::setParent(Transform *newParent) {
    // tell the parents good bye
    if (newParent != nullptr)
        newParent->removeChild(this);

    const Vector2 globalPos = getGlobalPosition();
    const float globalRot = getGlobalRotation();
    const Vector2 globalScale = getGlobalScale();

    this->parent_ = newParent;

    setGlobalPosition(globalPos);
    setGlobalRotation(globalRot);
    setGlobalScale(globalScale);
}
Transform *Transform::getParent() const {
    return parent_;
}

std::unique_ptr<TweenBase> Transform::localPosTween(const Vector2 target, const double duration, const Curve& curve) {
    return std::make_unique<Tween<Vector2>>(&localPosition, localPosition, target, duration, curve);
}
std::unique_ptr<TweenBase> Transform::localPosTween(const Vector2 start, const Vector2 end, const double duration, const Curve& curve) {
    return std::make_unique<Tween<Vector2>>(&localPosition, start, end, duration, curve);
}

std::unique_ptr<TweenBase> Transform::localRotationTween(const float target, const double duration, const Curve& curve) {
    return std::make_unique<Tween<float>>(&localRotation, localRotation, target, duration, curve);
}
std::unique_ptr<TweenBase> Transform::localRotationTween(const float start, const float end, const double duration, const Curve& curve) {
    return std::make_unique<Tween<float>>(&localRotation, start, end, duration, curve);
}

std::unique_ptr<TweenBase> Transform::localScaleTween(const Vector2 target, const double duration, const Curve& curve) {
    return std::make_unique<Tween<Vector2>>(&localScale, localScale, target, duration, curve);
}
std::unique_ptr<TweenBase> Transform::localScaleTween(const Vector2 start, const Vector2 end, const double duration, const Curve& curve) {
    return std::make_unique<Tween<Vector2>>(&localScale, start, end, duration, curve);
}

std::unique_ptr<TweenBase> Transform::globalPosTween(const Vector2 target, const double duration, const Curve& curve) {
    return std::make_unique<FunctionalTween<Vector2>>([this](const Vector2& pos){setGlobalPosition(pos);}, getGlobalPosition(), target, duration, curve);
}
std::unique_ptr<TweenBase> Transform::globalPosTween(const Vector2 start, const Vector2 end, const double duration, const Curve& curve) {
    return std::make_unique<FunctionalTween<Vector2>>([this](const Vector2& pos){setGlobalPosition(pos);}, start, end, duration, curve);
}

std::unique_ptr<TweenBase> Transform::globalRotationTween(const float target, const double duration, const Curve& curve) {
    return std::make_unique<FunctionalTween<float>>([this](const float rot){setGlobalRotation(rot);}, getGlobalRotation(), target, duration, curve);
}
std::unique_ptr<TweenBase> Transform::globalRotationTween(const float start, const float end, const double duration, const Curve& curve) {
    return std::make_unique<FunctionalTween<float>>([this](const float rot){setGlobalRotation(rot);}, start, end, duration, curve);
}

std::unique_ptr<TweenBase> Transform::globalScaleTween(const Vector2 target, const double duration, const Curve& curve) {
    return std::make_unique<FunctionalTween<Vector2>>([this](const Vector2& scale){setGlobalScale(scale);}, getGlobalScale(), target, duration, curve);
}
std::unique_ptr<TweenBase> Transform::globalScaleTween(const Vector2 start, const Vector2 end, const double duration, const Curve& curve) {
    return std::make_unique<FunctionalTween<Vector2>>([this](const Vector2& scale){setGlobalScale(scale);}, start, end, duration, curve);
}