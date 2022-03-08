// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEServiceRobot.h"

#include "gtest/gtest.h"

using namespace leka;

TEST(BLEServiceRobotTest, initialisation)
{
	auto service_robot = BLEServiceRobot {};

	EXPECT_NE(&service_robot, nullptr);
}

TEST(BLEServiceRobotTest, getSleepValueDefault)
{
	auto service_robot = BLEServiceRobot {};

	auto actual_sleep_value = service_robot.getSleepValue();
	EXPECT_FALSE(actual_sleep_value);
}

TEST(BLEServiceRobotTest, getSleepValueTrue)
{
	auto service_robot				 = BLEServiceRobot {};
	auto convert_to_handle_data_type = [](bool value) { return std::make_shared<uint8_t>(value).get(); };

	bool expected_sleep_value = true;

	auto data_received_handle = interface::BLEService::data_received_handle_t {
	};
	data_received_handle.data = convert_to_handle_data_type(expected_sleep_value);

	service_robot.onDataReceived(data_received_handle);

	auto actual_sleep_value = service_robot.getSleepValue();
	EXPECT_EQ(actual_sleep_value, expected_sleep_value);
}

TEST(BLEServiceRobotTest, getSleepValueNotSameHandle)
{
	auto service_robot = BLEServiceRobot {};

	bool expected_sleep_value = true;

	auto convert_to_handle_data_type = [](bool value) { return std::make_shared<uint8_t>(value).get(); };

	auto data_received_handle = interface::BLEService::data_received_handle_t {
	};
	data_received_handle.handle = 0xFFFF;
	data_received_handle.data	= convert_to_handle_data_type(expected_sleep_value);

	service_robot.onDataReceived(data_received_handle);

	auto actual_sleep_value = service_robot.getSleepValue();
	EXPECT_NE(actual_sleep_value, expected_sleep_value);
}
