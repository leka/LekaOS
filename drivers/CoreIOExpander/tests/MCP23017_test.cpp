// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "MCP23017.h"
#include <cstdint>

#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"

using namespace leka;

using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArrayArgument;

class CoreMCP23017Test : public ::testing::Test

{
  protected:
	CoreMCP23017Test() : coreIOExpander(i2cMock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	MCP23017 coreIOExpander;
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

TEST_F(CoreMCP23017Test, instantiation)
{
	ASSERT_NE(&coreIOExpander, nullptr);
}

TEST_F(CoreMCP23017Test, setRegisterMapping)
{
	std::array<uint8_t, 2> expected_IODIR_values = {0x00, 0x00};
	writeRegister(mcp23017::registers::IOCON, expected_IODIR_values);

	coreIOExpander.setRegisterMapping();
}

// std::array<uint8_t, 2> expected_INTCON_values = {0x00, 0x00};
// 		std::array<uint8_t, 2> actual_INTCON_values	  = {0x00, 0x00};
// 		readRegister(mcp23017::registers::INTCON, expected_INTCON_values);
// 		writeRegister(mcp23017::registers::INTCON, actual_INTCON_values);

TEST_F(CoreMCP23017Test, reset)
{
	{
		InSequence seq;
		EXPECT_CALL(i2cMock, write).Times(1);

		const auto expected_IODIR_values = ElementsAre(mcp23017::registers::IODIR, 0xff, 0xff);
		EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_IODIR_values));

		EXPECT_CALL(i2cMock, write).Times(10);
	}

	coreIOExpander.init();
}

TEST_F(CoreMCP23017Test, setInputPin)
{
	{
		InSequence seq;

		std::array<uint8_t, 2> expected_IODIR_values = {0x00, 0x0A};
		std::array<uint8_t, 2> actual_IODIR_values	 = {0x01, 0x0A};
		readRegister(mcp23017::registers::IODIR, expected_IODIR_values);
		writeRegister(mcp23017::registers::IODIR, actual_IODIR_values);
	}

	coreIOExpander.setInputPin(MCP23017::Pin::Pin_PA0);
}

TEST_F(CoreMCP23017Test, setOutputPin)
{
	coreIOExpander.setInputPin(MCP23017::Pin::Pin_PA0);
	{
		InSequence seq;
		std::array<uint8_t, 2> expected_IODIR_values = {0x01, 0x0A};
		std::array<uint8_t, 2> actual_IODIR_values	 = {0x00, 0x0A};
		readRegister(mcp23017::registers::IODIR, expected_IODIR_values);
		writeRegister(mcp23017::registers::IODIR, actual_IODIR_values);
	}

	coreIOExpander.setOutputPin(MCP23017::Pin::Pin_PA0);
}

TEST_F(CoreMCP23017Test, readInputs)
{
	coreIOExpander.setInputPin(MCP23017::Pin::Pin_PA0);
	{
		InSequence seq;

		std::array<uint8_t, 2> expected_GPIO_values = {0x00, 0xff};
		readRegister(mcp23017::registers::GPIO, expected_GPIO_values);
	}

	auto GPOI_values = coreIOExpander.readInputs();
	ASSERT_EQ(GPOI_values, 0xff00);
}

TEST_F(CoreMCP23017Test, setInputPolarity)
{
	uint16_t polarity_value = MCP23017::Pin::Pin_PA0 | MCP23017::Pin::Pin_PB5;

	std::array<uint8_t, 2> actual_IPOL_values = {MCP23017::Pin::Pin_PA0, MCP23017::Pin::Pin_PB5 >> 8};
	writeRegister(mcp23017::registers::IPOL, actual_IPOL_values);

	coreIOExpander.setInputPolarity(polarity_value);
}

TEST_F(CoreMCP23017Test, getInputPolarity)
{
	uint16_t expected_polarity_value = MCP23017::Pin::Pin_PA0 | MCP23017::Pin::Pin_PB5;
	coreIOExpander.setInputPolarity(expected_polarity_value);

	{
		InSequence seq;

		std::array<uint8_t, 2> expected_IPOL_values = {0x01, 0x20};
		readRegister(mcp23017::registers::IPOL, expected_IPOL_values);
	}

	auto actual_polarity_values = coreIOExpander.getInputPolarity();
	ASSERT_EQ(actual_polarity_values, expected_polarity_value);
}

TEST_F(CoreMCP23017Test, setPullups)
{
	uint16_t polarity_value = MCP23017::Pin::Pin_PA0 | MCP23017::Pin::Pin_PB5;

	std::array<uint8_t, 2> actual_GPPU_values = {MCP23017::Pin::Pin_PA0, MCP23017::Pin::Pin_PB5 >> 8};
	writeRegister(mcp23017::registers::GPPU, actual_GPPU_values);

	coreIOExpander.setPullups(polarity_value);
}

TEST_F(CoreMCP23017Test, getPullups)
{
	uint16_t expected_pullups_value = MCP23017::Pin::Pin_PA0 | MCP23017::Pin::Pin_PB5;
	coreIOExpander.setPullups(expected_pullups_value);

	{
		InSequence seq;

		std::array<uint8_t, 2> expected_GPPU_values = {0x01, 0x20};
		readRegister(mcp23017::registers::GPPU, expected_GPPU_values);
	}

	auto actual_pullups_values = coreIOExpander.getPullups();
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

	coreIOExpander.interruptOnChanges(MCP23017::Pin::Pin_PA0 | MCP23017::Pin::Pin_PA1);
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

	coreIOExpander.disableInterrupts(MCP23017::Pin::Pin_PA0);
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
	coreIOExpander.acknowledgeInterrupt(actual_pin, actual_values);

	ASSERT_EQ(actual_pin, 0x0300);
	ASSERT_EQ(actual_values, 0x0f00);
}
