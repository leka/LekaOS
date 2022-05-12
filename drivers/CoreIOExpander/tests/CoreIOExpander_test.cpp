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

		corei2c.write(i2c_address, buffer.data(), buffer.size(), false);
	}

	auto spy_readRegister(uint8_t reg) -> uint16_t
	{
		auto buffer_write = std::array<uint8_t, 1> {reg};
		auto buffer_read  = std::array<uint8_t, 2> {};

		corei2c.write(i2c_address, buffer_write.data(), buffer_write.size(), true);

		corei2c.read(i2c_address, buffer_read.data(), buffer_read.size(), false);

		return static_cast<uint16_t>(buffer_read[0] + (buffer_read[1] << 8));
	}

	const uint8_t i2c_address {0x4E};
	CoreI2C corei2c {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
	mbed::DigitalOut expander_reset {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
	CoreIOExpanderMCP23017 expander {corei2c, expander_reset};
};

TEST_F(CoreIOExpanderTest, initializationDefault)
{
	ASSERT_NE(&corei2c, nullptr);
	ASSERT_NE(&expander, nullptr);
}

TEST_F(CoreIOExpanderTest, init)
{
	expander.init();
}

TEST_F(CoreIOExpanderTest, config_IODIR)
{
	expander.config_IODIR(0);
}

TEST_F(CoreIOExpanderTest, config_GPPU)
{
	expander.config_GPPU(0);
}

TEST_F(CoreIOExpanderTest, config_IPOL)
{
	expander.config_IPOL(0);
}

TEST_F(CoreIOExpanderTest, config_GPIO)
{
	expander.config_GPIO(0);
}

TEST_F(CoreIOExpanderTest, setPinAsInput)
{
	expander.setPinAsInput(mcp23017::pin::PB0);
}

TEST_F(CoreIOExpanderTest, readInputPin)
{
	auto output_value = expander.readInputPin(mcp23017::pin::PB0);
}

TEST_F(CoreIOExpanderTest, readInputPinConfigInputPin)
{
	auto dir_config = uint16_t {5};
	dir_config |= mcp23017::pin::PB0;
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	auto input_value = expander.readInputPin(mcp23017::pin::PB0);
}

TEST_F(CoreIOExpanderTest, readInputPinConfigOutputPin)
{
	auto dir_config = uint16_t {5};
	dir_config &= ~mcp23017::pin::PB0;
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	auto input_value = expander.readInputPin(mcp23017::pin::PB0);
}

TEST_F(CoreIOExpanderTest, setModeForPinPullUp)
{
	auto dir_config = uint16_t {5};
	dir_config |= mcp23017::pin::PB0;
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	expander.setModeForInputPin(mcp23017::pin::PB0, PinMode::PullUp);
}

TEST_F(CoreIOExpanderTest, setModeForPinPullNone)
{
	auto dir_config = uint16_t {5};
	dir_config |= mcp23017::pin::PB0;
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	expander.setModeForInputPin(mcp23017::pin::PB0, PinMode::PullNone);
}

TEST_F(CoreIOExpanderTest, setModeForPinBis)
{
	auto dir_config = uint16_t {5};
	dir_config |= mcp23017::pin::PB0;
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	expander.setModeForInputPin(mcp23017::pin::PB0, PinMode::PullDown);
}

TEST_F(CoreIOExpanderTest, getModeForInputPinConfigInputPin)
{
	auto dir_config = uint16_t {5};
	dir_config |= mcp23017::pin::PB0;
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	auto pullup_config = uint16_t {5};
	pullup_config &= ~mcp23017::pin::PB0;
	expander.config_GPPU(pullup_config);
	auto mode = expander.getModeForInputPin(mcp23017::pin::PB0);
}

TEST_F(CoreIOExpanderTest, getModeForInputPinConfigInputPinPullUp)
{
	auto dir_config = uint16_t {5};
	dir_config |= mcp23017::pin::PB0;
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	auto pullup_config = uint16_t {5};
	pullup_config |= mcp23017::pin::PB0;
	spy_writeRegister(mcp23017::registers::GPPU, pullup_config);
	auto mode = expander.getModeForInputPin(mcp23017::pin::PB0);
}

TEST_F(CoreIOExpanderTest, getModeForPinConfigOutputPin)
{
	auto dir_config = uint16_t {5};
	dir_config &= ~mcp23017::pin::PB0;
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	auto mode = expander.getModeForInputPin(mcp23017::pin::PB0);
}

TEST_F(CoreIOExpanderTest, setPinAsOutput)
{
	expander.setPinAsOutput(mcp23017::pin::PA0);
}

TEST_F(CoreIOExpanderTest, readOutputPin)
{
	auto dir_config = uint16_t {5};
	dir_config &= ~mcp23017::pin::PA0;
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	auto output_value = expander.readOutputPin(mcp23017::pin::PA0);
}

TEST_F(CoreIOExpanderTest, readOutputPinConfigInputPin)
{
	auto dir_config = uint16_t {5};
	dir_config |= mcp23017::pin::PA0;
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	auto input_value = expander.readOutputPin(mcp23017::pin::PA0);
}

TEST_F(CoreIOExpanderTest, readOutputPinConfigOutputPin)
{
	auto dir_config = uint16_t {5};
	dir_config &= ~mcp23017::pin::PA0;
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	auto input_value = expander.readOutputPin(mcp23017::pin::PA0);
}

TEST_F(CoreIOExpanderTest, writeOutputPinLowPowerMode)
{
	auto dir_config = uint16_t {5};
	dir_config &= ~mcp23017::pin::PA0;
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	expander.writeOutputPin(mcp23017::pin::PA0, 0x00);
}

TEST_F(CoreIOExpanderTest, writeOutputPinNormalMode)
{
	auto dir_config = uint16_t {5};
	dir_config &= ~mcp23017::pin::PA0;
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	expander.writeOutputPin(mcp23017::pin::PA0, 0x01);
}

TEST_F(CoreIOExpanderTest, writeOutputPinBis)
{
	auto dir_config = uint16_t {5};
	dir_config &= ~mcp23017::pin::PA0;
	spy_writeRegister(mcp23017::registers::IODIR, dir_config);
	expander.writeOutputPin(mcp23017::pin::PA0, 0x10);
}
