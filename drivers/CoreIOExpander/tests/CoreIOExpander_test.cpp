// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreIOExpander.h"

#include "CoreI2C.h"
#include "gtest/gtest.h"
#include "span"

using namespace leka;
// using ::testing::_;

class CoreIOExpanderTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	void spy_writeRegister(uint8_t reg, uint16_t value)
	{
		std::array<uint8_t, 3> buffer {};

		buffer[0] = reg;
		buffer[1] = (0x00FF & value);
		buffer[2] = (0xFF00 & value) >> 8;

		corei2c.write(0, buffer.data(), buffer.size(), false);
	}

	auto spy_readRegister(uint8_t reg) -> uint16_t
	{
		auto buffer_write = std::array<uint8_t, 1> {reg};
		auto buffer_read  = std::array<uint8_t, 2> {};

		corei2c.write(0, buffer_write.data(), buffer_write.size(), true);

		corei2c.read(0, buffer_read.data(), buffer_read.size(), false);

		return static_cast<uint16_t>(buffer_read[0] + (buffer_read[1] << 8));
	}

	mbed::I2C i2c	= mbed::I2C(PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL);
	CoreI2C corei2c = CoreI2C {i2c};
	CoreIOExpanderMCP23017 expander = CoreIOExpanderMCP23017 {corei2c};
};

TEST_F(CoreIOExpanderTest, initializationDefault)
{
	ASSERT_NE(&i2c, nullptr);
	ASSERT_NE(&corei2c, nullptr);
	ASSERT_NE(&expander, nullptr);
}

TEST_F(CoreIOExpanderTest, reset)
{
	expander.reset();
}

TEST_F(CoreIOExpanderTest, config)
{
	expander.config(0, 0, 0);
}

TEST_F(CoreIOExpanderTest, setPinDirectionInput)
{
	expander.setPinDirection(mcp23017::pin::PA0, mcp23017::directory::DIR_INPUT);
}

TEST_F(CoreIOExpanderTest, setPinDirectionOutput)
{
	expander.setPinDirection(mcp23017::pin::PA0, mcp23017::directory::DIR_OUTPUT);
}

TEST_F(CoreIOExpanderTest, setPinAsInput)
{
	expander.setPinAsInput(mcp23017::pin::PA0);
}

TEST_F(CoreIOExpanderTest, readInputPin)
{
	auto input_value = expander.readInputPin(mcp23017::pin::PA0);
	ASSERT_EQ(0, input_value);
}

TEST_F(CoreIOExpanderTest, setModeForPinPullUp)
{
	expander.setModeForPin(mcp23017::pin::PA0, PinMode::PullUp);
}

TEST_F(CoreIOExpanderTest, setModeForPinPullNone)
{
	expander.setModeForPin(mcp23017::pin::PA0, PinMode::PullNone);
}

TEST_F(CoreIOExpanderTest, getModeForPin)
{
	auto mode = expander.getModeForPin(mcp23017::pin::PA0);
	ASSERT_EQ(PinMode::PullNone, mode);
}
