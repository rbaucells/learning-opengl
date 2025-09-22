#pragma once

#include <al.h>
#include <alc.h>
#include "../../math/vector2.h"
#include "../component.h"

class Object;
struct Vector3;

class AudioListener final : public Component {
public:
    static AudioListener* mainListener;

    explicit AudioListener(Object* owner);

    void update(float deltaTime) override;

    void setDistanceModel(ALenum model);
    void setVolume(float volume);
    void setListenerOrientation(const Vector3& forward, const Vector3& up);

    ~AudioListener() override;

private:
    Vector2 lastPos_;
    ALCdevice* device_;
    ALCcontext* context_;

    void queryErrors() const;

    void setListenerPos() const;
    void setListenerVelocity(float deltaTime);
};
