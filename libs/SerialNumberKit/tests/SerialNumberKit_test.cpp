// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "SerialNumberKit.h"
#include <cstring>
#include <fstream>

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
	SerialNumberKit serial_number_kit {mock_mcu, SerialNumberKit::DEFAULT_CONFIG};

	MCUID mcu_id_returned {0x2A2B2C2D, 0x3A3B3C3D, 0x4A4B4C4D};
};

TEST_F(SerialNumberKitTest, initialization)
{
	EXPECT_NE(&serial_number_kit, nullptr);
}

TEST_F(SerialNumberKitTest, getSerialNumber)
{
	auto expected_serial_number = std::array<uint8_t, 32> {"LK-22062A2B2C2D3A3B3C3D4A4B4C4D"};

	EXPECT_CALL(mock_mcu, getID).WillOnce(Return(mcu_id_returned));

	auto actual_serial_number = serial_number_kit.getSerialNumber();
	EXPECT_TRUE(std::equal(std::begin(expected_serial_number), std::end(expected_serial_number),
						   std::begin(actual_serial_number)));
}

TEST_F(SerialNumberKitTest, getShortSerialNumber)
{
	auto expected_serial_number = std::array<uint8_t, 16> {"LK-22062A2B2C2D"};

	EXPECT_CALL(mock_mcu, getID).WillOnce(Return(mcu_id_returned));

	auto actual_serial_number = serial_number_kit.getShortSerialNumber();
	EXPECT_TRUE(std::equal(std::begin(expected_serial_number), std::end(expected_serial_number),
						   std::begin(actual_serial_number)));
}
