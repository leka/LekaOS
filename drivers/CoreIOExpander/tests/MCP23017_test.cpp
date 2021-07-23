// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "MCP23017.h"

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
		const auto expected_pin_values = ElementsAre(mcp23017::registers::IODIR);
		EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_pin_values));

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
		const auto expected_pin_values = ElementsAre(mcp23017::registers::IODIR);
		EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_pin_values));

		std::array<uint8_t, 2> returned_values = {0x01, 0x0A};
		EXPECT_CALL(i2cMock, read)
			.WillOnce(DoAll(SetArrayArgument<1>(begin(returned_values), end(returned_values)), Return(0)));

		const auto expected_IODIR_values = ElementsAre(mcp23017::registers::IODIR, 0x00, 0x0A);
		EXPECT_CALL(i2cMock, write).With(Args<1, 2>(expected_IODIR_values));
	}

	coreIOExpander.setOutputPin(MCP23017::Pin::Pin_PA0);
}
