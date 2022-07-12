// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreQDAC.h"

#include "external/MCP4728.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"

using namespace leka;

using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArrayArgument;

class CoreQDACTest : public ::testing::Test

{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	const uint8_t i2c_address {0xC0};
	mock::CoreI2C mocki2c;
	CoreQDACMCP4728 dac {mocki2c, i2c_address};
};

TEST_F(CoreQDACTest, initializationDefault)
{
	auto new_dac = CoreQDACMCP4728 {mocki2c, i2c_address};
	ASSERT_NE(&new_dac, nullptr);
}

TEST_F(CoreQDACTest, init)
{
	const auto voltage_reference = ElementsAre(0x80);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(voltage_reference));

	const auto power_down = ElementsAre(0xA0, 0x00);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(power_down));

	const auto gain = ElementsAre(0xC0);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(gain));

	dac.init();
}

TEST_F(CoreQDACTest, write)
{
	auto value_to_write = uint16_t {0x0ABC};

	const auto expected_buffer_A = ElementsAre(0x40, 0x00, 0x00);
	const auto expected_buffer_B = ElementsAre(0x42, 0x0A, 0xBC);
	const auto expected_buffer_C = ElementsAre(0x44, 0x00, 0x00);
	const auto expected_buffer_D = ElementsAre(0x46, 0x00, 0x00);

	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_buffer_A)).Times(1);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_buffer_B)).Times(1);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_buffer_C)).Times(1);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_buffer_D)).Times(1);

	dac.write(mcp4728::channel::B, value_to_write);
}

TEST_F(CoreQDACTest, writeMemory)
{
	auto value_to_write = uint16_t {0x0ABC};

	const auto expected_buffer = ElementsAre(0x5A, 0x0A, 0xBC);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_buffer));

	dac.write(mcp4728::channel::B, value_to_write, true);
}

TEST_F(CoreQDACTest, writeAllChannels)
{
	auto value_to_write = uint16_t {0x0ABC};

	const auto expected_buffer = ElementsAre(0x0A, 0xBC);

	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_buffer)).Times(4);

	dac.writeAllChannels(value_to_write);
}

TEST_F(CoreQDACTest, writeAllChannelsMemory)
{
	auto value_to_write			 = uint16_t {0x0ABC};
	const auto expected_buffer_A = ElementsAre(0x50, 0x0A, 0xBC);
	const auto expected_buffer_B = ElementsAre(0x52, 0x0A, 0xBC);
	const auto expected_buffer_C = ElementsAre(0x54, 0x0A, 0xBC);
	const auto expected_buffer_D = ElementsAre(0x56, 0x0A, 0xBC);

	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_buffer_A)).Times(1);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_buffer_B)).Times(1);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_buffer_C)).Times(1);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_buffer_D)).Times(1);

	dac.writeAllChannels(value_to_write, true);
}

TEST_F(CoreQDACTest, read)
{
	auto expected_buffer = std::array<uint8_t, 24> {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
	auto expected_data	 = static_cast<uint16_t>(((expected_buffer.at(1) & 0x0F) << 8) | expected_buffer.at(2));

	EXPECT_CALL(mocki2c, read)
		.WillOnce(DoAll(SetArrayArgument<1>(begin(expected_buffer), end(expected_buffer)), Return(0)));

	auto data = dac.read(mcp4728::channel::A);

	ASSERT_EQ(expected_data, data);
}

TEST_F(CoreQDACTest, readMemory)
{
	auto expected_buffer = std::array<uint8_t, 24> {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
	auto expected_data	 = static_cast<uint16_t>(((expected_buffer.at(4) & 0x0F) << 8) | expected_buffer.at(5));

	EXPECT_CALL(mocki2c, read)
		.WillOnce(DoAll(SetArrayArgument<1>(begin(expected_buffer), end(expected_buffer)), Return(0)));

	auto data = dac.read(mcp4728::channel::A, true);

	ASSERT_EQ(expected_data, data);
}
