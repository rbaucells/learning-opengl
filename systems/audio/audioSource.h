#pragma once
#include <al.h>
#include <string>

#include "sound.h"

#include "../component.h"
#include "../../math/vector2.h"

struct AudioFilter;
struct AudioEffect;
class Object;

class AudioSource final : public Component {
public:
    AudioSource(Object* owner, const std::string& filePath);

    void play() const;
    void stop() const;
    void rewind() const;
    void pause() const;

    void addEffectAndFilter(const AudioEffect* audioEffect, const AudioFilter* audioFilter = nullptr);
    void setDirectFilter(const AudioFilter* audioFilter);

    /**
     * @brief Sets the volume of this audio source
     * @param volume [0, 1]
     */
    void setVolume(const float volume) {
        volume_ = volume;
    }

    /**
     * @brief Sets the pitch of this audio source
     * @param pitch (0, any]
     */
    void setPitch(const float pitch) {
        pitch_ = pitch;
    }

    [[nodiscard]] float getVolume() const {
        return volume_;
    }

    [[nodiscard]] float getPitch() const {
        return pitch_;
    }

    void setLooping(const bool looping) {
        looping_ = looping;
    }

    [[nodiscard]] bool getLooping() const {
        return looping_;
    }

    void setPlaybackTime(float time) const;

    [[nodiscard]] bool isPlaying() const;

    void update(float deltaTime) override;

    ~AudioSource() override;

private:
    float volume_ = 1;
    float pitch_ = 1;

    bool looping_ = AL_FALSE;

    void setSourcePos() const;
    void setSourceVelocity(float deltaTime);

    Vector2 lastPos_;

    ALuint source_ = 0;
    ALuint buffer_ = 0;

    Sound sound_;

    struct EffectAndFilter {
        ALuint effectId;
        ALuint filterId;
        ALuint auxEffectId;
    };

    std::vector<EffectAndFilter> effectsAndFilters_ = {};
    ALuint directFilter_ = 0;
};
