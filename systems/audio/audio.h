#pragma once
#include <OpenAL.h>
#include <string>

struct Sound {
public:
    struct WavHeader {
        std::uint8_t channels;
        std::int32_t sampleRate;
        std::uint8_t bitsPerSample;
        ALsizei size;
    };

    Sound(const std::string& filePath);

    WavHeader header = {};

    char* data = nullptr;

    static std::int32_t reinterpret4BytesAsInt(const char* bytes, std::size_t lenght);
    static WavHeader loadWavFileHeader(std::ifstream& file);
};

class AudioDevice {
public:
    static AudioDevice* audioDevice;

    AudioDevice();

    void queryErrors() const;

    bool getPlaying();

    void playSound(const Sound& sound);

    ~AudioDevice();

private:
    ALCdevice* device_;
    ALCcontext* context_;

    ALuint buffer_ = 0;
    ALuint source_ = 0;

    ALint state_{};
};
