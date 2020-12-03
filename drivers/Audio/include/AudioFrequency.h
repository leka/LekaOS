// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_AUDIO_FREQUENCY_H_
#define _LEKA_OS_LIB_AUDIO_FREQUENCY_H_

#include <chrono>

#include "drivers/AnalogOut.h"
#include "drivers/Ticker.h"
#include "platform/mbed_wait_api.h"

#include "math.h"

namespace leka {

class AudioFrequency
{
  public:
	AudioFrequency(mbed::AnalogOut &output);

	void playFrequency(uint32_t frequency = 440, uint16_t duration = 10);

  private:
	mbed::AnalogOut &_output;
	mbed::Ticker _ticker;

	const int _sample_size = 64;
	float _analog_out_data[64];
	float _volume_max = 0.50f;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_AUDIO_FREQUENCY_H_
