#include "audioSource.h"

#include <iostream>

#include "../../object.h"
#include <AL/al.h>

#include "effects/audioEffect.h"
#include "filters/audioFilter.h"

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

    if (state == AL_PLAYING) {
        return true;
    }

    return false;
}

void AudioSource::update(const float deltaTime) {
    if (isPlaying()) {
        setSourcePos();
        setSourceVelocity(deltaTime);
    }
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

void AudioSource::setSourceVelocity(const float deltaTime) {
    const Vector2 globalPos = object->transform.getGlobalPosition();
    const Vector2 velocity = (globalPos - lastPos_) / deltaTime;
    lastPos_ = globalPos;

    alSource3f(source_, AL_VELOCITY, velocity.x, velocity.y, 0);
}

void AudioSource::addEffectAndFilter(const AudioEffect* audioEffect, const AudioFilter* audioFilter) {
    assert(audioEffect);
    // make an effect
    ALuint effectId;
    alGenEffects(1, &effectId);
    // and apply the parameters
    audioEffect->applyEffect(effectId);

    if (audioFilter) { // if the filter exists we define it like normal
        // make a filter
        ALuint filterId;
        alGenFilters(1, &filterId);
        // and apply the parameters
        audioFilter->applyFilter(filterId);

        // make a slot for the effect and filter
        ALuint auxEffectId;
        alGenAuxiliaryEffectSlots(1, &auxEffectId);
        // bind the effect
        alAuxiliaryEffectSloti(auxEffectId, AL_EFFECTSLOT_EFFECT, effectId);
        // bind the filter
        alSource3i(source_, AL_AUXILIARY_SEND_FILTER, auxEffectId, 0, filterId);

        effectsAndFilters_.push_back({effectId, filterId, auxEffectId});
    }
    else { // the filter is nullptr so we just pass AL_FILTER_NULL
        // make a slot for the effect and filter
        ALuint auxEffectId;
        alGenAuxiliaryEffectSlots(1, &auxEffectId);
        // bind the effect
        alAuxiliaryEffectSloti(auxEffectId, AL_EFFECTSLOT_EFFECT, effectId);
        // bind the filter
        alSource3i(source_, AL_AUXILIARY_SEND_FILTER, auxEffectId, 0, AL_FILTER_NULL);

        effectsAndFilters_.push_back({effectId, AL_FILTER_NULL, auxEffectId});
    }
}

void AudioSource::setDirectFilter(const AudioFilter* audioFilter) {
    if (!audioFilter) {
        alSourcei(source_, AL_DIRECT_FILTER, AL_FILTER_NULL);
        return;
    }

    if (!directFilter_)
        alGenFilters(1, &directFilter_);

    audioFilter->applyFilter(directFilter_);
    alSourcei(source_, AL_DIRECT_FILTER, directFilter_);
}
