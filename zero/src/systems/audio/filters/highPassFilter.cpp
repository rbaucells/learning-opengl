#include "audioFilter.h"

void HighPass::applyFilter(const ALuint filterId) const {
    alFilteri(filterId, AL_FILTER_TYPE, getFilterType());

    alFilterf(filterId, AL_HIGHPASS_GAIN, gain);
    alFilterf(filterId, AL_HIGHPASS_GAINLF, gainLf);
}
