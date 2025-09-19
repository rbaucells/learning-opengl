#pragma once
#define AL_ALEXT_PROTOTYPES
#include <AL/efx.h>

#include <string>
#include "../component.h"
#include "../../math/vector2.h"
#include "AL/al.h"
#include "AL/alc.h"

struct Vector3;

struct Sound {
    struct WavHeader {
        std::uint8_t channels;
        std::int32_t sampleRate;
        std::uint8_t bitsPerSample;
        ALsizei size;
    };

    explicit Sound(const std::string& filePath);

    [[nodiscard]] std::uint8_t getHeaderChannels() const;
    [[nodiscard]] std::int32_t getHeaderSampleRate() const;
    [[nodiscard]] std::uint8_t getHeaderBitsPerSample() const;
    [[nodiscard]] ALsizei getHeaderSize() const;

    [[nodiscard]] const char* getData() const;

private:
    WavHeader header_ = {};

    char* data_ = nullptr;

    static std::int32_t reinterpret4BytesAsInt(const char* bytes, std::size_t lenght);
    static WavHeader loadWavFileHeader(std::ifstream& file);
};

class AudioEffect {
public:
    ALuint getEffect() const {
        return effect_;
    }

    virtual ~AudioEffect() {
        if (effect_ != 0) {
            alDeleteEffects(1, &effect_);
        }
    }

protected:
    explicit AudioEffect(const ALenum effectType) {
        alGenEffects(1, &effect_);
        alEffecti(effect_, AL_EFFECT_TYPE, effectType);
    }

    ALuint effect_;
};

class FrequencyShifterEffect final : public AudioEffect {
public:
    enum Direction {
        down,
        up,
        off
    };
    FrequencyShifterEffect() : AudioEffect(AL_EFFECT_FREQUENCY_SHIFTER) {}

    void setFrequency(const float frequencyHz) const {
        alEffectf(effect_, AL_FREQUENCY_SHIFTER_FREQUENCY, frequencyHz);
    }

    void setLeftDirection(const Direction direction) const {
        alEffecti(effect_, AL_FREQUENCY_SHIFTER_LEFT_DIRECTION, direction);
    }

    void setRightDirection(const Direction direction) const {
        alEffecti(effect_, AL_FREQUENCY_SHIFTER_RIGHT_DIRECTION, direction);
    }
};


class AudioSource final : public Component {
public:
    AudioSource(Object* owner, const std::string& filePath);

    void play() const;
    void stop() const;
    void rewind() const;
    void pause() const;

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

    float getVolume() const {
        return volume_;
    }

    float getPitch() const {
        return pitch_;
    }

    void setLooping(const bool looping) {
        looping_ = looping;
    }

    bool getLooping() const {
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

private:
    ALuint source_ = 0;
    ALuint buffer_ = 0;

    Sound sound_;
};

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
