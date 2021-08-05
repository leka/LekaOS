// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreIOExpander.h"
#include <cstdint>

#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"

using namespace leka;

using ::testing::_;
using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArrayArgument;

class CoreMCP23017Test : public ::testing::Test

{
  protected:
	CoreMCP23017Test() : coreMCP23017(i2cMock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	MCP23017 coreMCP23017;
	LKCoreI2CMock i2cMock;

	void readRegister(uint16_t pin, std::array<uint8_t, 2> &values)
	{
		const auto expected_register = ElementsAre(pin);
		EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_register));

		EXPECT_CALL(i2cMock, read).WillOnce(DoAll(SetArrayArgument<1>(begin(values), end(values)), Return(0)));
	}

	void writeRegister(uint16_t pin, std::array<uint8_t, 2> values)
	{
		const auto data = ElementsAre(pin, values[0], values[1]);
		EXPECT_CALL(i2cMock, write).With(Args<1, 2>(data));
	}
};

TEST_F(CoreMCP23017Test, mcp23017Instantiation)
{
	MCP23017::ExpandedInput coreExpandedInput = coreMCP23017.asInput(mcp23017::pin::PA0);

	ASSERT_NE(&coreMCP23017, nullptr);
	ASSERT_NE(&coreExpandedInput, nullptr);
}

TEST_F(CoreMCP23017Test, inputRead)
{
	MCP23017::ExpandedInput coreExpandedInput	 = coreMCP23017.asInput(mcp23017::pin::PA0);
	std::array<uint8_t, 2> expected_input_values = {0x01, 0x00};

	{
		InSequence seq;

		readRegister(mcp23017::registers::GPIO, expected_input_values);
	}

	auto actual_input_values = coreExpandedInput.read();
	ASSERT_EQ(actual_input_values,
			  static_cast<int>(expected_input_values[0]) + (static_cast<int>(expected_input_values[1]) << 8));
}

TEST_F(CoreMCP23017Test, inputModePullUp)
{
	MCP23017::ExpandedInput coreExpandedInput = coreMCP23017.asInput(mcp23017::pin::PA0);

	{
		InSequence seq;

		std::array<uint8_t, 2> expected_GPPU_values = {0x00, 0x00};
		std::array<uint8_t, 2> actual_GPPU_values	= {0x01, 0x00};

		readRegister(mcp23017::registers::GPPU, expected_GPPU_values);
		writeRegister(mcp23017::registers::GPPU, actual_GPPU_values);
	}

	coreExpandedInput.mode(PinMode::PullUp);
}

TEST_F(CoreMCP23017Test, inputModePullNone)
{
	MCP23017::ExpandedInput coreExpandedInput = coreMCP23017.asInput(mcp23017::pin::PA0);

	{
		InSequence seq;

		std::array<uint8_t, 2> expected_GPPU_values = {0xff, 0x00};
		std::array<uint8_t, 2> actual_GPPU_values	= {0xfe, 0x00};

		readRegister(mcp23017::registers::GPPU, expected_GPPU_values);
		writeRegister(mcp23017::registers::GPPU, actual_GPPU_values);
	}

	coreExpandedInput.mode(PinMode::PullNone);
}

TEST_F(CoreMCP23017Test, inputModePullDown)
{
	MCP23017::ExpandedInput coreExpandedInput = coreMCP23017.asInput(mcp23017::pin::PA0);

	EXPECT_CALL(i2cMock, write).Times(0);
	EXPECT_CALL(i2cMock, read).Times(0);

	coreExpandedInput.mode(PinMode::PullDown);
}

TEST_F(CoreMCP23017Test, isConnected)
{
	MCP23017::ExpandedInput coreExpandedInput = coreMCP23017.asInput(mcp23017::pin::PA0);

	auto is_connected = coreExpandedInput.is_connected();
	ASSERT_EQ(is_connected, 1);
}

TEST_F(CoreMCP23017Test, setRegisterMapping)
{
	{
		InSequence seq;

		std::array<uint8_t, 2> expected_IOCON_values = {0x80, 0x0F};
		std::array<uint8_t, 2> actual_IOCON_values	 = {0x00, 0x0F};
		readRegister(mcp23017::registers::IOCON, expected_IOCON_values);
		writeRegister(mcp23017::registers::IOCON, actual_IOCON_values);
	}

	coreMCP23017.setRegisterMapping();
}

TEST_F(CoreMCP23017Test, setRegisterMappingSeparated)
{
	{
		InSequence seq;

		std::array<uint8_t, 2> expected_IOCON_values = {0x00, 0x0F};
		std::array<uint8_t, 2> actual_IOCON_values	 = {0x80, 0x8F};
		readRegister(mcp23017::registers::IOCON, expected_IOCON_values);
		writeRegister(mcp23017::registers::IOCON, actual_IOCON_values);
	}

	coreMCP23017.setRegisterMapping(true);
}

TEST_F(CoreMCP23017Test, reset)
{
	{
		InSequence seq;
		const auto expected_reset_IODIR_values = ElementsAre(_, 0x00, 0x00);
		EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_reset_IODIR_values)).Times(11);

		std::array<uint8_t, 2> expected_IODIR_values = {0x00, 0x00};
		std::array<uint8_t, 2> actual_IODIR_values	 = {0x01, 0x00};
		readRegister(mcp23017::registers::IODIR, expected_IODIR_values);
		writeRegister(mcp23017::registers::IODIR, actual_IODIR_values);

		std::array<uint8_t, 2> actual_GPPU_values = {mcp23017::pin::PA0, 0x00};
		writeRegister(mcp23017::registers::GPPU, actual_GPPU_values);

		std::array<uint8_t, 2> actual_GPIO_values = {0x00, mcp23017::pin::PA0};
		writeRegister(mcp23017::registers::GPIO, actual_GPIO_values);
	}

	coreMCP23017.init(mcp23017::pin::PA0);
}

TEST_F(CoreMCP23017Test, setInputPins)
{
	{
		InSequence seq;

		std::array<uint8_t, 2> expected_IODIR_values = {0x00, 0x0A};
		std::array<uint8_t, 2> actual_IODIR_values	 = {0x01, 0x0A};
		readRegister(mcp23017::registers::IODIR, expected_IODIR_values);
		writeRegister(mcp23017::registers::IODIR, actual_IODIR_values);
	}

	coreMCP23017.setInputPins(mcp23017::pin::PA0);
}

TEST_F(CoreMCP23017Test, setOutputPins)
{
	coreMCP23017.setInputPins(mcp23017::pin::PA0);
	{
		InSequence seq;
		std::array<uint8_t, 2> expected_IODIR_values = {0x01, 0x0A};
		std::array<uint8_t, 2> actual_IODIR_values	 = {0x00, 0x0A};
		readRegister(mcp23017::registers::IODIR, expected_IODIR_values);
		writeRegister(mcp23017::registers::IODIR, actual_IODIR_values);
	}

	coreMCP23017.setOutputPins(mcp23017::pin::PA0);
}

TEST_F(CoreMCP23017Test, writeOutputs)
{
	std::array<uint8_t, 2> actual_GPIO_values = {0x00, mcp23017::pin::PA0};
	writeRegister(mcp23017::registers::GPIO, actual_GPIO_values);

	coreMCP23017.writeOutputs(mcp23017::pin::PA0);
}

TEST_F(CoreMCP23017Test, readOutputs)
{
	coreMCP23017.setInputPins(mcp23017::pin::PA0);
	{
		InSequence seq;
		std::array<uint8_t, 2> expected_GPIO_values = {0x01, 0x00};
		readRegister(mcp23017::registers::OLAT, expected_GPIO_values);
	}

	auto actual_GPIO_values = coreMCP23017.readOutputs();

	ASSERT_EQ(actual_GPIO_values, 0x01);
}

TEST_F(CoreMCP23017Test, readInputs)
{
	coreMCP23017.setInputPins(mcp23017::pin::PA0);
	{
		InSequence seq;

		std::array<uint8_t, 2> expected_GPIO_values = {0x00, 0xff};
		readRegister(mcp23017::registers::GPIO, expected_GPIO_values);
	}

	auto GPOI_values = coreMCP23017.readInputs();
	ASSERT_EQ(GPOI_values, 0xff00);
}

TEST_F(CoreMCP23017Test, setInputPolarity)
{
	uint16_t polarity_value = mcp23017::pin::PA0 | mcp23017::pin::PB5;

	std::array<uint8_t, 2> actual_IPOL_values = {mcp23017::pin::PA0, mcp23017::pin::PB5 >> 8};
	writeRegister(mcp23017::registers::IPOL, actual_IPOL_values);

	coreMCP23017.setInputPolarity(polarity_value);
}

TEST_F(CoreMCP23017Test, getInputPolarity)
{
	uint16_t expected_polarity_value = mcp23017::pin::PA0 | mcp23017::pin::PB5;
	coreMCP23017.setInputPolarity(expected_polarity_value);

	{
		InSequence seq;

		std::array<uint8_t, 2> expected_IPOL_values = {0x01, 0x20};
		readRegister(mcp23017::registers::IPOL, expected_IPOL_values);
	}

	auto actual_polarity_values = coreMCP23017.getInputPolarity();
	ASSERT_EQ(actual_polarity_values, expected_polarity_value);
}

TEST_F(CoreMCP23017Test, setPullups)
{
	uint16_t polarity_value = mcp23017::pin::PA0 | mcp23017::pin::PB5;

	std::array<uint8_t, 2> actual_GPPU_values = {mcp23017::pin::PA0, mcp23017::pin::PB5 >> 8};
	writeRegister(mcp23017::registers::GPPU, actual_GPPU_values);

	coreMCP23017.setPullups(polarity_value);
}

TEST_F(CoreMCP23017Test, getPullups)
{
	uint16_t expected_pullups_value = mcp23017::pin::PA0 | mcp23017::pin::PB5;
	coreMCP23017.setPullups(expected_pullups_value);

	{
		InSequence seq;

		std::array<uint8_t, 2> expected_GPPU_values = {0x01, 0x20};
		readRegister(mcp23017::registers::GPPU, expected_GPPU_values);
	}

	auto actual_pullups_values = coreMCP23017.getPullups();
	ASSERT_EQ(actual_pullups_values, expected_pullups_value);
}

TEST_F(CoreMCP23017Test, interruptOnChanges)
{
	{
		InSequence seq;

		std::array<uint8_t, 2> expected_INTCON_values = {0x00, 0x00};
		std::array<uint8_t, 2> actual_INTCON_values	  = {0x00, 0x00};
		readRegister(mcp23017::registers::INTCON, expected_INTCON_values);
		writeRegister(mcp23017::registers::INTCON, actual_INTCON_values);

		std::array<uint8_t, 2> expected_GPINTEN_values = {0x00, 0x00};
		std::array<uint8_t, 2> actual_GPINTEN_values   = {0x03, 0x00};
		readRegister(mcp23017::registers::GPINTEN, expected_GPINTEN_values);
		writeRegister(mcp23017::registers::GPINTEN, actual_GPINTEN_values);
	}

	coreMCP23017.interruptOnChanges(mcp23017::pin::PA0 | mcp23017::pin::PA1);
}

TEST_F(CoreMCP23017Test, disableInterrupts)
{
	{
		InSequence seq;

		std::array<uint8_t, 2> expected_GPINTEN_values = {0xff, 0xff};
		std::array<uint8_t, 2> actual_GPINTEN_values   = {0xfe, 0xff};
		readRegister(mcp23017::registers::GPINTEN, expected_GPINTEN_values);
		writeRegister(mcp23017::registers::GPINTEN, actual_GPINTEN_values);
	}

	coreMCP23017.disableInterrupts(mcp23017::pin::PA0);
}

TEST_F(CoreMCP23017Test, acknowledgeInterrupt)
{
	{
		InSequence seq;

		std::array<uint8_t, 2> expected_INTF_values	  = {0x00, 0x03};
		std::array<uint8_t, 2> expected_INTCAP_values = {0x00, 0x0f};
		readRegister(mcp23017::registers::INTF, expected_INTF_values);
		readRegister(mcp23017::registers::INTCAP, expected_INTCAP_values);
	}

	uint16_t actual_pin {};
	uint16_t actual_values {};
	coreMCP23017.acknowledgeInterrupt(actual_pin, actual_values);

	ASSERT_EQ(actual_pin, 0x0300);
	ASSERT_EQ(actual_values, 0x0f00);
}
