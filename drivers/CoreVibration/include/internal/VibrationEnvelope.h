// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_VIBRATION_ENVELOPE_H_
#define _LEKA_OS_LIB_VIBRATION_ENVELOPE_H_

#include <cstdint>
#include <cstdio>
#include <stdexcept>

#include "LogKit.h"

namespace leka {

class VibrationEnvelope
{
  public:
	enum class EnvelopType
	{
		Window,
		Triangle,	// Configurable duty cycle
		Smooth		// Starts and ends smoothly
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
	explicit TriangleEnvelope(float dutyCycle) : _dutyCycle(dutyCycle)
	{
		if (dutyCycle > 1.F || dutyCycle < 0.F) {
			log_error("ERROR: Duty cycle must be between 0 and 1");
			// TODO() : handle error
		}
	};
	void apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples) final;

  private:
	float _dutyCycle;
};

class SmoothEnvelope : public VibrationEnvelope
{
  public:
	SmoothEnvelope() = default;
	void apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples) final;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_VIBRATION_ENVELOPE_H_