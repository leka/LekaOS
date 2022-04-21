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
		EXPECT_CALL(battery, level).InSequence(on_low_battery_sequence);

		Sequence on_data_updated_sequence;
		EXPECT_CALL(battery, level).InSequence(on_data_updated_sequence);
		// TODO: Specify which BLE service and what is expected if necessary
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(on_data_updated_sequence);

		EXPECT_CALL(sleep_timeout, onTimeout);

		EXPECT_CALL(battery, onChargeDidStart);

		EXPECT_CALL(battery, onChargeDidStop);

		{
			InSequence event_setup_complete;

			Sequence is_charging_sequence;

			EXPECT_CALL(battery, isCharging).InSequence(is_charging_sequence).WillOnce(Return(false));
			// TODO: Specify which BLE service and what is expected if necessary
			EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).InSequence(is_charging_sequence);

			EXPECT_CALL(sleep_timeout, start);
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

		EXPECT_CALL(sleep_timeout, onTimeout);

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
		}
	}

	rc.registerEvents();
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelBelow25)
{
	auto battery_level = 0;

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelAbove5Below25)
{
	auto battery_level = 22;

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelAbove25Below50)
{
	auto battery_level = 42;

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelAbove50Below75)
{
	auto battery_level = 66;

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}

TEST_F(RobotControllerTest, onStartChargingBehaviorLevelAbove75)
{
	auto battery_level = 90;

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.onStartChargingBehavior(battery_level);
}
