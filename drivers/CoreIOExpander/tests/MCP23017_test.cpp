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
};

TEST_F(CoreMCP23017Test, instantiation)
{
	ASSERT_NE(&coreIOExpander, nullptr);
}

TEST_F(CoreMCP23017Test, setRegisterMapping)
{
	const auto expected_IODIR_values = ElementsAre(mcp23017::registers::IOCON, 0x00, 0x00);
	EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_IODIR_values));

	coreIOExpander.setRegisterMapping();
}

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
		const auto expected_register = ElementsAre(mcp23017::registers::IODIR);
		EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_register));

		std::array<uint8_t, 2> returned_values = {0x00, 0x0A};
		EXPECT_CALL(i2cMock, read)
			.WillOnce(DoAll(SetArrayArgument<1>(begin(returned_values), end(returned_values)), Return(0)));

		const auto expected_IODIR_values = ElementsAre(mcp23017::registers::IODIR, 0x01, 0x0A);
		EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_IODIR_values));
	}

	coreIOExpander.setInputPin(MCP23017::Pin::Pin_PA0);
}

TEST_F(CoreMCP23017Test, setOutputPin)
{
	coreIOExpander.setInputPin(MCP23017::Pin::Pin_PA0);
	{
		InSequence seq;
		const auto expected_register = ElementsAre(mcp23017::registers::IODIR);
		EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_register));

		std::array<uint8_t, 2> returned_values = {0x01, 0x0A};
		EXPECT_CALL(i2cMock, read)
			.WillOnce(DoAll(SetArrayArgument<1>(begin(returned_values), end(returned_values)), Return(0)));

		const auto expected_IODIR_values = ElementsAre(mcp23017::registers::IODIR, 0x00, 0x0A);
		EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_IODIR_values));
	}

	coreIOExpander.setOutputPin(MCP23017::Pin::Pin_PA0);
}

TEST_F(CoreMCP23017Test, readInputs)
{
	coreIOExpander.setInputPin(MCP23017::Pin::Pin_PA0);
	{
		InSequence seq;
		const auto expected_register = ElementsAre(mcp23017::registers::GPIO);
		EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_register));

		std::array<uint8_t, 2> returned_values = {0x00, 0xff};
		EXPECT_CALL(i2cMock, read)
			.WillOnce(DoAll(SetArrayArgument<1>(begin(returned_values), end(returned_values)), Return(0)));
	}

	auto GPOI_values = coreIOExpander.readInputs();
	ASSERT_EQ(GPOI_values, 0xff00);
}

TEST_F(CoreMCP23017Test, setInputPolarity)
{
	uint16_t polarity_value = MCP23017::Pin::Pin_PA0 | MCP23017::Pin::Pin_PB5;

	const auto expected_pin_values =
		ElementsAre(mcp23017::registers::IPOL, MCP23017::Pin::Pin_PA0, MCP23017::Pin::Pin_PB5 >> 8);
	EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_pin_values));

	coreIOExpander.setInputPolarity(polarity_value);
}

TEST_F(CoreMCP23017Test, getInputPolarity)
{
	uint16_t expected_polarity_value = MCP23017::Pin::Pin_PA0 | MCP23017::Pin::Pin_PB5;
	coreIOExpander.setInputPolarity(expected_polarity_value);

	{
		InSequence seq;
		const auto expected_pin_values = ElementsAre(mcp23017::registers::IPOL);
		EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_pin_values));

		std::array<uint8_t, 2> returned_values = {0x01, 0x20};
		EXPECT_CALL(i2cMock, read)
			.WillOnce(DoAll(SetArrayArgument<1>(begin(returned_values), end(returned_values)), Return(0)));
	}

	auto actual_polarity_values = coreIOExpander.getInputPolarity();
	ASSERT_EQ(actual_polarity_values, expected_polarity_value);
}

TEST_F(CoreMCP23017Test, setPullups)
{
	uint16_t polarity_value = MCP23017::Pin::Pin_PA0 | MCP23017::Pin::Pin_PB5;

	const auto expected_pin_values =
		ElementsAre(mcp23017::registers::GPPU, MCP23017::Pin::Pin_PA0, MCP23017::Pin::Pin_PB5 >> 8);
	EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_pin_values));

	coreIOExpander.setPullups(polarity_value);
}

TEST_F(CoreMCP23017Test, getPullups)
{
	uint16_t expected_pullups_value = MCP23017::Pin::Pin_PA0 | MCP23017::Pin::Pin_PB5;
	coreIOExpander.setPullups(expected_pullups_value);

	{
		InSequence seq;
		const auto expected_pin_values = ElementsAre(mcp23017::registers::GPPU);
		EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_pin_values));

		std::array<uint8_t, 2> returned_values = {0x01, 0x20};
		EXPECT_CALL(i2cMock, read)
			.WillOnce(DoAll(SetArrayArgument<1>(begin(returned_values), end(returned_values)), Return(0)));
	}

	auto actual_pullups_values = coreIOExpander.getPullups();
	ASSERT_EQ(actual_pullups_values, expected_pullups_value);
}
