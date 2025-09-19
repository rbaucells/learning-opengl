#include <iostream>

#include "audio.h"
#include "../../object.h"
#include <AL/al.h>

AudioSource::AudioSource(Object* owner, const std::string& filePath) : Component(owner), sound_(filePath) {
    std::printf("Constructing AudioSource\n");
    // create source and buffer
    alGenSources(1, &source_);
    alGenBuffers(1, &buffer_);

    // read the sound for info
    const auto channels = sound_.getHeaderChannels();
    const auto bitsPerSample = sound_.getHeaderBitsPerSample();
    const auto data = sound_.getData();
    const auto size = sound_.getHeaderSize();
    const auto sampleRate = sound_.getHeaderSampleRate();

    // mono or stereo
    // 8 or 16 bit
    ALenum format = 0;
    // get format
    if (channels == 1 && bitsPerSample == 8)
        format = AL_FORMAT_MONO8;
    else if (channels == 1 && bitsPerSample == 16)
        format = AL_FORMAT_MONO16;
    else if (channels == 2 && bitsPerSample == 8)
        format = AL_FORMAT_STEREO8;
    else if (channels == 2 && bitsPerSample == 16)
        format = AL_FORMAT_STEREO16;
    else {
        std::cerr
            << "ERROR: unrecognised wave format: "
            << channels << " channels, "
            << bitsPerSample << " bps" << std::endl;
        return;
    }

    // define the data
    alBufferData(buffer_, format, data, size, sampleRate);
}

/**
 * @brief Plays the audio with any effects and pitch/volume
 */
void AudioSource::play() const {
    alSourcef(source_, AL_PITCH, pitch_);
    alSourcef(source_, AL_GAIN, volume_);
    alSourcei(source_, AL_LOOPING, looping_);

    // tell it where the info is
    alSourcei(source_, AL_BUFFER, buffer_);

    setSourcePos();

    // play
    alSourcePlay(source_);
}

void AudioSource::stop() const {
    alSourceStop(source_);
}


void AudioSource::rewind() const {
    alSourceRewind(source_);
}

void AudioSource::pause() const {
    alSourcePause(source_);
}

void AudioSource::setPlaybackTime(const float time) const {
    alSourcef(source_, AL_SEC_OFFSET, time);
}

bool AudioSource::isPlaying() const {
    ALint state;
    alGetSourcei(source_, AL_SOURCE_STATE, &state);

    if (state == AL_PLAYING)
        return true;

    return false;
}

void AudioSource::update(const float deltaTime) {
    if (isPlaying())
        setSourcePos();
}

AudioSource::~AudioSource() {
    std::printf("Destroying AudioSource\n");
    // clean up time
    alDeleteSources(1, &source_);
    alDeleteBuffers(1, &buffer_);
}

void AudioSource::setSourcePos() const {
    // for directional audio
    auto [x, y] = object->transform.getGlobalPosition();
    alSource3f(source_, AL_POSITION, x, y, 0);
}