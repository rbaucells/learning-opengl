#include "audioFilter.h"

void BandPass::applyFilter(const ALuint filterId) const {
    alFilteri(filterId, AL_FILTER_TYPE, getFilterType());

    alFilterf(filterId, AL_BANDPASS_GAIN, gain);
    alFilterf(filterId, AL_BANDPASS_GAINLF, gainLf);
    alFilterf(filterId, AL_BANDPASS_GAINHF, gainHf);
}
