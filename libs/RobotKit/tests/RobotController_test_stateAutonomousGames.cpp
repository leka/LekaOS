// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"
#include "SMAutonomousGame.h"

TEST_F(RobotControllerTest, stateGameConnectedEventCommandReceivedIsChargingFalse)
{
	rc.state_machine.set_current_states(boost::sml::state<game::SMAutonomousGame>, lksm::state::connected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	Sequence on_game_exit_sequence;
	EXPECT_CALL(mock_videokit, stopVideo).InSequence(on_game_exit_sequence);
	EXPECT_CALL(mock_motor_left, stop).InSequence(on_game_exit_sequence);
	EXPECT_CALL(mock_motor_right, stop).InSequence(on_game_exit_sequence);

	Sequence on_working_entry_sequence;
	EXPECT_CALL(timeout, onTimeout).InSequence(on_working_entry_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(on_working_entry_sequence);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AtLeast(1));

	rc.state_machine.process_event(lksm::event::command_received {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working, lksm::state::connected));
}

TEST_F(RobotControllerTest, stateGameDisconnectedEventCommandReceivedIsChargingFalse)
{
	rc.state_machine.set_current_states(boost::sml::state<game::SMAutonomousGame>, lksm::state::disconnected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AtLeast(1));

	rc.state_machine.process_event(lksm::event::command_received {});

	EXPECT_TRUE(rc.state_machine.is(boost::sml::state<game::SMAutonomousGame>, lksm::state::disconnected));
}

TEST_F(RobotControllerTest, stateGameEventBleConnectionGuardIsNotCharging)
{
	rc.state_machine.set_current_states(boost::sml::state<game::SMAutonomousGame>, lksm::state::disconnected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	EXPECT_CALL(mock_videokit, stopVideo).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_left, stop).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_right, stop).Times(AtLeast(1));

	EXPECT_CALL(mock_belt, setColor).Times(AtLeast(1));
	EXPECT_CALL(mock_belt, show).Times(AtLeast(1));
	EXPECT_CALL(mock_videokit, playVideoOnce).Times(1);

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

TEST_F(RobotControllerTest, stateGameEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(boost::sml::state<game::SMAutonomousGame>);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));

	Sequence on_game_exit_sequence;
	EXPECT_CALL(mock_videokit, stopVideo).InSequence(on_game_exit_sequence);
	EXPECT_CALL(mock_motor_left, stop).InSequence(on_game_exit_sequence);
	EXPECT_CALL(mock_motor_right, stop).InSequence(on_game_exit_sequence);

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

TEST_F(RobotControllerTest, stateGameEventChargeDidStartGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(boost::sml::state<game::SMAutonomousGame>);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(boost::sml::state<game::SMAutonomousGame>));
}

TEST_F(RobotControllerTest, stateGameConnectedEventCommandReceivedGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(boost::sml::state<game::SMAutonomousGame>, lksm::state::connected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));

	Sequence on_game_exit_sequence;
	EXPECT_CALL(mock_videokit, stopVideo).InSequence(on_game_exit_sequence);
	EXPECT_CALL(mock_motor_left, stop).InSequence(on_game_exit_sequence);
	EXPECT_CALL(mock_motor_right, stop).InSequence(on_game_exit_sequence);

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

TEST_F(RobotControllerTest, stateGameDisconnectedEventCommandReceivedGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(boost::sml::state<game::SMAutonomousGame>, lksm::state::disconnected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AtLeast(1));

	rc.state_machine.process_event(lksm::event::command_received {});

	EXPECT_TRUE(rc.state_machine.is(boost::sml::state<game::SMAutonomousGame>, lksm::state::disconnected));
}

TEST_F(RobotControllerTest, stateGameEventBleConnectionGuardIsCharging)
{
	rc.state_machine.set_current_states(boost::sml::state<game::SMAutonomousGame>, lksm::state::disconnected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));

	EXPECT_CALL(mock_videokit, stopVideo).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_left, stop).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_right, stop).Times(AtLeast(1));

	Sequence start_charging_behavior_sequence;
	EXPECT_CALL(battery, level).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, onTimeout).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, start).InSequence(start_charging_behavior_sequence);

	EXPECT_CALL(mock_ears, hide).Times(1);
	EXPECT_CALL(mock_belt, hide).Times(1);
	EXPECT_CALL(mock_belt, setColor).Times(AtLeast(1));
	EXPECT_CALL(mock_belt, show).Times(AtLeast(1));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AtLeast(1));

	rc.state_machine.process_event(lksm::event::ble_connection {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateGameEventAutonomousGameTransition)
{
	rc.state_machine.set_current_states(boost::sml::state<game::SMAutonomousGame>);

	EXPECT_CALL(mock_videokit, stopVideo).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_left, stop).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_right, stop).Times(AtLeast(1));
	EXPECT_CALL(mock_videokit, displayImage).Times(1);

	rc.raiseAutonomousGameTransition();

	EXPECT_TRUE(rc.state_machine.is(boost::sml::state<game::SMAutonomousGame>));
}
