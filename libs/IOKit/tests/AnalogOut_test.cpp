// Leka - LekaOS
// Copyright 2021 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "IOKit/AnalogOut.h"

#include "gtest/gtest.h"
#include "mocks/leka/DACTouch.h"
#include "mocks/leka/I2C.h"

using namespace leka;
using ::testing::_;

class AnalogOutTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	const uint8_t i2c_address {0xC0};
	mock::CoreI2C mocki2c;
	mock::DACTouch mockDACTouch {mocki2c, i2c_address};

	const uint8_t channel {1};

	io::expanded::AnalogOut out {mockDACTouch, channel};
};

TEST_F(AnalogOutTest, instantiationDefaultPinType)
{
	io::expanded::AnalogOut new_out {mockDACTouch, channel};

	ASSERT_NE(&out, nullptr);
}

TEST_F(AnalogOutTest, write)
{
	const auto data = ElementsAre(0x0A, 0xBC);
	EXPECT_CALL(mockDACTouch, writeToSpecificInputRegister).With(Args<1, 2>(data));

	out.write(0x0ABC);
}
