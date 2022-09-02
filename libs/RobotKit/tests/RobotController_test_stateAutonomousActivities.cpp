// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, stateGameConnectedEventCommandReceived)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities, lksm::state::connected);

	Sequence on_game_exit_sequence;
	EXPECT_CALL(mock_videokit, stopVideo).InSequence(on_game_exit_sequence);
	EXPECT_CALL(mock_motor_left, stop).InSequence(on_game_exit_sequence);
	EXPECT_CALL(mock_motor_right, stop).InSequence(on_game_exit_sequence);

	Sequence on_working_entry_sequence;
	EXPECT_CALL(timeout, onTimeout).InSequence(on_working_entry_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(on_working_entry_sequence);

	rc.state_machine.process_event(lksm::event::command_received {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working, lksm::state::connected));
}

TEST_F(RobotControllerTest, stateGameDisconnectedEventCommandReceived)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities, lksm::state::disconnected);

	rc.state_machine.process_event(lksm::event::command_received {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::autonomous_activities, lksm::state::disconnected));
}

TEST_F(RobotControllerTest, stateGameEventBleConnection)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities, lksm::state::disconnected);

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

	rc.state_machine.process_event(lksm::event::ble_connection {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working, lksm::state::connected));
}

TEST_F(RobotControllerTest, stateGameEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

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
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::autonomous_activities));
}

TEST_F(RobotControllerTest, stateGameEventAutonomousActivityRequested)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	EXPECT_CALL(mock_videokit, stopVideo).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_left, stop).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_right, stop).Times(AtLeast(1));
	EXPECT_CALL(mock_videokit, displayImage).Times(1);

	rc.onMagicCardAvailable(MagicCard::remote_standard);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::autonomous_activities));
}

TEST_F(RobotControllerTest, stateGameActivityDisplayTagsChosen)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	const std::unordered_map<MagicCard, interface::Activity *> activities {{MagicCard::number_10, &display_tag}};

	activitykit.registerActivities(activities);

	EXPECT_CALL(mock_videokit, displayImage).Times(1);

	rc.onMagicCardAvailable(MagicCard::number_10);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::autonomous_activities));
}
