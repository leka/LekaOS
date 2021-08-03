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

}	// namespace leka

#endif	 // _LEKA_OS_LIB_VIBRATION_ENVELOPE_H_