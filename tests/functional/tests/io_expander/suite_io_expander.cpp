// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreI2C.h"
#include "CoreIOExpander.h"
#include "DigitalOut.h"
#include "tests/config.h"

using namespace leka;
using namespace std::chrono;
using namespace boost::ut;

suite suite_io_expander = [] {
	const uint8_t i2c_address {0x4E};
	auto corei2c		   = CoreI2C {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
	auto io_expander_reset = mbed::DigitalOut {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
	auto io_expander	   = CoreIOExpanderMCP23017 {corei2c, io_expander_reset};

	"Initialization"_test = [&] { expect(neq(&io_expander, nullptr)); };
};
