#include <cmath>
#include <cstdio>

#include "VibrationEnvelope.h"

using namespace leka;

void WindowEnvelope::apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples) {}

void TriangleEnvelope::apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples)
{
	if (buffer == nullptr || totalSamples == 0) {
		return;
	}

	static const auto threshold = static_cast<uint32_t>(static_cast<float>(totalSamples) * _dutyCycle);
	float increment				= NAN;
	float value					= NAN;

	if (currentSample < threshold) {
		increment = 1.F / static_cast<float>(threshold);
		value	  = increment * static_cast<float>(currentSample);
	} else {
		increment = -1.F / static_cast<float>(totalSamples - threshold);
		value	  = 1.F + increment * static_cast<float>(currentSample - threshold);
	}

	for (uint32_t i = 0; i < nbSamples; ++i) {
		buffer[i] = value * buffer[i];
		value += increment;
	}
}

void SmoothEnvelope::apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples)
{
	if (buffer == nullptr || totalSamples == 0) {
		return;
	}

	static const auto sinCoef = static_cast<float>(2 * M_PI / totalSamples);
	const float cosOffset	  = 0.5;
	const float cosAmpli	  = -0.5;

	for (uint32_t i = 0; i < nbSamples; ++i) {
		buffer[i] *= static_cast<float>(cosAmpli * cos(static_cast<float>(i + currentSample) * sinCoef) + cosOffset);
	}
}