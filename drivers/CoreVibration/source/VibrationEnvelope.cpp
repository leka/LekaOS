#include "VibrationEnvelope.h"
#include <cmath>
#include <cstdio>

using namespace leka;

void WindowEnvelope::apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples) {}

void TriangleEnvelope::apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples)
{
	// static const float dutyCycle = 2.F / 3;
	static const auto threshold = static_cast<uint32_t>(static_cast<float>(totalSamples) * _dutyCycle);
	float increment				= NAN;
	float value					= NAN;

	if (buffer == nullptr || totalSamples == 0) {
		return;
	}
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
	static const float sinCoef = 2 * M_PI / totalSamples;
	if (buffer == nullptr || totalSamples == 0) {
		return;
	}
	for (uint32_t i = 0; i < nbSamples; ++i) {
		buffer[i] *= -0.5 * cos((i + currentSample) * sinCoef) + 0.5;
		// printf("%.2f\n", buffer[i]);
	}
}
