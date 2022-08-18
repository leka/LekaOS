// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreQDAC.h"

#include "MemoryUtils.h"
#include "external/MCP4728.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"

using namespace leka;

using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
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
	const auto expected_vref		= static_cast<uint8_t>(mcp4728::command::set_vref | 0x00);
	const auto expected_buffer_vref = ElementsAre(expected_vref);

	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_buffer_vref));

	const auto expected_pd_first_byte	  = static_cast<uint8_t>(mcp4728::command::set_power_down | 0x00);
	const auto expected_pd_second_byte	  = uint8_t {0x00};
	const auto expected_buffer_power_down = ElementsAre(expected_pd_first_byte, expected_pd_second_byte);

	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_buffer_power_down));

	const auto expected_gain		= static_cast<uint8_t>(mcp4728::command::set_gain | 0x00);
	const auto expected_buffer_gain = ElementsAre(expected_gain);

	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_buffer_gain));

	dac.init();
}

TEST_F(CoreQDACTest, write)
{
	auto value_to_write = uint16_t {0x0ABC};

	auto command = std::array<uint8_t, 12> {};

	command.at(3 * mcp4728::channel::A) =
		static_cast<uint8_t>(mcp4728::command::multi_write | mcp4728::channel::A << 1);
	command.at(3 * mcp4728::channel::A + 1) = 0x00 | (0x0F & utils::memory::getHighByte(0x00));
	command.at(3 * mcp4728::channel::A + 2) = utils::memory::getLowByte(0x00);

	command.at(3 * mcp4728::channel::B) =
		static_cast<uint8_t>(mcp4728::command::multi_write | mcp4728::channel::B << 1);
	command.at(3 * mcp4728::channel::B + 1) = 0x00 | (0x0F & utils::memory::getHighByte(value_to_write));
	command.at(3 * mcp4728::channel::B + 2) = utils::memory::getLowByte(value_to_write);

	command.at(3 * mcp4728::channel::C) =
		static_cast<uint8_t>(mcp4728::command::multi_write | mcp4728::channel::C << 1);
	command.at(3 * mcp4728::channel::C + 1) = 0x00 | (0x0F & utils::memory::getHighByte(0x00));
	command.at(3 * mcp4728::channel::C + 2) = utils::memory::getLowByte(0x00);

	command.at(3 * mcp4728::channel::D) =
		static_cast<uint8_t>(mcp4728::command::multi_write | mcp4728::channel::D << 1);
	command.at(3 * mcp4728::channel::D + 1) = 0x00 | (0x0F & utils::memory::getHighByte(0x00));
	command.at(3 * mcp4728::channel::D + 2) = utils::memory::getLowByte(0x00);

	const auto expected_buffer = ElementsAreArray(command);

	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_buffer)).Times(1);

	dac.write(mcp4728::channel::B, value_to_write);
}

TEST_F(CoreQDACTest, writeOutOfBounds)
{
	auto value_to_write = uint16_t {0x0ABC};

	auto out_channel = uint8_t {5};
	dac.write(out_channel, value_to_write);
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

TEST_F(CoreQDACTest, readOutOfBounds)
{
	auto expected_data = uint16_t {0};

	auto out_channel = uint8_t {5};
	auto data		 = dac.read(out_channel);

	ASSERT_EQ(expected_data, data);
}
