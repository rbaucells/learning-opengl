#include <fstream>
#include <iostream>

#include "audio.h"

Sound::Sound(const std::string& filePath) {
    std::ifstream in(filePath);

    if(!in.is_open())
    {
        std::cerr << "ERROR: Could not open \"" << filePath << "\"" << std::endl;
        return;
    }
    header = loadWavFileHeader(in);
    if(!header.size)
    {
        std::cerr << "ERROR: Could not load wav header of \"" << filePath << "\"" << std::endl;
        return;
    }

    data = new char[header.size];
    in.read(data, header.size);
}

std::int32_t Sound::reinterpret4BytesAsInt(const char* bytes, const std::size_t lenght) {
    std::int32_t result = 0;
    // if its a little endian system, we dont do much
    if constexpr (std::endian::native == std::endian::little) {
        // we just rewrite the int32's memory as the bytes
        std::memcpy(&result, bytes, lenght);
    }
    else if constexpr (std::endian::native == std::endian::big) {
        // we need to swap [byte1, byte2, byte3, byte4] -> [byte4, byte3, byte2, byte1]

        // to do that we reinterpret the opposite byte in the result as the byte in bytes
        char* memOfResult = reinterpret_cast<char*>(&result);
        for (std::size_t i = 0; i < lenght; i++) {
            // ^ for loop goes from 0 to lenght
            // memOfResults is written in order of lenght to 0 (backwards)
            // bytes is red fro 0 to lenght
            memOfResult[3 - i] = bytes[i];
        }
    }

    return result;
}

Sound::WavHeader Sound::loadWavFileHeader(std::ifstream& file) {
    char buffer[4];
    if (!file.is_open())
        return {};

    // the RIFF
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read RIFF" << std::endl;
        return {};
    }
    if (std::strncmp(buffer, "RIFF", 4) != 0) {
        std::cerr << "ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)" << std::endl;
        return {};
    }

    // the size of the file
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read size of file" << std::endl;
        return {};
    }

    // the WAVE
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read WAVE" << std::endl;
        return {};
    }
    if (std::strncmp(buffer, "WAVE", 4) != 0) {
        std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
        return {};
    }

    // "fmt/0"
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read fmt/0" << std::endl;
        return {};
    }

    // this is always 16, the size of the fmt data chunk
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read the 16" << std::endl;
        return {};
    }

    // PCM should be 1?
    if (!file.read(buffer, 2)) {
        std::cerr << "ERROR: could not read PCM" << std::endl;
        return {};
    }

    // the number of channels
    if (!file.read(buffer, 2)) {
        std::cerr << "ERROR: could not read number of channels" << std::endl;
        return {};
    }
    std::uint8_t channels = reinterpret4BytesAsInt(buffer, 2);

    // sample rate
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read sample rate" << std::endl;
        return {};
    }
    std::int32_t sampleRate = reinterpret4BytesAsInt(buffer, 4);

    // (sampleRate * bitsPerSample * channels) / 8
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
        return {};
    }

    // ?? dafaq
    if (!file.read(buffer, 2)) {
        std::cerr << "ERROR: could not read dafaq" << std::endl;
        return {};
    }

    // bitsPerSample
    if (!file.read(buffer, 2)) {
        std::cerr << "ERROR: could not read bits per sample" << std::endl;
        return {};
    }
    std::uint8_t bitsPerSample = reinterpret4BytesAsInt(buffer, 2);

    // data chunk header "data"
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read data chunk header" << std::endl;
        return {};
    }
    if (std::strncmp(buffer, "data", 4) != 0) {
        std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
        return {};
    }

    // size of data
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read data size" << std::endl;
        return {};
    }
    ALsizei size = reinterpret4BytesAsInt(buffer, 4);

    /* cannot be at the end of file */
    if (file.eof()) {
        std::cerr << "ERROR: reached EOF on the file" << std::endl;
        return {};
    }
    if (file.fail()) {
        std::cerr << "ERROR: fail state set on the file" << std::endl;
        return {};
    }

    return {channels, sampleRate, bitsPerSample, size};
}
