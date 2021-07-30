// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_VIBRATION_ENVELOPE_H_
#define _LEKA_OS_LIB_VIBRATION_ENVELOPE_H_

#include <cstdint>

namespace leka {

class VibrationEnvelope
{
  public:
	enum EnvelopType
	{
		Window,
		Sawtooth,
		Triangular,	  // Up 2/3 of the time and down 1/3
		Buzz,		  // round
		Pulse		  // multiple maximums
	};

	virtual void apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples) = 0;
};

class WindowEnvelope : public VibrationEnvelope
{
  public:
	WindowEnvelope() = default;
	void apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples) final;
};

class SawtoothEnvelope : public VibrationEnvelope
{
  public:
	SawtoothEnvelope() = default;
	void apply(float *buffer, uint32_t nbSamples, uint32_t currentSample, uint32_t totalSamples) final;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_VIBRATION_ENVELOPE_H_
