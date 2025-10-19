#include "audioEffect.h"

void Distortion::applyEffect(const ALuint effectId) const {
    alEffecti(effectId, AL_EFFECT_TYPE, getEffectType());

    alEffectf(effectId, AL_DISTORTION_EDGE, edge);
    alEffectf(effectId, AL_DISTORTION_GAIN, gain);
    alEffectf(effectId, AL_DISTORTION_LOWPASS_CUTOFF, lowpassCutoff);
    alEffectf(effectId, AL_DISTORTION_EQCENTER, eqCenter);
    alEffectf(effectId, AL_DISTORTION_EQBANDWIDTH, eqBandwidth);
}
