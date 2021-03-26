// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"
#include <vector>

#include "PinNames.h"

#include "HelloWorld.h"
#include "LKCoreMicrophone.h"
#include "math.h"

using namespace leka;

HelloWorld hello;

LKCoreMicrophone microphone(MCU_MIC_INPUT);

static BufferedSerial serial(USBTX, USBRX, 9600);
constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

// SMA - Small Moving Average
template <uint8_t N, class input_t = uint16_t, class sum_t = uint32_t>
class SMA
{
  public:
	SMA() = default;

	input_t operator()(input_t input)
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

	input_t previousInputs[N] = {};
};

// RMS - Root Mean Square
int RMS(std::array<int, 10> &data, int newvalue)
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

	mean = square / data.size();

	root = std::sqrt(mean);

	return static_cast<int>(root);
}

int main(void)
{
	auto start = Kernel::Clock::now();
	rtos::ThisThread::sleep_for(2s);

	hello.start();

	std::array<int, 10> rms_buffer;
	static SMA<5> sma_filter;

	while (true) {
		auto t = Kernel::Clock::now() - start;

		int rawValue = static_cast<int>(1000 * microphone.readVolume());

		if (rawValue <= 458) {
			rawValue = std::abs(rawValue - 458) + 458;
		}

		// RMS vs SMA value (un/comment to test)
		// auto output = sma_filter(rawValue)
		auto output = RMS(rms_buffer, rawValue);

		// TODO: print floats
		int length = sprintf(buff, "%d\n", output);
		serial.write(buff, length);

		wait_us(250);
	}

	return 0;
}
