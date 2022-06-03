// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreIOExpander.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"
#include "mocks/mbed/DigitalOut.h"
#include "span"

using namespace leka;
using ::testing::_;
using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::InSequence;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;

class CoreIOExpanderTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	const uint8_t i2c_address {0x4E};
	mock::CoreI2C mocki2c;

	mbed::mock::DigitalOut expander_reset {};

	const uint16_t pin_number_in {0x0100};
	const uint16_t pin_number_out {0x0001};

	void MOCK_FUNCTION_expectedCallsReadRegister(uint8_t reg, uint16_t data, std::array<uint8_t, 2> &expected_buffer)
	{
		{
			InSequence seq;

			auto expected_reg = ElementsAre(reg);

			auto buffer_A	= static_cast<uint8_t>(data & 0x00FF);
			auto buffer_B	= static_cast<uint8_t>((data & 0xFF00) >> 8);
			expected_buffer = std::to_array<uint8_t>({buffer_A, buffer_B});

			EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).With(Args<1, 2>(expected_reg)).Times(1);
			EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
				.WillOnce(
					DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));
		}
	}

	void MOCK_FUNCTION_expectedCallsWriteRegister(uint8_t reg, uint16_t data)
	{
		{
			InSequence seq;

			auto buffer_A		  = static_cast<uint8_t>(data & 0x00FF);
			auto buffer_B		  = static_cast<uint8_t>((data & 0xFF00) >> 8);
			auto expected_command = ElementsAre(reg, buffer_A, buffer_B);
			EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).With(Args<1, 2>(expected_command)).Times(1);
		}
	}

	void MOCK_FUNCTION_expectedCallsInitAllRegisters()
	{
		{
			InSequence seq;
			EXPECT_CALL(expander_reset, write);
			EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::IODIR, 0xFFFF);
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::IPOL, 0x0000);
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::GPINTEN, 0x0000);
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::DEFVAL, 0x0000);
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::INTCON, 0x0000);
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::IOCON, 0x0000);
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::GPPU, 0x0000);
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::INTF, 0x0000);
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::INTCAP, 0x0000);
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::GPIO, 0x0000);
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::OLAT, 0x0000);
		}
	}
};

TEST_F(CoreIOExpanderTest, initializationDefault)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};
	EXPECT_NE(&expander, nullptr);
}

TEST_F(CoreIOExpanderTest, setPinAsInput)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();

	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0xFFFF, IODIR_expected_buffer);

	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::IODIR, 0xFFFF);

	expander.setPinAsInput(pin_number_in);
}

TEST_F(CoreIOExpanderTest, readInputPinConfigInputPinHigh)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0xFFFF, IODIR_expected_buffer);

	std::array<uint8_t, 2> GPIO_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::GPIO, 0xFFFF, GPIO_expected_buffer);

	auto output_value = expander.readInputPin(pin_number_in);
	EXPECT_EQ(1, output_value);
}

TEST_F(CoreIOExpanderTest, readInputPinConfigInputPinLow)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0xFFFF, IODIR_expected_buffer);

	std::array<uint8_t, 2> GPIO_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::GPIO, 0x0000, GPIO_expected_buffer);

	auto output_value = expander.readInputPin(pin_number_in);
	EXPECT_EQ(0, output_value);
}

TEST_F(CoreIOExpanderTest, readInputPinConfigOutputPin)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0x0000, IODIR_expected_buffer);

	auto output_value = expander.readInputPin(pin_number_in);
	EXPECT_EQ(0, output_value);
}

TEST_F(CoreIOExpanderTest, setModeForPinPullUpConfigInputPin)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0xFFFF, IODIR_expected_buffer);

	std::array<uint8_t, 2> GPPU_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::GPPU, 0xFFFF, GPPU_expected_buffer);
	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::GPPU, 0xFFFF);

	expander.setModeForPin(pin_number_in, PinMode::PullUp);
}

TEST_F(CoreIOExpanderTest, setModeForPinPullNoneConfigInputPin)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander			 = CoreIOExpanderMCP23017 {mocki2c, expander_reset};
	auto expected_dir_buffer = std::to_array<uint8_t>({0x00, 0x01});

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0xFFFF, IODIR_expected_buffer);

	std::array<uint8_t, 2> GPPU_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::GPPU, 0x0000, GPPU_expected_buffer);
	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::GPPU, 0x0000);
	expander.setModeForPin(pin_number_in, PinMode::PullNone);
}

TEST_F(CoreIOExpanderTest, setModeForPinPullDownConfigInputPin)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander			 = CoreIOExpanderMCP23017 {mocki2c, expander_reset};
	auto expected_dir_buffer = std::to_array<uint8_t>({0x00, 0x01});

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0xFFFF, IODIR_expected_buffer);

	std::array<uint8_t, 2> GPPU_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::GPPU, 0xFFFF, GPPU_expected_buffer);
	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::GPPU, 0xFFFF);
	expander.setModeForPin(pin_number_in, PinMode::PullDown);
}

TEST_F(CoreIOExpanderTest, setModeForPinConfigOutputPin)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0x0000, IODIR_expected_buffer);

	expander.setModeForPin(pin_number_in, PinMode::PullDown);
}

TEST_F(CoreIOExpanderTest, getModeForInputPinConfigInputPinPullNone)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0xFFFF, IODIR_expected_buffer);

	std::array<uint8_t, 2> GPPU_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::GPPU, 0x0000, GPPU_expected_buffer);

	auto mode = expander.getModeForPin(pin_number_in);
	EXPECT_EQ(PinMode::PullNone, mode);
}

TEST_F(CoreIOExpanderTest, getModeForInputPinConfigInputPinPullUp)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0xFFFF, IODIR_expected_buffer);

	std::array<uint8_t, 2> GPPU_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::GPPU, 0xFFFF, GPPU_expected_buffer);

	auto mode = expander.getModeForPin(pin_number_in);
	EXPECT_EQ(PinMode::PullUp, mode);
}

TEST_F(CoreIOExpanderTest, getModeForPinConfigOutputPin)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0x0000, IODIR_expected_buffer);

	auto mode = expander.getModeForPin(pin_number_in);
}

TEST_F(CoreIOExpanderTest, setPinAsOutput)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0x0000, IODIR_expected_buffer);
	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::IODIR, 0x0000);
	expander.setPinAsOutput(pin_number_out);
}

TEST_F(CoreIOExpanderTest, readOutputPinConfigInputPin)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0xFFFF, IODIR_expected_buffer);

	auto output_value = expander.readOutputPin(pin_number_out);
	EXPECT_EQ(0, output_value);
}

TEST_F(CoreIOExpanderTest, readOutputPinConfigOutputPinHigh)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0x0000, IODIR_expected_buffer);

	std::array<uint8_t, 2> GPIO_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::GPIO, 0xFFFF, GPIO_expected_buffer);

	auto output_value = expander.readOutputPin(pin_number_out);
	EXPECT_EQ(1, output_value);
}

TEST_F(CoreIOExpanderTest, readOutputPinConfigOutputPinLow)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0x0000, IODIR_expected_buffer);

	std::array<uint8_t, 2> GPIO_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::GPIO, 0x0000, GPIO_expected_buffer);

	auto output_value = expander.readOutputPin(pin_number_out);
	EXPECT_EQ(0, output_value);
}

TEST_F(CoreIOExpanderTest, writeOutputPinConfigOutputPinLow)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0x0000, IODIR_expected_buffer);

	std::array<uint8_t, 2> GPIO_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::GPIO, 0x0000, GPIO_expected_buffer);
	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::GPIO, 0x0000);

	expander.writeOutputPin(pin_number_out, 0x00);
}

TEST_F(CoreIOExpanderTest, writeOutputPinConfigOutputPinHigh)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0x0000, IODIR_expected_buffer);

	std::array<uint8_t, 2> GPIO_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::GPIO, 0xFFFF, GPIO_expected_buffer);
	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::GPIO, 0xFFFF);
	expander.writeOutputPin(pin_number_out, 0x01);
}

TEST_F(CoreIOExpanderTest, writeOutputPinConfigOutputPinBis)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0x0000, IODIR_expected_buffer);

	std::array<uint8_t, 2> GPIO_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::GPIO, 0xFFFF, GPIO_expected_buffer);
	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::registers::GPIO, 0xFFFF);
	expander.writeOutputPin(pin_number_out, 0x10);
}

TEST_F(CoreIOExpanderTest, writeOutputPinConfigInputPin)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::registers::IODIR, 0xFFFF, IODIR_expected_buffer);

	expander.writeOutputPin(pin_number_out, 0x10);
}
