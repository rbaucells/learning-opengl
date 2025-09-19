#include "../audio.h"

void LowPassFilter::applyFilter(const ALuint filterId) const {
    alFilteri(filterId, AL_FILTER_TYPE, getFilterType());

    alFilterf(filterId, AL_LOWPASS_GAIN, gain);
    alFilterf(filterId, AL_LOWPASS_GAINHF, gainHf);
}
