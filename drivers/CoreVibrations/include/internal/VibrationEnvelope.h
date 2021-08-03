// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_VIBRATION_ENVELOPE_H_
#define _LEKA_OS_LIB_VIBRATION_ENVELOPE_H_

#include <cstdint>
#include <cstdio>

namespace leka {

class VibrationEnvelope
{
  public:
	enum EnvelopType
	{
		Window,
		Triangle,	// configurable duty cycle
		Smooth,		// round
		Pulse		// multiple maximums
	};

	virtual void apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples) = 0;
};

class WindowEnvelope : public VibrationEnvelope
{
  public:
	WindowEnvelope() = default;
	void apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples) final;
};

class TriangleEnvelope : public VibrationEnvelope
{
  public:
	TriangleEnvelope(float dutyCycle) : _dutyCycle(dutyCycle)
	{
		if (dutyCycle > 1.F || dutyCycle < 0.F) {
			printf("Duty cycle must be between 0 and 1\n");
			// TODO() : handle error
		}
	};
	void apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples) final;

  private:
	float _dutyCycle;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_VIBRATION_ENVELOPE_H_