// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, stateEmergencyStoppedConnectedEventCommandReceivedIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::emergency_stopped, lksm::state::connected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	Sequence on_working_entry_sequence;
	EXPECT_CALL(timeout, onTimeout).InSequence(on_working_entry_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(on_working_entry_sequence);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	rc.state_machine.process_event(lksm::event::command_received {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working, lksm::state::connected));
}

TEST_F(RobotControllerTest, stateEmergencyStoppedDisconnectedEventCommandReceivedIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::emergency_stopped, lksm::state::disconnected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber());

	rc.state_machine.process_event(lksm::event::command_received {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::emergency_stopped, lksm::state::disconnected));
}

TEST_F(RobotControllerTest, stateEmergencyStoppedEventBleConnectionGuardIsNotCharging)
{
	rc.state_machine.set_current_states(lksm::state::emergency_stopped, lksm::state::disconnected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	expectedCallsStopActuators();
	Sequence on_ble_connection_sequence;
	EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::ble_connection)))
		.Times(1)
		.InSequence(on_ble_connection_sequence);
	EXPECT_CALL(mock_videokit, playVideoOnce).Times(1).InSequence(on_ble_connection_sequence);

	Sequence on_working_entry_sequence;
	EXPECT_CALL(timeout, onTimeout).InSequence(on_working_entry_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_lcd, turnOn).Times(2).InSequence(on_working_entry_sequence);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AtLeast(1));

	rc.state_machine.process_event(lksm::event::ble_connection {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working, lksm::state::connected));
}

TEST_F(RobotControllerTest, stateEmergencyStoppedEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::emergency_stopped);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));

	Sequence start_charging_behavior_sequence;
	EXPECT_CALL(battery, level).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, onTimeout).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, start).InSequence(start_charging_behavior_sequence);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AtLeast(1));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateEmergencyStoppedEventChargeDidStartGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::emergency_stopped);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::emergency_stopped));
}

TEST_F(RobotControllerTest, stateEmergencyStoppedConnectedEventCommandReceivedGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::emergency_stopped, lksm::state::connected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));

	Sequence start_charging_behavior_sequence;
	EXPECT_CALL(battery, level).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, onTimeout).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, start).InSequence(start_charging_behavior_sequence);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AtLeast(1));

	rc.state_machine.process_event(lksm::event::command_received {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging, lksm::state::connected));
}

TEST_F(RobotControllerTest, stateEmergencyStoppedDisonnectedEventCommandReceivedGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::emergency_stopped, lksm::state::disconnected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AtLeast(1));

	rc.state_machine.process_event(lksm::event::command_received {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::emergency_stopped, lksm::state::disconnected));
}

TEST_F(RobotControllerTest, stateEmergencyStoppedEventBleConnectionGuardIsCharging)
{
	rc.state_machine.set_current_states(lksm::state::emergency_stopped, lksm::state::disconnected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));

	Sequence start_charging_behavior_sequence;
	EXPECT_CALL(battery, level).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, onTimeout).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, start).InSequence(start_charging_behavior_sequence);

	expectedCallsStopActuators();
	Sequence on_ble_connection_sequence;
	EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::ble_connection)))
		.Times(1)
		.InSequence(on_ble_connection_sequence);
	EXPECT_CALL(mock_videokit, playVideoOnce).Times(0).InSequence(on_ble_connection_sequence);
	EXPECT_CALL(mock_lcd, turnOn).Times(0).InSequence(on_ble_connection_sequence);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AtLeast(1));

	rc.state_machine.process_event(lksm::event::ble_connection {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

// ! TODO: Refactor with composite SM & CoreTimer mock
TEST_F(RobotControllerTest, stateEmergencyStoppedDiceRollDetectedDelayNotOver)
{
	rc.state_machine.set_current_states(lksm::state::emergency_stopped);

	auto maximal_delay_before_over = 1s;

	EXPECT_CALL(mock_videokit, displayImage).Times(0);

	spy_kernel_addElapsedTimeToTickCount(maximal_delay_before_over);
	rc.onMagicCardAvailable(MagicCard::dice_roll);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::emergency_stopped));
}

// ! TODO: Refactor with composite SM & CoreTimer mock
TEST_F(RobotControllerTest, stateEmergencyStoppedDiceRollDetectedDelayOverEventAutonomousActivityRequested)
{
	rc.state_machine.set_current_states(lksm::state::emergency_stopped);

	auto minimal_delay_over = 1001ms;

	EXPECT_CALL(mock_videokit, displayImage).Times(1);

	spy_kernel_addElapsedTimeToTickCount(minimal_delay_over);
	rc.onMagicCardAvailable(MagicCard::dice_roll);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::autonomous_activities));
}
