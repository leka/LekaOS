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
		EXPECT_CALL(
			mock_videokit,
			displayImage(std::filesystem::path {"/fs/home/img/system/robot-battery-empty-must_be_charged.jpg"}));
		expectedCallsStopMotors();
		EXPECT_CALL(battery, level).InSequence(on_low_battery_sequence);

		Sequence on_data_updated_sequence;
		EXPECT_CALL(battery, level).InSequence(on_data_updated_sequence);
		EXPECT_CALL(battery, isCharging).InSequence(on_data_updated_sequence).WillOnce(Return(false));
		EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload).InSequence(on_data_updated_sequence);
		// TODO: Specify which BLE service and what is expected if necessary
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(2).InSequence(on_data_updated_sequence);

		EXPECT_CALL(battery, onChargeDidStart);

		EXPECT_CALL(battery, onChargeDidStop);

		{
			InSequence event_setup_complete;

			Sequence is_charging_sequence;

			EXPECT_CALL(battery, isCharging).InSequence(is_charging_sequence).WillOnce(Return(false));
			// TODO: Specify which BLE service and what is expected if necessary
			EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(is_charging_sequence);

			Sequence run_launching_behavior_sequence;
			EXPECT_CALL(
				mock_videokit,
				displayImage(std::filesystem::path {"/fs/home/img/system/robot-misc-splash_screen-large-400.jpg"}))
				.InSequence(run_launching_behavior_sequence);
			EXPECT_CALL(mock_lcd, turnOn).InSequence(run_launching_behavior_sequence);

			EXPECT_CALL(timeout_state_transition, onTimeout);
			EXPECT_CALL(timeout_state_transition, start);

			EXPECT_CALL(mock_videokit, playVideoOnRepeat);
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
		EXPECT_CALL(battery, isCharging).InSequence(on_data_updated_sequence).WillOnce(Return(true));
		EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload).InSequence(on_data_updated_sequence);
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(2).InSequence(on_data_updated_sequence);
		EXPECT_CALL(mock_videokit, displayImage).InSequence(on_data_updated_sequence);

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
			EXPECT_CALL(
				mock_videokit,
				displayImage(std::filesystem::path {"/fs/home/img/system/robot-misc-splash_screen-large-400.jpg"}))
				.InSequence(run_launching_behavior_sequence);
			EXPECT_CALL(mock_lcd, turnOn).InSequence(run_launching_behavior_sequence);

			Sequence start_charging_behavior_sequence;
			EXPECT_CALL(battery, level).InSequence(start_charging_behavior_sequence);
			EXPECT_CALL(mock_videokit, displayImage).InSequence(start_charging_behavior_sequence);
			EXPECT_CALL(mock_ledkit, start).InSequence(start_charging_behavior_sequence);
			EXPECT_CALL(mock_lcd, turnOn).InSequence(start_charging_behavior_sequence);
			EXPECT_CALL(timeout_state_internal, onTimeout).InSequence(start_charging_behavior_sequence);
			EXPECT_CALL(timeout_state_internal, start).InSequence(start_charging_behavior_sequence);
		}
	}

	rc.registerEvents();
}

TEST_F(RobotControllerTest, registerOnFactoryResetNotificationCallback)
{
	mbed::Callback<void(bool)> callback {};

	rc.registerOnFactoryResetNotificationCallback(callback);

	// nothing can be expected
}

TEST_F(RobotControllerTest, onChargingBehaviorLevelBelow25)
{
	auto battery_level = 0;

	EXPECT_CALL(mock_videokit,
				displayImage(std::filesystem::path {"/fs/home/img/system/robot-battery-charging-empty_red.jpg"}))
		.Times(1);

	rc.onChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onChargingBehaviorLevelAbove25Below50)
{
	auto battery_level = 32;

	EXPECT_CALL(mock_videokit,
				displayImage(std::filesystem::path {"/fs/home/img/system/robot-battery-charging-quarter_1-red.jpg"}))
		.Times(1);

	rc.onChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onChargingBehaviorLevelAbove50Below75)
{
	auto battery_level = 62;

	EXPECT_CALL(mock_videokit,
				displayImage(std::filesystem::path {"/fs/home/img/system/robot-battery-charging-quarter_2-orange.jpg"}))
		.Times(1);

	rc.onChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onChargingBehaviorLevelAbove75Below90)
{
	auto battery_level = 88;

	EXPECT_CALL(mock_videokit,
				displayImage(std::filesystem::path {"/fs/home/img/system/robot-battery-charging-quarter_3-green.jpg"}))
		.Times(1);

	rc.onChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onChargingBehaviorLevelAbove90)
{
	auto battery_level = 94;

	EXPECT_CALL(mock_videokit,
				displayImage(std::filesystem::path {"/fs/home/img/system/robot-battery-charging-quarter_4-green.jpg"}))
		.Times(1);

	rc.onChargingBehavior(battery_level);
}
