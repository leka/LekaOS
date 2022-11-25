// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, stateFileExchangeEventFileExchangeStopRequestedGuardIsCharging)
{
	rc.state_machine.set_current_states(lksm::state::file_exchange);

	auto returned_is_charging = true;

	// TODO (@yann): Trigger file_exchange_stop_requested in StateMachine from BLE and do not use process_event

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(returned_is_charging));

	Sequence on_file_exchange_end;
	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber()).InSequence(on_file_exchange_end);

	Sequence start_charging_behavior_sequence;
	EXPECT_CALL(battery, level).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, onTimeout).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, start).InSequence(start_charging_behavior_sequence);

	rc.state_machine.process_event(lksm::event::file_exchange_stop_requested {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateFileExchangeEventFileExchangeStopRequestedGuardIsNotCharging)
{
	rc.state_machine.set_current_states(lksm::state::file_exchange);

	auto returned_is_charging = false;

	// TODO (@yann): Trigger file_exchange_stop_requested in StateMachine from BLE and do not use process_event

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(returned_is_charging));

	Sequence on_file_exchange_end;
	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber()).InSequence(on_file_exchange_end);

	Sequence on_working_entry_sequence;
	EXPECT_CALL(timeout, onTimeout).InSequence(on_working_entry_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(on_working_entry_sequence);

	rc.state_machine.process_event(lksm::event::file_exchange_stop_requested {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working));
}

TEST_F(RobotControllerTest, stateFileExchangeEventDisconnectionGuardIsCharging)
{
	rc.state_machine.set_current_states(lksm::state::file_exchange, lksm::state::connected);

	auto returned_is_charging = true;

	// TODO (@yann): Trigger ble_disonnection in StateMachine from BLE and do not use process_event

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(returned_is_charging));

	Sequence on_file_exchange_end;
	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber()).InSequence(on_file_exchange_end);

	EXPECT_CALL(mock_videokit, stopVideo).Times(2);
	EXPECT_CALL(mock_motor_left, stop).Times(2);
	EXPECT_CALL(mock_motor_right, stop).Times(2);
	EXPECT_CALL(mock_ears, hide).Times(1);
	EXPECT_CALL(mock_belt, hide).Times(1);

	Sequence start_charging_behavior_sequence;
	EXPECT_CALL(battery, level).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, onTimeout).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, start).InSequence(start_charging_behavior_sequence);

	rc.state_machine.process_event(lksm::event::ble_disconnection {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging, lksm::state::disconnected));
}

TEST_F(RobotControllerTest, stateFileExchangeEventDisconnectionGuardIsNotCharging)
{
	rc.state_machine.set_current_states(lksm::state::file_exchange, lksm::state::connected);

	auto returned_is_charging = false;

	// TODO (@yann): Trigger ble_disonnection in StateMachine from BLE and do not use process_event

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(returned_is_charging));

	Sequence on_file_exchange_end;
	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber()).InSequence(on_file_exchange_end);

	EXPECT_CALL(mock_videokit, stopVideo).Times(2);
	EXPECT_CALL(mock_motor_left, stop).Times(2);
	EXPECT_CALL(mock_motor_right, stop).Times(2);
	EXPECT_CALL(mock_ears, hide).Times(1);
	EXPECT_CALL(mock_belt, hide).Times(1);

	Sequence on_idle_sequence;
	EXPECT_CALL(timeout, onTimeout).InSequence(on_idle_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_idle_sequence);
	EXPECT_CALL(mock_videokit, playVideoOnRepeat).InSequence(on_idle_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(on_idle_sequence);

	rc.state_machine.process_event(lksm::event::ble_disconnection {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle, lksm::state::disconnected));
}

TEST_F(RobotControllerTest, stateFileExchangeEventEmergencyStop)
{
	rc.state_machine.set_current_states(lksm::state::file_exchange);

	Sequence on_file_exchange_end;
	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber()).InSequence(on_file_exchange_end);

	EXPECT_CALL(mock_motor_left, stop).Times(2);
	EXPECT_CALL(mock_motor_right, stop).Times(2);
	EXPECT_CALL(mock_belt, hide).Times(1);
	EXPECT_CALL(mock_ears, hide).Times(1);
	EXPECT_CALL(mock_lcd, turnOff).Times(1);
	EXPECT_CALL(mock_videokit, stopVideo).Times(2);

	rc.onMagicCardAvailable(MagicCard::emergency_stop);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::emergency_stopped));
}
