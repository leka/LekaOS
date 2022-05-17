// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreIOExpander.h"

#include "CoreI2C.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"
#include "span"

using namespace leka;
using ::testing::_;
using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
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
	mbed::DigitalOut expander_reset {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
	CoreIOExpanderMCP23017 expander {mocki2c, expander_reset};
};

TEST_F(CoreIOExpanderTest, initializationDefault)
{
	CoreIOExpanderMCP23017 new_expander {mocki2c, expander_reset};
	ASSERT_NE(&new_expander, nullptr);
}

TEST_F(CoreIOExpanderTest, init)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(12);
	expander.init();
}

TEST_F(CoreIOExpanderTest, config_IODIR)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0);
}

TEST_F(CoreIOExpanderTest, config_GPPU)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_GPPU(0);
}

TEST_F(CoreIOExpanderTest, config_IPOL)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IPOL(0);
}

TEST_F(CoreIOExpanderTest, config_GPIO)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_GPIO(0);
}

TEST_F(CoreIOExpanderTest, setPinAsInput)
{
	auto expected_buffer = std::to_array<uint8_t>({0x00, 0x01});

	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).Times(1);
		EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
			.Times(1)
			.WillOnce(DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));

		EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	}
	expander.setPinAsInput(mcp23017::pin::PB0);
}

TEST_F(CoreIOExpanderTest, readInputPinConfigInputPinHigh)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0100);

	auto expected_buffer = std::to_array<uint8_t>({0x00, 0x01});
	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).Times(1);
		EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
			.Times(1)
			.WillOnce(DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));
	}

	auto output_value = expander.readInputPin(mcp23017::pin::PB0);
	EXPECT_EQ(0x0100, output_value);
}

TEST_F(CoreIOExpanderTest, readInputPinConfigInputPinLow)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0100);

	auto expected_buffer = std::to_array<uint8_t>({0x00, 0x00});
	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).Times(1);
		EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
			.Times(1)
			.WillOnce(DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));
	}

	auto output_value = expander.readInputPin(mcp23017::pin::PB0);
	EXPECT_EQ(0x0000, output_value);
}

TEST_F(CoreIOExpanderTest, readInputPinConfigOutputPin)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0000);

	auto output_value = expander.readInputPin(mcp23017::pin::PB0);
	EXPECT_EQ(0, output_value);
}

TEST_F(CoreIOExpanderTest, setModeForPinPullUp)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0100);

	auto expected_buffer = std::to_array<uint8_t>({0x00, 0x01});
	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).Times(1);
		EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
			.Times(1)
			.WillOnce(DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));
		EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	}

	expander.setModeForInputPin(mcp23017::pin::PB0, PinMode::PullUp);
}

TEST_F(CoreIOExpanderTest, setModeForPinPullNone)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0100);

	auto expected_buffer = std::to_array<uint8_t>({0x00, 0x01});
	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).Times(1);
		EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
			.Times(1)
			.WillOnce(DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));
		EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	}

	expander.setModeForInputPin(mcp23017::pin::PB0, PinMode::PullNone);
}

TEST_F(CoreIOExpanderTest, setModeForPinBis)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0100);
	auto expected_buffer = std::to_array<uint8_t>({0x00, 0x01});
	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).Times(1);
		EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
			.Times(1)
			.WillOnce(DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));
		EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	}

	expander.setModeForInputPin(mcp23017::pin::PB0, PinMode::PullDown);
}

TEST_F(CoreIOExpanderTest, getModeForInputPinConfigInputPinPullNone)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0100);
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_GPPU(0x0000);

	auto expected_buffer = std::to_array<uint8_t>({0x00, 0x00});
	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).Times(1);
		EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
			.Times(1)
			.WillOnce(DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));
	}
	auto mode = expander.getModeForInputPin(mcp23017::pin::PB0);
	EXPECT_EQ(PinMode::PullNone, mode);
}

TEST_F(CoreIOExpanderTest, getModeForInputPinConfigInputPinPullUp)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0100);
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_GPPU(0x0100);

	auto expected_buffer = std::to_array<uint8_t>({0x00, 0x01});
	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).Times(1);
		EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
			.Times(1)
			.WillOnce(DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));
	}
	auto mode = expander.getModeForInputPin(mcp23017::pin::PB0);
	EXPECT_EQ(PinMode::PullUp, mode);
}

TEST_F(CoreIOExpanderTest, getModeForPinConfigOutputPin)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0000);
	auto mode = expander.getModeForInputPin(mcp23017::pin::PB0);
}

TEST_F(CoreIOExpanderTest, setPinAsOutput)
{
	auto expected_buffer = std::to_array<uint8_t>({0x00, 0x00});

	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).Times(1);
		EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
			.Times(1)
			.WillOnce(DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));

		EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	}
	expander.setPinAsOutput(mcp23017::pin::PA0);
}

TEST_F(CoreIOExpanderTest, readOutputPinConfigInputPin)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0001);

	auto output_value = expander.readOutputPin(mcp23017::pin::PA0);
	EXPECT_EQ(0, output_value);
}

TEST_F(CoreIOExpanderTest, readOutputPinConfigOutputPinHigh)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0000);
	auto expected_buffer = std::to_array<uint8_t>({0x01, 0x00});
	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).Times(1);
		EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
			.Times(1)
			.WillOnce(DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));
	}

	auto output_value = expander.readOutputPin(mcp23017::pin::PA0);
	EXPECT_EQ(0x0001, output_value);
}

TEST_F(CoreIOExpanderTest, readOutputPinConfigOutputPinLow)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0000);
	auto expected_buffer = std::to_array<uint8_t>({0x00, 0x00});
	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).Times(1);
		EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
			.Times(1)
			.WillOnce(DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));
	}

	auto output_value = expander.readOutputPin(mcp23017::pin::PA0);
	EXPECT_EQ(0x0000, output_value);
}

TEST_F(CoreIOExpanderTest, writeOutputPinLowPowerMode)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0000);
	auto expected_buffer = std::to_array<uint8_t>({0x00, 0x00});
	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).Times(1);
		EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
			.Times(1)
			.WillOnce(DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));
		EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	}

	expander.writeOutputPin(mcp23017::pin::PA0, 0x00);
}

TEST_F(CoreIOExpanderTest, writeOutputPinNormalMode)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0000);
	auto expected_buffer = std::to_array<uint8_t>({0x00, 0x00});
	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).Times(1);
		EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
			.Times(1)
			.WillOnce(DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));
		EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	}
	expander.writeOutputPin(mcp23017::pin::PA0, 0x01);
}

TEST_F(CoreIOExpanderTest, writeOutputPinBis)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.config_IODIR(0x0000);
	auto expected_buffer = std::to_array<uint8_t>({0x00, 0x00});
	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).Times(1);
		EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
			.Times(1)
			.WillOnce(DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));
		EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	}
	expander.writeOutputPin(mcp23017::pin::PA0, 0x10);
}
