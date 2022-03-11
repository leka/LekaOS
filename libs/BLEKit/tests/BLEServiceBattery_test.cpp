// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEServiceBattery.h"

#include "gtest/gtest.h"

using namespace leka;

TEST(BLEServiceBatteryTest, initialisation)
{
	auto service_battery = BLEServiceBattery {};

	EXPECT_NE(&service_battery, nullptr);
}

TEST(BLEServiceBatteryTest, setBatteryLevelAny)
{
	auto service_battery = BLEServiceBattery {};

	uint8_t actual_level {};

	auto spy_callback = [&actual_level](const BLEServiceBattery::data_to_send_handle_t &handle) {
		actual_level = std::get<1>(handle)[0];
	};
	service_battery.onDataReadyToSend(spy_callback);

	for (uint8_t expected_level = 0; expected_level <= 100; expected_level++) {
		service_battery.setBatteryLevel(expected_level);
		EXPECT_EQ(actual_level, expected_level);
	}
}

TEST(BLEServiceBatteryTest, setBatteryLevelExceed)
{
	auto service_battery = BLEServiceBattery {};

	uint8_t actual_level {};

	auto spy_callback = [&actual_level](const BLEServiceBattery::data_to_send_handle_t &handle) {
		actual_level = std::get<1>(handle)[0];
	};
	service_battery.onDataReadyToSend(spy_callback);

	for (uint8_t level_send = 101; level_send < 255; level_send++) {
		service_battery.setBatteryLevel(level_send);
		EXPECT_EQ(actual_level, 100);
	}
}

TEST(BLEServiceBatteryTest, onDataReceived)
{
	auto service_battery = BLEServiceBattery {};

	auto dummy_params = BLEServiceBattery::data_received_handle_t {};
	service_battery.onDataReceived(dummy_params);

	// nothing expected
}
