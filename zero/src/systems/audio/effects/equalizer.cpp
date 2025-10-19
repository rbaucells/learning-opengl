#include "audioEffect.h"

void Equalizer::applyEffect(const ALuint effectId) const {
    alEffecti(effectId, AL_EFFECT_TYPE, getEffectType());

    alEffectf(effectId, AL_EQUALIZER_LOW_GAIN, lowGain);
    alEffectf(effectId, AL_EQUALIZER_LOW_CUTOFF, lowCutoff);

    alEffectf(effectId, AL_EQUALIZER_MID1_GAIN, mid1Gain);
    alEffectf(effectId, AL_EQUALIZER_MID1_CENTER, mid1Center);
    alEffectf(effectId, AL_EQUALIZER_MID1_WIDTH, mid1Width);

    alEffectf(effectId, AL_EQUALIZER_MID2_GAIN, mid2Gain);
    alEffectf(effectId, AL_EQUALIZER_MID2_CENTER, mid2Center);
    alEffectf(effectId, AL_EQUALIZER_MID2_WIDTH, mid2Width);

    alEffectf(effectId, AL_EQUALIZER_HIGH_GAIN, highGain);
    alEffectf(effectId, AL_EQUALIZER_HIGH_CUTOFF, highCutoff);
}
