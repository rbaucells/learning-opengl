#include "audioEffect.h"

void Compressor::applyEffect(const ALuint effectId) const {
    alEffecti(effectId, AL_EFFECT_TYPE, getEffectType());

    alEffecti(effectId, AL_COMPRESSOR_ONOFF, onOff);
}
