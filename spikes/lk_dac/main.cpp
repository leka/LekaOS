// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cmath>

#include "rtos/ThisThread.h"

#include "CoreDAC.h"
#include "CoreSTM32Hal.h"
#include "CoreSTM32HalBasicTimer.h"
#include "DigitalOut.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono_literals;

auto hal	   = CoreSTM32Hal {};
auto hal_timer = CoreSTM32HalBasicTimer {hal};
auto coredac   = CoreDAC {hal, hal_timer};

auto audio_enable = mbed::DigitalOut {SOUND_ENABLE, 1};

constexpr uint32_t sample_rate_hz = 44'100;
constexpr auto coefficient		  = 10;

void fillBufferWithSinWave(uint16_t *buffer, uint32_t samples_per_period, uint16_t maxValue, uint16_t minValue)
{
	auto resolution = 2.0 * M_PI / samples_per_period;

	auto sin0_1		   = [](double value) { return (sin(value) + 1.0) / 2.0; };
	auto normalization = [maxValue, minValue](double standard_value) {
		return standard_value * (maxValue - minValue) + minValue;
	};

	for (uint32_t sample = 0; sample < samples_per_period; sample += coefficient) {
		auto standard_value	  = sin0_1(sample * resolution);
		auto normalized_value = normalization(standard_value);
		std::fill_n(buffer + sample, coefficient, static_cast<uint16_t>(normalized_value));
	}
}

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	hal_timer.initialize(sample_rate_hz * coefficient);
	coredac.initialize();

	const uint32_t frequency = 440;
	const uint16_t maxVal	 = 0xFFF;
	const uint16_t minVal	 = 0x000;
	std::array<uint16_t, sample_rate_hz * coefficient / frequency> buffer {};
	fillBufferWithSinWave(buffer.data(), buffer.size(), maxVal, minVal);

	coredac.registerDataToPlay(buffer);

	log_info("buffer size: %d", buffer.size());
	log_info("Start sound");
	coredac.start();

	rtos::ThisThread::sleep_for(1s);

	log_info("Stop sound");
	coredac.stop();

	while (true) {
		rtos::ThisThread::sleep_for(1min);
	}
}
