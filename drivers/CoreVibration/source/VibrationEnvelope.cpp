#include "VibrationEnvelope.h"

using namespace leka;

void WindowEnvelope::apply(uint16_t *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples) {}

void SawtoothEnvelope::apply(uint16_t *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples)
{
	float increment = (1.F / static_cast<float>(totalSamples));
	float value		= increment * static_cast<float>(currentSample);

	for (uint32_t i = 0; i < nbSamples; ++i) {
		buffer[i] = static_cast<uint16_t>(value * static_cast<float>(buffer[i]));
		value += increment;
	}
}