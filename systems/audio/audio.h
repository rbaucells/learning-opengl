#pragma once
#include <string>
#include "../component.h"
#include "AL/al.h"
#include "AL/alc.h"

struct Sound {
    struct WavHeader {
        std::uint8_t channels;
        std::int32_t sampleRate;
        std::uint8_t bitsPerSample;
        ALsizei size;
    };

    explicit Sound(const std::string& filePath);

    std::uint8_t getHeaderChannels() const;
    std::int32_t getHeaderSampleRate() const;
    std::uint8_t getHeaderBitsPerSample() const;
    ALsizei getHeaderSize() const;

    const char* getData() const;

private:
    WavHeader header_ = {};

    char* data_ = nullptr;

    static std::int32_t reinterpret4BytesAsInt(const char* bytes, std::size_t lenght);
    static WavHeader loadWavFileHeader(std::ifstream& file);
};

struct AudioEffect {

};

class AudioSource final : public Component {
public:
    AudioSource(Object* owner, const std::string& filePath);

    void play(const std::vector<AudioEffect>& effects = {}) const;
    bool isPlaying() const;

    ~AudioSource() override;

private:
    ALuint source_ = 0;
    ALuint buffer_ = 0;

    Sound sound_;
};

class AudioListner final : public Component {
public:
    static AudioListner* audioDevice;

    explicit AudioListner(Object* owner);

    void update(float deltaTime) override;

    ~AudioListner() override;

private:
    ALCdevice* device_;
    ALCcontext* context_;

    void queryErrors() const;
    void setListnerPos() const;
};

