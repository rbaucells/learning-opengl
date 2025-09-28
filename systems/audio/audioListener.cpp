#include "audioListener.h"

#include <iostream>

#include "../../object.h"

AudioListener* AudioListener::mainListener = nullptr;
/**
 * @warning PLS DONT MAKE MULTIPLE AUDIO LISTENERS
 * @param owner Owner
 */
AudioListener::AudioListener(Object* owner) : Component(owner) {
    if (!mainListener)
        mainListener = this;
    else {
        // VERY DANGEROUS, WILL DELETE THIS CLASS INSTANCE WHILE ITS RUNNING, PLS DONT MAKE MULTIPLE AUDIOLISTENERS
        object->removeComponent<AudioListener>();
        return;
    }

    if (ALCdevice* device = alcOpenDevice(nullptr); !device) {
        std::cerr << "Error opening alc device\n";
    }
    else {
        device_ = device;
    }

    if (!alcIsExtensionPresent(device_, "ALC_EXT_EFX")) {
        std::cerr << "EFX not supported by this device\n";
    }

    if (ALCcontext* context = alcCreateContext(device_, nullptr); context == nullptr)
        std::cerr << "Error creating al context\n";
    else {
        context_ = context;
    }

    if (!alcMakeContextCurrent(context_)) {
        std::cerr << "Making context current failed \n";
    }
}

void AudioListener::update(const float deltaTime) {
    setListenerPos();
    setListenerVelocity(deltaTime);
    queryErrors();
}

void AudioListener::setDistanceModel(const ALenum model) {
    alDistanceModel(model);
}

void AudioListener::setVolume(const float volume) {
    alListenerf(AL_GAIN, volume);
}

void AudioListener::setListenerOrientation(const Vector3& forward, const Vector3& up) {
    const float vectorTuple[6] = {
        forward.x, forward.y, forward.z,
        up.x, up.y, up.z
    };

    // this copies the data so vectorTuple is safe to delete
    alListenerfv(AL_ORIENTATION, vectorTuple);
}



AudioListener::~AudioListener() {
    std::printf("Destroyed AudioListener\n");
    if (!alcMakeContextCurrent(nullptr)) {
        std::cerr << "Setting context current failed \n";
    }

    alcDestroyContext(context_);

    if (!alcCloseDevice(device_)) {
        std::cerr << "Closing device failed \n";
    }
}

/**
 * To be called in update loop
 */
void AudioListener::queryErrors() const {
    switch (const auto errorCode = alGetError()) {
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

void AudioListener::setListenerPos() const {
    Vector2 globalPos = object->transform.getGlobalPosition();
    alListener3f(AL_POSITION, globalPos.x, globalPos.y, 0);
}

void AudioListener::setListenerVelocity(const float deltaTime) {
    const Vector2 globalPos = object->transform.getGlobalPosition();
    const Vector2 velocity = (globalPos - lastPos_) / deltaTime;
    lastPos_ = globalPos;

    alListener3f(AL_VELOCITY, velocity.x, velocity.y, 0);
}

