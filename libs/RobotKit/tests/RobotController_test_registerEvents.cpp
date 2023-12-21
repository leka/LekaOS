// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, registerEventsIsNotCharging)
{
	// TODO: Add EXPECT_CALL(mock_batterykit, onDataUpdated);
	// TODO: Add EXPECT_CALL(mock_batterykit, onLowBattery);

	{
		EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));
		EXPECT_CALL(battery, level).WillRepeatedly(Return(0));

		{
			{
				EXPECT_CALL(mock_ledkit, stop);
				EXPECT_CALL(mock_videokit, displayImage(std::filesystem::path {
											   "/fs/home/img/system/robot-battery-empty-must_be_charged.jpg"}));
				EXPECT_CALL(mock_motor_left, stop);
				EXPECT_CALL(mock_motor_right, stop);
			}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, lowBattery);
		}		// ? : on_data_updated callback call

		{
			EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload);
			// TODO: Specify which BLE service and what is expected if necessary
			EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(2);
		}	// ? : on_low_battery callback call

	}	// TODO: Replace with EXPECT_CALL(mock_batterykit, startEventHandler);

	EXPECT_CALL(battery, onChargeDidStart);
	EXPECT_CALL(battery, onChargeDidStop);

	rc.registerEvents();

	// TODO: Expect raise call
}

TEST_F(RobotControllerTest, registerEventsIsCharging)
{
	// TODO: Add EXPECT_CALL(mock_batterykit, onDataUpdated);
	// TODO: Add EXPECT_CALL(mock_batterykit, onLowBattery);

	{
		EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));
		EXPECT_CALL(battery, level).WillRepeatedly(Return(100));

		{
			{
				EXPECT_CALL(mock_videokit, displayImage(std::filesystem::path {
											   "/fs/home/img/system/robot-battery-charging-quarter_4-green.jpg"}));
			}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, chargingFull);
		}		// ? : on_data_updated callback call

		{
			EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload);
			// TODO: Specify which BLE service and what is expected if necessary
			EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(2);
		}	// ? : on_low_battery callback call

	}	// TODO: Replace with EXPECT_CALL(mock_batterykit, startEventHandler);

	EXPECT_CALL(battery, onChargeDidStart);
	EXPECT_CALL(battery, onChargeDidStop);

	rc.registerEvents();

	// TODO: Expect raise call
}
