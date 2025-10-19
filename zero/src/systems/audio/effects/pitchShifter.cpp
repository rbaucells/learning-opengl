#include "audioEffect.h"

void PitchShifter::applyEffect(const ALuint effectId) const {
    alEffecti(effectId, AL_EFFECT_TYPE, getEffectType());

    alEffecti(effectId, AL_PITCH_SHIFTER_COARSE_TUNE, coarseTune);
    alEffecti(effectId, AL_PITCH_SHIFTER_FINE_TUNE, fineTune);
}
