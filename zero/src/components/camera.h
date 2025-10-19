#pragma once
#include "math++/math.h"
#include "../systems/component.h"

class Camera final : public Component {
public:
    struct AspectRatio {
        float width, height;

        static AspectRatio sixteenByNine;
        static AspectRatio oneByOne;
        static AspectRatio twentyOneByNine;
        static AspectRatio fourByThree;
    };

    explicit Camera(Object* owner, bool main = true);
    void awake() override;
    void setMain();
    Matrix<4, 4> getViewMatrix();

    float height = 5;
    AspectRatio aspectRatio = AspectRatio::sixteenByNine;

    static Camera* mainCamera;

    ~Camera() override;

private:
    Matrix<4, 4> lastLocalToWorldMatrix_;

    Matrix<4, 4> viewMatrix_;

    [[nodiscard]] Matrix<4, 4> calculateCameraLocalToWorld() const;
};
