// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreI2C.h"
#include "CoreQDAC.h"
#include "tests/config.h"

using namespace leka;
using namespace std::chrono;
using namespace boost::ut;

suite suite_qdac = [] {
	auto corei2c = CoreI2C {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
	auto dac	 = CoreQDACMCP4728 {corei2c, 0xC2};
	auto data	 = uint16_t {};
	auto channel = uint8_t {};

	"read channel A"_test = [&] {
		data	= 0x0ABC;
		channel = 0x00;
		dac.write(channel, data);
		auto ret = dac.read(channel);
		expect(data == ret) << "Failed to read Channel A";
	};

	"read channel B"_test = [&] {
		data	= 0x0DEF;
		channel = 0x01;
		dac.write(channel, data);
		auto ret = dac.read(channel);
		expect(data == ret) << "Failed to read Channel B";
	};
};
