#include "audioEffect.h"

void FrequencyShifter::applyEffect(const ALuint effectId) const {
    alEffecti(effectId, AL_EFFECT_TYPE, getEffectType());
    alEffectf(effectId, AL_FREQUENCY_SHIFTER_FREQUENCY, frequencyHz);
    alEffecti(effectId, AL_FREQUENCY_SHIFTER_LEFT_DIRECTION, leftDirection);
    alEffecti(effectId, AL_FREQUENCY_SHIFTER_RIGHT_DIRECTION, rightDirection);
}
