// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDACTouch.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"

using namespace leka;

using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::Return;
using ::testing::SetArrayArgument;

class CoreDACTouchTest : public ::testing::Test

{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	const uint8_t i2c_address {0xC0};
	mock::CoreI2C mocki2c;
	CoreDACTouchMCP4728 dac {mocki2c, i2c_address};
};

TEST_F(CoreDACTouchTest, initializationDefault)
{
	auto new_dac = CoreDACTouchMCP4728 {mocki2c, i2c_address};
	ASSERT_NE(&new_dac, nullptr);
}

TEST_F(CoreDACTouchTest, writeToAllInputRegister)
{
	auto value_to_write = std::array<uint8_t, 2> {0x0F, 0xA8};

	const auto data = ElementsAre(0x0F, 0xA8);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(data));

	dac.writeToAllInputRegister(value_to_write);
}

TEST_F(CoreDACTouchTest, writeToSpecificInputRegister)
{
	auto value_to_write = std::array<uint8_t, 2> {0x0F, 0xA8};

	const auto data = ElementsAre(0x46, 0x0F, 0xA8);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(data));

	dac.writeToSpecificInputRegister(mcp4728::channel::D, value_to_write);
}

TEST_F(CoreDACTouchTest, writeToMemoryRegisterUntilChannelD)
{
	auto value_to_write = std::array<uint8_t, 2> {0xFF, 0xAA};

	const auto data = ElementsAre(0x50, 0xFF, 0xAA);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(data));

	dac.writeToMemoryRegisterUntilChannelD(mcp4728::channel::A, value_to_write);
}

TEST_F(CoreDACTouchTest, writeToSpecificMemoryRegister)
{
	auto value_to_write = std::array<uint8_t, 2> {0xFF, 0xAA};

	const auto data = ElementsAre(0x62, 0xFF, 0xAA);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(data));

	dac.writeToSpecificMemoryRegister(mcp4728::channel::B, value_to_write);
}

TEST_F(CoreDACTouchTest, readMemory)
{
	auto data		   = std::array<uint8_t, 24> {};
	auto expected_data = std::array<uint8_t, 24> {0x01, 0x02, 0x03, 0x04};

	EXPECT_CALL(mocki2c, read)
		.WillOnce(DoAll(SetArrayArgument<1>(begin(expected_data), end(expected_data)), Return(0)));

	dac.readMemory(data);

	ASSERT_EQ(data, expected_data);
}

TEST_F(CoreDACTouchTest, writeVoltageReference)
{
	const auto data = ElementsAre(0x80);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(data));

	dac.writeVoltageReference(mcp4728::data::voltage_reference::Vdd);
}

TEST_F(CoreDACTouchTest, writePowerMode)
{
	const auto data = ElementsAre(0xAC, 0x40);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(data));

	dac.writePowerMode(mcp4728::data::power_mode::channel_a::powerDown500K |
					   mcp4728::data::power_mode::channel_c::powerDown1K);
}

TEST_F(CoreDACTouchTest, writeGain)
{
	const auto data = ElementsAre(0xCF);
	EXPECT_CALL(mocki2c, write).With(Args<1, 2>(data));

	dac.writeGain(mcp4728::data::gain::x2::all);
}
