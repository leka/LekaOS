// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, stateSleepingEventCommandReceived)
{
	rc.state_machine.set_current_states(lksm::state::sleeping, lksm::state::connected);

	Sequence on_exit_sleeping_sequence;
	EXPECT_CALL(timeout, stop).InSequence(on_exit_sleeping_sequence);
	EXPECT_CALL(mock_videokit, stopVideo).InSequence(on_exit_sleeping_sequence);
	expectedCallsStopMotors();

	Sequence on_working_entry_sequence;
	EXPECT_CALL(timeout, onTimeout).InSequence(on_working_entry_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(on_working_entry_sequence);

	rc.state_machine.process_event(lksm::event::command_received {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working, lksm::state::connected));
}

TEST_F(RobotControllerTest, stateSleepingEventBleConnection)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	Sequence on_exit_sleeping_sequence;
	EXPECT_CALL(timeout, stop).InSequence(on_exit_sleeping_sequence);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	EXPECT_CALL(mock_videokit, stopVideo).Times(AtLeast(1));
	EXPECT_CALL(mock_belt, hide).Times(AtLeast(1));
	EXPECT_CALL(mock_ears, hide).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_left, stop).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_right, stop).Times(AtLeast(1));

	EXPECT_CALL(mock_videokit, playVideoOnce).Times(AtLeast(1));
	EXPECT_CALL(mock_belt, setColor).Times(AtLeast(1));
	EXPECT_CALL(mock_belt, show).Times(AtLeast(1));

	Sequence on_working_entry_sequence;
	EXPECT_CALL(timeout, onTimeout).InSequence(on_working_entry_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(on_working_entry_sequence);

	rc.state_machine.process_event(lksm::event::ble_connection {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working));
}

TEST_F(RobotControllerTest, stateSleepingEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));

	Sequence on_exit_sleeping_sequence;
	EXPECT_CALL(timeout, stop).InSequence(on_exit_sleeping_sequence);
	EXPECT_CALL(mock_videokit, stopVideo).InSequence(on_exit_sleeping_sequence);
	expectedCallsStopMotors();

	Sequence start_charging_behavior_sequence;
	EXPECT_CALL(battery, level).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, onTimeout).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, start).InSequence(start_charging_behavior_sequence);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateSleepingEventChargeDidStartGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::sleeping));
}

TEST_F(RobotControllerTest, stateSleepingEventEmergencyStop)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	Sequence on_exit_sleeping_sequence;
	EXPECT_CALL(timeout, stop).InSequence(on_exit_sleeping_sequence);

	EXPECT_CALL(mock_motor_left, stop).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_right, stop).Times(AtLeast(1));
	EXPECT_CALL(mock_belt, hide).Times(1);
	EXPECT_CALL(mock_ears, hide).Times(1);
	EXPECT_CALL(mock_lcd, turnOff).Times(1);
	EXPECT_CALL(mock_videokit, stopVideo).Times(AtLeast(1));

	rc.raiseEmergencyStop();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::emergency_stopped));
}

TEST_F(RobotControllerTest, stateSleepingEventAutonomousGameTransition)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	Sequence on_exit_sleeping_sequence;
	EXPECT_CALL(timeout, stop).InSequence(on_exit_sleeping_sequence);
	EXPECT_CALL(mock_videokit, stopVideo).InSequence(on_exit_sleeping_sequence);
	expectedCallsStopMotors();

	EXPECT_CALL(mock_videokit, displayImage).Times(1);

	rc.raiseAutonomousGameTransition();

	EXPECT_TRUE(rc.state_machine.is(boost::sml::state<game::SMAutonomousGame>));
}
