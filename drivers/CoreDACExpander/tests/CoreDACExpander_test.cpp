// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDACExpander.h"

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

class CoreDACExpanderTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	const uint8_t i2c_address {0xC0};
	const uint8_t channel {0x01};

	mock::CoreI2C mocki2c;
	CoreDACExpanderMCP4728 expander {mocki2c};
};

TEST_F(CoreDACExpanderTest, initializationDefault)
{
	CoreDACExpanderMCP4728 new_expander {mocki2c};
	EXPECT_NE(&new_expander, nullptr);
}

TEST_F(CoreDACExpanderTest, reset)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.reset(i2c_address, channel);
}

TEST_F(CoreDACExpanderTest, multiple_write_for_dac_input_registers)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.multiple_write_for_dac_input_registers(i2c_address, channel, 0x0FFF);
}

TEST_F(CoreDACExpanderTest, sequential_write_for_dac_input_registers_and_eeprom)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.sequential_write_for_dac_input_registers_and_eeprom(i2c_address, channel, 0x0FFF);
}

TEST_F(CoreDACExpanderTest, single_write_for_dac_input_register_and_eeprom)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
	expander.single_write_for_dac_input_register_and_eeprom(i2c_address, channel, 0x0FFF);
}

TEST_F(CoreDACExpanderTest, write_vref)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 1, false)).Times(1);
	expander.write_vref(i2c_address, channel);
}

TEST_F(CoreDACExpanderTest, write_power_down)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 2, false)).Times(1);
	expander.write_power_down(i2c_address, channel);
}

TEST_F(CoreDACExpanderTest, write_gain)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, 1, false)).Times(1);
	expander.write_gain(i2c_address, channel);
}
