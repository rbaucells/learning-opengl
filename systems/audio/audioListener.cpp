#include <iostream>
#include "audio.h"

#include "../../object.h"

AudioListner* AudioListner::audioDevice = nullptr;
/**
 * @warning PLS DONT MAKE MULTIPLE AUDIO LISTENERS
 * @param owner Owner
 */
AudioListner::AudioListner(Object* owner) : Component(owner) {
    std::printf("Creating Audio Listener\n");
    if (!audioDevice)
        audioDevice = this;
    else {
        // VERY DANGEROUS, WILL DELETE THIS CLASS INSTANCE, PLS DONT MAKE MULTIPLE AUDIOLISTENERS
        object->removeComponent<AudioListner>();
        return;
    }

    if (ALCdevice* device = alcOpenDevice(nullptr); !device) {
        std::cerr << "Error opening alc device\n";
    }
    else {
        device_ = device;
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

void AudioListner::update(const float deltaTime) {
    setListnerPos();
    queryErrors();
}

void AudioListner::setListnerPos() const {
    Vector2 globalPos = object->transform.getGlobalPosition();
    alListener3f(AL_POSITION, globalPos.x, globalPos.y, 0);
}

AudioListner::~AudioListner() {
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
void AudioListner::queryErrors() const {
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
