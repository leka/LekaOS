// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreQDAC.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"

using namespace leka;

using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
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
	auto value_to_write = std::array<uint8_t, 2> {0xFF, 0xAA};

	const auto data = ElementsAre(0x5A, 0xFF, 0xAA);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(data));

	dac.write(mcp4728::channel::B, value_to_write);
}

TEST_F(CoreQDACTest, read)
{
	auto data		   = std::array<uint8_t, 24> {};
	auto expected_data = std::array<uint8_t, 24> {0x01, 0x02, 0x03, 0x04};

	EXPECT_CALL(mocki2c, read)
		.WillOnce(DoAll(SetArrayArgument<1>(begin(expected_data), end(expected_data)), Return(0)));

	dac.read(data);

	ASSERT_EQ(data, expected_data);
}
