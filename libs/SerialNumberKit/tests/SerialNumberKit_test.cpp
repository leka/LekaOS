// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "SerialNumberKit.h"

#include "CastUtils.h"
#include "gtest/gtest.h"
#include "mocks/leka/MCU.h"

using namespace leka;

using ::testing::Return;

class SerialNumberKitTest : public ::testing::Test
{
  protected:
	SerialNumberKitTest() {}

	// void SetUp() override {}
	// void TearDown() override {}

	mock::MCU mock_mcu;
	SerialNumberKit serial_number_kit {mock_mcu};
};

TEST_F(SerialNumberKitTest, initialization)
{
	EXPECT_NE(&serial_number_kit, nullptr);
}

TEST_F(SerialNumberKitTest, getSerialNumber)
{
	auto mcu_id_returned		= MCUID {0x2A2B2C2D, 0x3A3B3C3D, 0x4A4B4C4D};
	auto expected_serial_number = std::array<uint8_t, 33> {"LK-22xx-2A2B2C2D3A3B3C3D4A4B4C4D"};

	EXPECT_CALL(mock_mcu, getID).WillOnce(Return(mcu_id_returned));

	auto actual_serial_number = serial_number_kit.getSerialNumber();
	EXPECT_EQ(actual_serial_number, expected_serial_number);
}

TEST_F(SerialNumberKitTest, getShortSerialNumber)
{
	auto mcu_id_returned		= MCUID {0x2A2B2C2D, 0x3A3B3C3D, 0x4A4B4C4D};
	auto expected_serial_number = std::array<uint8_t, 17> {"LK-22xx-2A2B2C2D"};

	EXPECT_CALL(mock_mcu, getID).WillOnce(Return(mcu_id_returned));

	auto actual_serial_number = serial_number_kit.getShortSerialNumber();
	EXPECT_EQ(actual_serial_number, expected_serial_number);
}
