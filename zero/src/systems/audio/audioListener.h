#pragma once

#include <al.h>
#include <alc.h>
#include "math++/math.h"
#include "../component.h"

class AudioListener final : public Component {
public:
    static AudioListener* mainListener;

    explicit AudioListener(const ComponentParams& params);

    void update(float deltaTime) override;

    void setDistanceModel(ALenum model);
    void setVolume(float volume);
    void setListenerOrientation(const Vector<3>& forward, const Vector<3>& up);

    ~AudioListener() override;

private:
    Vector<2> lastPos_;
    ALCdevice* device_;
    ALCcontext* context_;

    void queryErrors() const;

    void setListenerPos() const;
    void setListenerVelocity(float deltaTime);
};
