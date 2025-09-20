#include "audioEffect.h"

void VocalMorpher::applyEffect(const ALuint effectId) const {
    alEffecti(effectId, AL_EFFECT_TYPE, getEffectType());

    alEffecti(effectId, AL_VOCAL_MORPHER_PHONEME_A, phonemeA);
    alEffecti(effectId, AL_VOCAL_MORPHER_PHONEME_B, phonemeB);
    alEffecti(effectId, AL_VOCAL_MORPHER_PHONEMEA_COARSE_TUNING, phonemeACoarseTuning);
    alEffecti(effectId, AL_VOCAL_MORPHER_PHONEMEB_COARSE_TUNING, phonemeBCoarseTuning);
    alEffecti(effectId, AL_VOCAL_MORPHER_WAVEFORM, waveForm);
    alEffectf(effectId, AL_VOCAL_MORPHER_RATE, rate);
}
