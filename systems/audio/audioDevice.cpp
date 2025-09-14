#include "audio.h"

#include <iostream>

AudioDevice::AudioDevice() {
    // Initialize Open AL
    device_ = alcOpenDevice(nullptr); // open default device

    if (device_ == nullptr)
        return;

    context_ = alcCreateContext(device_, nullptr);

    if (context_ == nullptr)
        return;

    if (!alcMakeContextCurrent(context_)) {
        std::cerr << "Making context current failed \n";
    }

    // if (!audioDevice)
        // audioDevice = this;
}

/**
 * To be called in update loop
 */
void AudioDevice::queryErrors() const {
    switch (auto errorCode = alGetError()) {
        case AL_NO_ERROR:
            break;
        case AL_INVALID_NAME:
            std::cerr << "Invalid name parameter. \n";
            break;
        case AL_INVALID_ENUM:
            std::cerr << "Invalid parameter. \n";
            break;
        case AL_INVALID_VALUE:
            std::cerr << "Invalid enum parameter value. \n";
            break;
        case AL_INVALID_OPERATION:
            std::cerr << "Illegal call. \n";
            break;
        case AL_OUT_OF_MEMORY:
            std::cerr << "Unable to allocate memory. \n";
            break;
        default:
            std::cerr << "UNKNOWN ALC ERROR: " << errorCode;
            break;
    }

    switch (ALCenum errorCode = alcGetError(device_)) {
        case AL_NO_ERROR:
            break;
        case AL_INVALID_NAME:
            std::cerr << "Invalid name parameter. \n";
            break;
        case AL_INVALID_ENUM:
            std::cerr << "Invalid parameter. \n";
            break;
        case AL_INVALID_VALUE:
            std::cerr << "Invalid enum parameter value. \n";
            break;
        case AL_INVALID_OPERATION:
            std::cerr << "Illegal call. \n";
            break;
        case AL_OUT_OF_MEMORY:
            std::cerr << "Unable to allocate memory. \n";
            break;
        default:
            std::cerr << "UNKNOWN ALC ERROR: " << errorCode;
            break;
    }
}

bool AudioDevice::getPlaying() {
    alGetSourcei(source_, AL_SOURCE_STATE, &state_);
    return state_;
}

void AudioDevice::playSound(const Sound& sound) {
    alGenBuffers(1, &buffer_);

    ALenum format = 0;

    if (sound.header.channels == 1 && sound.header.bitsPerSample == 8)
        format = AL_FORMAT_MONO8;
    else if (sound.header.channels == 1 && sound.header.bitsPerSample == 16)
        format = AL_FORMAT_MONO16;
    else if (sound.header.channels == 2 && sound.header.bitsPerSample == 8)
        format = AL_FORMAT_STEREO8;
    else if (sound.header.channels == 2 && sound.header.bitsPerSample == 16)
        format = AL_FORMAT_STEREO16;
    else {
        std::cerr
                << "ERROR: unrecognised wave format: "
                << sound.header.channels << " channels, "
                << sound.header.bitsPerSample << " bps" << std::endl;
        return;
    }

    alBufferData(buffer_, format, sound.data, sound.header.size, sound.header.sampleRate);

    alGenSources(1, &source_);
    alSourcef(source_, AL_PITCH, 1);
    alSourcef(source_, AL_GAIN, 1.0f);
    alSourcei(source_, AL_LOOPING, AL_FALSE);
    alSourcei(source_, AL_BUFFER, buffer_);

    alSourcePlay(source_);
}

AudioDevice::~AudioDevice() {
    alDeleteSources(1, &source_);
    alDeleteBuffers(1, &buffer_);

    if (!alcMakeContextCurrent(nullptr)) {
        std::cerr << "Setting context current failed \n";
    }
    alcDestroyContext(context_);
    if (!alcCloseDevice(device_)) {
        std::cerr << "Closing device failed \n";
    }
}
