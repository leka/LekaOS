// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cmath>
#include <deque>

#include "PinNames.h"

#include "rtos/ThisThread.h"

#include "CoreMicrophone.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

// SMA - Small Moving Average
template <uint8_t N, class input_t = uint16_t, class sum_t = uint32_t>
class SMA
{
  public:
	SMA() = default;

	auto operator()(input_t input) -> input_t
	{
		sum -= previousInputs[index];
		sum += input;
		previousInputs[index] = input;
		if (++index == N) {
			index = 0;
		}
		return (sum + (N / 2)) / N;
	}

	static_assert(sum_t(0) < sum_t(-1),
				  "Error: sum data type should be an unsigned integer, otherwise, "
				  "the rounding operation in the return statement is invalid.");

  private:
	sum_t sum	  = 0;
	uint8_t index = 0;

	std::array<input_t, N> previousInputs {};
};

// RMS - Root Mean Square
auto RMS(std::deque<int> &data, int newvalue) -> int
{
	int square = 0;
	float mean = 0;
	float root = 0;

	data.push_back(newvalue);
	for (auto value: data) {
		square += value * value;
	}
	data.pop_front();

	mean = static_cast<float>(square) / static_cast<float>(data.size());

	root = std::sqrt(mean);

	return static_cast<int>(root);
}

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	// RMS vs SMA value (un/comment to test)
	// static auto sma_filter = SMA<5> {};
	auto rms_buffer = std::deque<int> {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	CoreMicrophone microphone(MCU_MIC_INPUT);

	while (true) {
		auto rawValue = static_cast<int>(1000 * microphone.readVolume());

		if (rawValue <= 458) {
			rawValue = std::abs(rawValue - 458) + 458;
		}

		// RMS vs SMA value (un/comment to test)
		// auto output = sma_filter(rawValue);
		auto output = RMS(rms_buffer, rawValue);

		// TODO (@john_doe): print floats
		if (rawValue > 650) {
			log_info("Peak: %d | Mean: %d", rawValue, output);
			rtos::ThisThread::sleep_for(6ms);
		}
	}
}
