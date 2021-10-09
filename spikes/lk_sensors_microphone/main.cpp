// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cmath>

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "drivers/I2C.h"
#include "platform/mbed_wait_api.h"
#include "rtos/ThisThread.h"

#include "HelloWorld.h"
#include "LKCoreMicrophone.h"
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
		if (++index == N) index = 0;
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
auto RMS(std::array<int, 10> &data, int newvalue) -> int
{
	int square = 0;
	float mean = 0;
	float root = 0;

	for (auto i: data) {
		square += data[i] * data[i];
		data[i] = data[i + 1];
	}

	data[data.size() - 1] = newvalue;

	square += newvalue * newvalue;

	mean = static_cast<float>(square) / static_cast<float>(data.size());

	root = std::sqrt(mean);

	return static_cast<int>(root);
}

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_trace_function([](const char *str, size_t size) { serial.write(str, size); });

	HelloWorld hello;
	hello.start();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	// RMS vs SMA value (un/comment to test)
	// static auto sma_filter = SMA<5> {};
	auto rms_buffer = std::array<int, 10> {};

	LKCoreMicrophone microphone(MCU_MIC_INPUT);

	while (true) {
		auto rawValue = static_cast<int>(1000 * microphone.readVolume());

		if (rawValue <= 458) {
			rawValue = std::abs(rawValue - 458) + 458;
		}

		// RMS vs SMA value (un/comment to test)
		// auto output = sma_filter(rawValue)
		auto output = RMS(rms_buffer, rawValue);

		// TODO: print floats
		log_info("%d", output);

		wait_us(250);
	}
}
