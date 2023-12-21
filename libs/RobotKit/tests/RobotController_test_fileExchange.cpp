// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, startFileExchangeIsCharging)
{
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));

	{
		EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::blink_on_charge)));
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, blinkOnCharge);
	{
		EXPECT_CALL(mock_videokit, displayImage(std::filesystem::path {"/fs/home/img/system/robot-file_exchange.jpg"}));
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, fileExchange);
	EXPECT_CALL(mock_lcd, turnOn);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber());

	rc.startFileExchange();
}

TEST_F(RobotControllerTest, startFileExchangeIsNotCharging)
{
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	{
		EXPECT_CALL(mock_videokit, displayImage(std::filesystem::path {"/fs/home/img/system/robot-file_exchange.jpg"}));
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, fileExchange);
	EXPECT_CALL(mock_lcd, turnOn);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber());

	rc.startFileExchange();
}

TEST_F(RobotControllerTest, stopFileExchange)
{
	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber());

	rc.stopFileExchange();
}

TEST_F(RobotControllerTest, isReadyToFileExchange)
{
	auto actual_is_ready = bool {};
	auto minimal_battery = uint8_t {25};

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber());

	EXPECT_CALL(battery, level).WillRepeatedly(Return(minimal_battery));
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));
	actual_is_ready = rc.isReadyToFileExchange();
	EXPECT_FALSE(actual_is_ready);

	EXPECT_CALL(battery, level).WillRepeatedly(Return(minimal_battery));
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));
	actual_is_ready = rc.isReadyToFileExchange();
	EXPECT_FALSE(actual_is_ready);

	EXPECT_CALL(battery, level).WillRepeatedly(Return(minimal_battery + 1));
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));
	actual_is_ready = rc.isReadyToFileExchange();
	EXPECT_FALSE(actual_is_ready);

	EXPECT_CALL(battery, level).WillRepeatedly(Return(minimal_battery + 1));
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));
	actual_is_ready = rc.isReadyToFileExchange();
	EXPECT_TRUE(actual_is_ready);
}
