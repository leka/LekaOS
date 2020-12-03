// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "AudioFrequency.h"

namespace leka {

using namespace mbed;

volatile int _time_counter = 0;

void counter()
{
	_time_counter++;
}

AudioFrequency::AudioFrequency(AnalogOut &output) : _output(output) {}

void AudioFrequency::playFrequency(uint32_t frequency, uint16_t duration)
{
	_time_counter = 0;
	for (int k = 0; k < _sample_size; k++) {
		_analog_out_data[k] = ((1.0 + sin((float(k) / (float)_sample_size * 6.28318530717959))) / 2.0) * _volume_max;
	}

	std::chrono::seconds sec(1);
	_ticker.attach(&counter, sec);

	uint8_t i = 0;
	uint32_t time_step =
		(uint32_t)(1.0 * 1000000000 / ((float)frequency * (float)_sample_size));   // second value is Hz. Used in ns
	while (_time_counter < duration) {
		_output = _analog_out_data[i];
		i		= (i + 1) & 0x03F;
		wait_ns(time_step);
	}

	_ticker.detach();
}

}	// namespace leka