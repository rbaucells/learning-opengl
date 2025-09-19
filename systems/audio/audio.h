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

struct AudioEffect {
    virtual void applyEffect(ALuint effectId) const = 0;

    [[nodiscard]] constexpr virtual ALenum getEffectType() const = 0;

    virtual ~AudioEffect() = default;
};

struct FrequencyShifterEffect final : public AudioEffect {
    enum Direction {
        down = AL_FREQUENCY_SHIFTER_DIRECTION_DOWN,
        up = AL_FREQUENCY_SHIFTER_DIRECTION_UP,
        off = AL_FREQUENCY_SHIFTER_DIRECTION_OFF
    };

    void applyEffect(ALuint effectId) const override;

    [[nodiscard]] constexpr ALenum getEffectType() const override {
        return AL_EFFECT_FREQUENCY_SHIFTER;
    }

    // default params
    float frequencyHz = 0;
    Direction leftDirection = down;
    Direction rightDirection = down;
};

struct AudioFilter {
    virtual void applyFilter(ALuint filterId) const = 0;

    [[nodiscard]] constexpr virtual ALenum getFilterType() const = 0;

    virtual ~AudioFilter() = default;
};

struct LowPassFilter final : public AudioFilter {
    void applyFilter(ALuint filterId) const override;

    [[nodiscard]] constexpr ALenum getFilterType() const override {
        return AL_FILTER_LOWPASS;
    }

    float gain = 1;
    float gainHf = 1;
};

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

    ALuint source_ = 0;
    ALuint buffer_ = 0;

    Sound sound_;

    struct EffectAndFilter {
        ALuint effectId;
        ALuint filterId;
        ALuint auxEffectId;
    };

    std::vector<EffectAndFilter> effectsAndFilters_;
    ALuint directFilter_;
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
