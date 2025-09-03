#include "transform.h"

#include "logging.h"

struct Decomposed2D {
    Vector2 position{};
    float rotation{}; // degrees
    Vector2 scale{};
};

Decomposed2D decompose2D(const ColumnMatrix4X4 &m) {
    Logging l("decompose2D");
    // TODO: what did chatgpt do in here?
    Decomposed2D out;

    // Translation
    out.position = {m.data[3][0], m.data[3][1]};

    // Raw basis vectors
    Vector2 col0 = {m.data[0][0], m.data[0][1]};
    Vector2 col1 = {m.data[1][0], m.data[1][1]};

    // Scale from raw lengths
    float scaleX = col0.Magnitude();
    float scaleY = col1.Magnitude();

    // Normalize basis vectors
    if (scaleX != 0)
        col0 /= scaleX;
    if (scaleY != 0)
        col1 /= scaleY;

    // Remove shear: make col1 perpendicular to col0
    float shear = col0.Dot(col1);
    col1 -= col0 * shear;

    // Recompute scaleY after removing shear
    scaleY = col1.Magnitude();
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

Transform::Transform(const Vector2 pos, const float rot, const Vector2 scale) {
    setGlobalPosition(pos);
    setGlobalRotation(rot);
    setGlobalScale(scale);
}

Transform::Transform(Vector2 pos, float rot, Vector2 scale, Transform *parent) {
    setParent(parent);
    this->localPosition = pos;
    this->localRotation = rot;
    this->localScale = scale;
}

Vector2 Transform::getGlobalPosition() const {
    return decompose2D(localToWorldMatrix()).position;
}

float Transform::getGlobalRotation() const {
    if (parent != nullptr) {
        return fmod(parent->getGlobalRotation() + localRotation + 360.0f, 360.0f);
    }
    return fmod(localRotation + 360.0f, 360.0f);
}

Vector2 Transform::getGlobalScale() const {
    const ColumnMatrix4X4 m = localToWorldMatrix();

    // get global rotation and then turn it into radians
    const float rotationRadians = getGlobalRotation() * (static_cast<float>(M_PI) / 180.0f);
    const float cosR = std::cos(rotationRadians);
    const float sinR = std::sin(rotationRadians);

    // undo rotation of columns to extract scale
    const float sx = m.data[0][0] * cosR + m.data[0][1] * sinR;
    const float sy = -m.data[1][0] * sinR + m.data[1][1] * cosR;

    return {sx, sy};
}

void Transform::setGlobalPosition(const Vector2 pos) {
    if (parent != nullptr) {
        const Vector2 localPos = parent->worldToLocalMatrix() * pos;
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

void Transform::setGlobalScale(const Vector2 desiredGlobalScale) {
    // TODO: Figure out wth chatgpt did in this function because i cant understand any of it
    if (!parent) {
        // Always store positive magnitudes (no mirroring)
        localScale = {std::abs(desiredGlobalScale.x), std::abs(desiredGlobalScale.y)};
        return;
    }

    // 1) Parent linear (2x2) columns in column-major storage
    const ColumnMatrix4X4 pm = parent->localToWorldMatrix();
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
    float lx = desiredGlobalScale.x / denomX;
    float ly = desiredGlobalScale.y / denomY;

    localScale = {std::fabs(lx), std::fabs(ly)};
}

ColumnMatrix4X4 Transform::localToWorldMatrix() const {
    Logging l("localToWorldMatrix");
    ColumnMatrix4X4 transformationMatrix = ColumnMatrix4X4::identity();
    // error: the first thing to use transformationMatrix (no matter what it is) gives 11:SIGSEGV which i think is a memory access violation

    std::cout << "transformationMatrix: \n" << transformationMatrix.toString();

    std::cout << "Applying translation \n";
    transformationMatrix = transformationMatrix.translate(localPosition.x, localPosition.y, 0);
    std::cout << "Applying rotation \n";
    transformationMatrix = transformationMatrix.rotate_z(localRotation);
    std::cout << "Applying scale \n";
    transformationMatrix = transformationMatrix.scale_anisotropic(localScale.x, localScale.y, 1);

    if (parent != nullptr) {
        std::cout << "Parent != nullptr \n";
        const auto parentLocalToWorld = parent->localToWorldMatrix();
        std::cout << "Got parent local to world matrix \n";
        return parentLocalToWorld.multiply(transformationMatrix);
    }

    std::cout << "returning transformationMatrix \n";
    return transformationMatrix;
}

ColumnMatrix4X4 Transform::worldToLocalMatrix() const {
    ColumnMatrix4X4 localToWorld = localToWorldMatrix();
    ColumnMatrix4X4 inverse = localToWorld.inverse();
    return inverse;
}

const Transform *Transform::getParent() const {
    return parent;
}

void Transform::setParent(Transform *newParent) {
    // were getting a new valid parent, let our old parent know we are moving out
    assert(newParent != nullptr);

    if (newParent != parent && parent != nullptr) {
        parent->removeChild(this);
    }

    auto globalPos = getGlobalPosition();
    auto globalRot = getGlobalRotation();
    auto globalScale = getGlobalScale();

    // change parents
    parent = newParent;
    // tell the new parent we are now theirs
    parent->addChild(this);

    // make sure we stay in the same position globaly
    setGlobalPosition(globalPos);
    setGlobalRotation(globalRot);
    setGlobalScale(globalScale);
}

void Transform::unParent() {
    Logging l("unParent");
    std::cout << "Getting global pos \n";
    Vector2 globalPos = getGlobalPosition();
    std::cout << "got global pos \n";
    std::cout << "Getting global rot \n";
    float globalRot = getGlobalRotation();
    std::cout << "got global rot \n";
    std::cout << "Getting global scale \n";
    Vector2 globalScale = getGlobalScale();
    std::cout << "got global scale \n";

    std::cout << "setting parent to nullptr \n";
    parent = nullptr;

    std::cout << "setting global pos \n";
    setGlobalPosition(globalPos);
    std::cout << "setting global rot \n";
    setGlobalRotation(globalRot);
    std::cout << "setting global scale \n";
    setGlobalScale(globalScale);
}

void Transform::addChild(Transform *child) {
    assert(child != nullptr);

    children.push_back(child);
}

std::vector<Transform *> Transform::getChildren() {
    return children;
}

void Transform::removeChild(Transform *child) {
    assert(child != nullptr);
    std::erase(children, child);
}

void Transform::removeAllChildren() {
    Logging l("removeAllChildren");
    for (Transform *child: children) {
        child->unParent();
    }

    children.clear();
}
