#include "VibrationEnvelope.h"

using namespace leka;

void WindowEnvelope::apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples) {}

void SawtoothEnvelope::apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples)
{
	if (buffer == nullptr || totalSamples == 0) {
		return;
	}
	float increment = (1.F / static_cast<float>(totalSamples));
	float value		= increment * static_cast<float>(currentSample);

	for (uint32_t i = 0; i < nbSamples; ++i) {
		buffer[i] = value * buffer[i];
		value += increment;
	}
}