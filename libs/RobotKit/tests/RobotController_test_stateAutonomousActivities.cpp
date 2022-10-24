// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, stateAutonomousActivityConnectedEventCommandReceived)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities, lksm::state::connected);

	Sequence on_autonomous_activity_exit_sequence;
	EXPECT_CALL(mock_videokit, stopVideo).InSequence(on_autonomous_activity_exit_sequence);
	EXPECT_CALL(mock_motor_left, stop).InSequence(on_autonomous_activity_exit_sequence);
	EXPECT_CALL(mock_motor_right, stop).InSequence(on_autonomous_activity_exit_sequence);

	Sequence on_working_entry_sequence;
	EXPECT_CALL(timeout, onTimeout).InSequence(on_working_entry_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(on_working_entry_sequence);

	rc.state_machine.process_event(lksm::event::command_received {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working, lksm::state::connected));
}

TEST_F(RobotControllerTest, stateAutonomousActivityDisconnectedEventCommandReceived)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities, lksm::state::disconnected);

	rc.state_machine.process_event(lksm::event::command_received {});

	EXPECT_TRUE(rc.state_machine.is(lksm::state::autonomous_activities, lksm::state::disconnected));
}

TEST_F(RobotControllerTest, stateAutonomousActivityEventBleConnection)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities, lksm::state::disconnected);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	EXPECT_CALL(mock_belt, hide).Times(AtLeast(1));
	EXPECT_CALL(mock_ears, hide).Times(AtLeast(1));
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

TEST_F(RobotControllerTest, stateAutonomousActivityEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));

	Sequence on_autonomous_activity_exit_sequence;
	EXPECT_CALL(mock_videokit, stopVideo).InSequence(on_autonomous_activity_exit_sequence);
	EXPECT_CALL(mock_motor_left, stop).InSequence(on_autonomous_activity_exit_sequence);
	EXPECT_CALL(mock_motor_right, stop).InSequence(on_autonomous_activity_exit_sequence);

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

TEST_F(RobotControllerTest, stateAutonomousActivityEventChargeDidStartGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::autonomous_activities));
}

TEST_F(RobotControllerTest, stateAutonomousActivityStartActivity)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	auto set_activitykit_is_playing = [this] {
		const std::unordered_map<MagicCard, interface::Activity *> activities {{MagicCard::number_10, &display_tag}};
		activitykit.registerActivities(activities);
		EXPECT_CALL(mock_videokit, displayImage).Times(1);
		activitykit.start(MagicCard::number_10);
	};
	set_activitykit_is_playing();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::autonomous_activities));
}

TEST_F(RobotControllerTest, stateAutonomousActivityStartActivityActivityAlreadyStarted)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	auto set_activitykit_is_playing = [this] {
		const std::unordered_map<MagicCard, interface::Activity *> activities {{MagicCard::number_10, &display_tag}};
		activitykit.registerActivities(activities);
		EXPECT_CALL(mock_videokit, displayImage).Times(1);
		activitykit.start(MagicCard::number_10);
	};
	set_activitykit_is_playing();

	EXPECT_CALL(mock_videokit, displayImage).Times(0);

	rc.onMagicCardAvailable(MagicCard::number_10);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::autonomous_activities));
}

TEST_F(RobotControllerTest, stateAutonomousActivityActivityStartedBackToMenuDelayNotOver)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	auto maximal_delay_before_over = 1s;

	auto set_activitykit_is_playing = [this] {
		const std::unordered_map<MagicCard, interface::Activity *> activities {{MagicCard::number_10, &display_tag}};
		activitykit.registerActivities(activities);
		EXPECT_CALL(mock_videokit, displayImage).Times(1);
		activitykit.start(MagicCard::number_10);
	};
	set_activitykit_is_playing();

	EXPECT_CALL(mock_videokit, displayImage).Times(0);

	spy_kernel_addElapsedTimeToTickCount(maximal_delay_before_over);
	rc.onMagicCardAvailable(MagicCard::dice_roll);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::autonomous_activities));
}

// ! TODO: Refactor with composite SM & CoreTimer mock
TEST_F(RobotControllerTest, stateAutonomousActivityActivityStartedBackToMenu)
{
	auto minimal_delay_over = 1001ms;

	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	auto set_activitykit_is_playing = [this] {
		const std::unordered_map<MagicCard, interface::Activity *> activities {{MagicCard::number_10, &display_tag}};
		activitykit.registerActivities(activities);
		EXPECT_CALL(mock_videokit, displayImage).Times(1);
		activitykit.start(MagicCard::number_10);
	};
	set_activitykit_is_playing();

	EXPECT_CALL(mock_videokit, stopVideo);
	expectedCallsStopMotors();

	EXPECT_CALL(mock_videokit, displayImage).Times(1);

	spy_kernel_addElapsedTimeToTickCount(minimal_delay_over);
	rc.onMagicCardAvailable(MagicCard::dice_roll);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::autonomous_activities));
}

// ! TODO: Refactor with composite SM & CoreTimer mock
TEST_F(RobotControllerTest, stateAutonomousActivityDiceRollDetectedDelayNotOver)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	auto maximal_delay_before_over = 2s;

	EXPECT_CALL(mock_videokit, stopVideo).Times(0);
	EXPECT_CALL(mock_motor_left, stop).Times(0);
	EXPECT_CALL(mock_motor_right, stop).Times(0);
	EXPECT_CALL(mock_videokit, displayImage).Times(0);

	spy_kernel_addElapsedTimeToTickCount(maximal_delay_before_over);
	rc.onMagicCardAvailable(MagicCard::dice_roll);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::autonomous_activities));
}

// ! TODO: Refactor with composite SM & CoreTimer mock
TEST_F(RobotControllerTest, stateAutonomousActivityDiceRollDetectedDelayOverEventAutonomousActivityExitedDisconnected)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities, lksm::state::disconnected);

	auto minimal_delay_over = 2001ms;

	EXPECT_CALL(mock_videokit, stopVideo).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_left, stop).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_right, stop).Times(AtLeast(1));

	Sequence on_idle_entry_sequence;
	EXPECT_CALL(timeout, onTimeout).InSequence(on_idle_entry_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_idle_entry_sequence);
	EXPECT_CALL(mock_videokit, playVideoOnRepeat).InSequence(on_idle_entry_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(on_idle_entry_sequence);

	spy_kernel_addElapsedTimeToTickCount(minimal_delay_over);
	rc.onMagicCardAvailable(MagicCard::dice_roll);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

// ! TODO: Refactor with composite SM & CoreTimer mock
TEST_F(RobotControllerTest, stateAutonomousActivityDiceRollDetectedDelayOverEventAutonomousActivityExitedConnected)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities, lksm::state::connected);

	auto minimal_delay_over = 2001ms;

	EXPECT_CALL(mock_videokit, stopVideo).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_left, stop).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_right, stop).Times(AtLeast(1));

	Sequence on_working_entry_sequence;
	EXPECT_CALL(timeout, onTimeout).InSequence(on_working_entry_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_working_entry_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(on_working_entry_sequence);

	spy_kernel_addElapsedTimeToTickCount(minimal_delay_over);
	rc.onMagicCardAvailable(MagicCard::dice_roll);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working));
}
