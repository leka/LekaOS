// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, registerEventsBatteryIsNotCharging)
{
	rc.state_machine.set_current_states(lksm::state::setup);

	{
		InSequence seq;

		Sequence on_low_battery_sequence;
		EXPECT_CALL(battery, level).InSequence(on_low_battery_sequence);
		EXPECT_CALL(battery, isCharging).InSequence(on_low_battery_sequence).WillOnce(Return(false));
		EXPECT_CALL(mock_videokit, displayImage(std::filesystem::path {"/fs/images/loading.jpg"}));
		EXPECT_CALL(battery, level).InSequence(on_low_battery_sequence);

		Sequence on_data_updated_sequence;
		EXPECT_CALL(battery, level).InSequence(on_data_updated_sequence);
		// TODO: Specify which BLE service and what is expected if necessary
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(on_data_updated_sequence);

		EXPECT_CALL(timeout, onTimeout);

		EXPECT_CALL(battery, onChargeDidStart);

		EXPECT_CALL(battery, onChargeDidStop);

		{
			InSequence event_setup_complete;

			Sequence is_charging_sequence;

			EXPECT_CALL(battery, isCharging).InSequence(is_charging_sequence).WillOnce(Return(false));
			// TODO: Specify which BLE service and what is expected if necessary
			EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(is_charging_sequence);

			Sequence run_launching_behavior_sequence;
			EXPECT_CALL(mock_videokit, displayImage(std::filesystem::path {"/fs/images/logo.jpg"}))
				.InSequence(run_launching_behavior_sequence);
			EXPECT_CALL(mock_lcd, turnOn).InSequence(run_launching_behavior_sequence);

			EXPECT_CALL(timeout, onTimeout);
			EXPECT_CALL(timeout, start);

			EXPECT_CALL(mock_videokit, playVideo);
			EXPECT_CALL(mock_lcd, turnOn);
		}
	}

	rc.registerEvents();
}

TEST_F(RobotControllerTest, registerEventsBatteryIsCharging)
{
	rc.state_machine.set_current_states(lksm::state::setup);

	{
		InSequence seq;

		Sequence on_low_battery_sequence;
		EXPECT_CALL(battery, level).InSequence(on_low_battery_sequence);
		EXPECT_CALL(battery, isCharging).InSequence(on_low_battery_sequence).WillOnce(Return(true));
		EXPECT_CALL(battery, level).InSequence(on_low_battery_sequence);

		Sequence on_data_updated_sequence;
		EXPECT_CALL(battery, level).InSequence(on_data_updated_sequence);
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(on_data_updated_sequence);

		EXPECT_CALL(timeout, onTimeout);

		EXPECT_CALL(battery, onChargeDidStart);

		EXPECT_CALL(battery, onChargeDidStop);

		{
			InSequence event_setup_complete;

			Sequence is_charging_sequence;

			EXPECT_CALL(battery, isCharging).InSequence(is_charging_sequence).WillOnce(Return(true));
			// TODO: Specify which BLE service and what is expected if necessary
			EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(is_charging_sequence);

			EXPECT_CALL(battery, isCharging).InSequence(is_charging_sequence).WillOnce(Return(true));
			// TODO: Specify which BLE service and what is expected if necessary
			EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(is_charging_sequence);

			Sequence run_launching_behavior_sequence;
			EXPECT_CALL(mock_videokit, displayImage(std::filesystem::path {"/fs/images/logo.jpg"}))
				.InSequence(run_launching_behavior_sequence);
			EXPECT_CALL(mock_lcd, turnOn).InSequence(run_launching_behavior_sequence);

			Sequence start_charging_behavior_sequence;
			EXPECT_CALL(mock_lcd, turnOn).InSequence(start_charging_behavior_sequence);
			EXPECT_CALL(timeout, onTimeout).InSequence(start_charging_behavior_sequence);
			EXPECT_CALL(timeout, start).InSequence(start_charging_behavior_sequence);
		}
	}

	rc.registerEvents();
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelBelow25)
{
	auto battery_level = 0;

	EXPECT_CALL(mock_videokit, displayImage(std::filesystem::path {"/fs/images/low_battery.jpg"})).Times(1);
	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelAbove5Below25)
{
	auto battery_level = 22;

	EXPECT_CALL(mock_videokit, displayImage(std::filesystem::path {"/fs/images/battery_red.jpg"})).Times(1);
	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelAbove25Below50)
{
	auto battery_level = 42;

	EXPECT_CALL(mock_videokit, displayImage(std::filesystem::path {"/fs/images/battery_yellow_2.jpg"})).Times(1);
	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelAbove50Below75)
{
	auto battery_level = 66;

	EXPECT_CALL(mock_videokit, displayImage(std::filesystem::path {"/fs/images/battery_green_3.jpg"})).Times(1);
	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelAbove75)
{
	auto battery_level = 90;

	EXPECT_CALL(mock_videokit, displayImage(std::filesystem::path {"/fs/images/battery_green_4.jpg"})).Times(1);
	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onBleConnection)
{
	EXPECT_CALL(mock_motor_left, stop()).Times(2);
	EXPECT_CALL(mock_motor_right, stop()).Times(2);
	EXPECT_CALL(mock_ears, hide()).Times(1);
	EXPECT_CALL(mock_belt, hide()).Times(1);
	EXPECT_CALL(mock_videokit, stopVideo()).Times(2);

	rc.stopActuatorsOnBleConnection();
}

TEST_F(RobotControllerTest, onBleDisconnection)
{
	EXPECT_CALL(mock_motor_left, stop()).Times(2);
	EXPECT_CALL(mock_motor_right, stop()).Times(2);
	EXPECT_CALL(mock_ears, hide()).Times(1);
	EXPECT_CALL(mock_belt, hide()).Times(1);
	EXPECT_CALL(mock_videokit, stopVideo()).Times(2);

	rc.stopActuatorsOnBleDisconnection();
}
