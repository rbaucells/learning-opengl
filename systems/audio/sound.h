#pragma once
#include <al.h>
#include <iosfwd>
#include <string>

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
