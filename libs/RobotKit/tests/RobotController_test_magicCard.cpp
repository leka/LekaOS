// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

// ! TODO: Refactor with composite SM & CoreTimer mock

TEST_F(RobotControllerTest, onMagicCardAvailableEmergencyStopDelayNotOver)
{
	expectedCallsResetAutonomousActivitiesTimeout();

	rc.onMagicCardAvailable(MagicCard::emergency_stop);
}

TEST_F(RobotControllerTest, onMagicCardAvailableEmergencyStopDelayOver)
{
	auto delay_over = 11s;

	expectedCallsResetAutonomousActivitiesTimeout();

	{
		EXPECT_CALL(mock_lcd, turnOff);

		EXPECT_CALL(mock_ears, hide).Times(AnyNumber());
		EXPECT_CALL(mock_belt, hide).Times(AnyNumber());
		EXPECT_CALL(mock_ledkit, stop).Times(AnyNumber());
		EXPECT_CALL(mock_videokit, stopVideo).Times(AnyNumber());
		EXPECT_CALL(mock_motor_left, stop).Times(AnyNumber());
		EXPECT_CALL(mock_motor_right, stop).Times(AnyNumber());

		EXPECT_CALL(timeout_state_internal, stop).Times(AnyNumber());
		EXPECT_CALL(timeout_state_transition, stop).Times(AnyNumber());
	}	// TODO: Expect raise call

	spy_kernel_addElapsedTimeToTickCount(delay_over);

	rc.onMagicCardAvailable(MagicCard::emergency_stop);
}

TEST_F(RobotControllerTest, onMagicCardAvailableDiceRollNotInAutonomousActivityDelayNotOver)
{
	expectedCallsResetAutonomousActivitiesTimeout();

	auto delay_not_over = 1000ms;
	spy_kernel_addElapsedTimeToTickCount(delay_not_over);

	rc.onMagicCardAvailable(MagicCard::dice_roll);
}

TEST_F(RobotControllerTest, onMagicCardAvailableDiceRollNotInAutonomousActivityDelayOver)
{
	expectedCallsResetAutonomousActivitiesTimeout();

	auto delay_over = 1001ms;
	spy_kernel_addElapsedTimeToTickCount(delay_over);

	{
		EXPECT_CALL(mock_ears, hide).Times(AnyNumber());
		EXPECT_CALL(mock_belt, hide).Times(AnyNumber());
		EXPECT_CALL(mock_ledkit, stop).Times(AnyNumber());
		EXPECT_CALL(mock_videokit, stopVideo).Times(AnyNumber());
		EXPECT_CALL(mock_motor_left, stop).Times(AnyNumber());
		EXPECT_CALL(mock_motor_right, stop).Times(AnyNumber());

		EXPECT_CALL(timeout_state_internal, stop).Times(AnyNumber());
		EXPECT_CALL(timeout_state_transition, stop).Times(AnyNumber());

		EXPECT_CALL(mock_videokit, displayImage).Times(AnyNumber());
	}	// TODO: Expect raise call

	rc.onMagicCardAvailable(MagicCard::dice_roll);
}

TEST_F(RobotControllerTest, onMagicCardAvailableDiceRollInAutonomousActivityIsNotPlayingDelayNotOver)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	expectedCallsResetAutonomousActivitiesTimeout();

	auto delay_not_over = 2000ms;
	spy_kernel_addElapsedTimeToTickCount(delay_not_over);

	rc.onMagicCardAvailable(MagicCard::dice_roll);
}

TEST_F(RobotControllerTest, onMagicCardAvailableDiceRollInAutonomousActivityIsNotPlayingDelayOver)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	// expectedCallsResetAutonomousActivitiesTimeout();

	auto delay_over = 2001ms;
	spy_kernel_addElapsedTimeToTickCount(delay_over);

	{
		EXPECT_CALL(timeout_autonomous_activities, stop).Times(AnyNumber());
		EXPECT_CALL(timeout_autonomous_activities, onTimeout).Times(AnyNumber());
		EXPECT_CALL(timeout_autonomous_activities, start).Times(AnyNumber());

		EXPECT_CALL(mock_ears, hide).Times(AnyNumber());
		EXPECT_CALL(mock_belt, hide).Times(AnyNumber());
		EXPECT_CALL(mock_ledkit, stop).Times(AnyNumber());
		EXPECT_CALL(mock_videokit, stopVideo).Times(AnyNumber());
		EXPECT_CALL(mock_motor_left, stop).Times(AnyNumber());
		EXPECT_CALL(mock_motor_right, stop).Times(AnyNumber());

		EXPECT_CALL(timeout_state_internal, stop).Times(AnyNumber());
		EXPECT_CALL(timeout_state_transition, stop).Times(AnyNumber());

		EXPECT_CALL(mock_videokit, displayImage).Times(AnyNumber());

		EXPECT_CALL(timeout_state_transition, onTimeout).Times(AnyNumber());
		EXPECT_CALL(timeout_state_transition, start).Times(AnyNumber());
		EXPECT_CALL(mock_videokit, playVideoOnRepeat);
		EXPECT_CALL(mock_lcd, turnOn);
	}	// TODO: Expect raise call

	rc.onMagicCardAvailable(MagicCard::dice_roll);
}

TEST_F(RobotControllerTest, onMagicCardAvailableDiceRollInAutonomousActivityIsPlayingDelayNotOver)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	expectedCallsResetAutonomousActivitiesTimeout();

	auto set_activitykit_is_playing = [this] {
		const std::unordered_map<MagicCard, interface::Activity *> activities {{MagicCard::number_10, &display_tag}};
		activitykit.registerActivities(activities);
		EXPECT_CALL(mock_videokit, displayImage);
		activitykit.start(MagicCard::number_10);
	};
	set_activitykit_is_playing();

	auto delay_not_over = 1000ms;
	spy_kernel_addElapsedTimeToTickCount(delay_not_over);

	rc.onMagicCardAvailable(MagicCard::dice_roll);
}

TEST_F(RobotControllerTest, onMagicCardAvailableDiceRollInAutonomousActivityIsPlayingDelayOver)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	// expectedCallsResetAutonomousActivitiesTimeout();

	auto set_activitykit_is_playing = [this] {
		const std::unordered_map<MagicCard, interface::Activity *> activities {{MagicCard::number_10, &display_tag}};
		activitykit.registerActivities(activities);
		EXPECT_CALL(mock_videokit, displayImage);
		activitykit.start(MagicCard::number_10);
	};
	set_activitykit_is_playing();

	auto delay_over = 1001ms;
	spy_kernel_addElapsedTimeToTickCount(delay_over);

	{
		EXPECT_CALL(timeout_autonomous_activities, stop).Times(AnyNumber());
		EXPECT_CALL(timeout_autonomous_activities, onTimeout).Times(AnyNumber());
		EXPECT_CALL(timeout_autonomous_activities, start).Times(AnyNumber());

		EXPECT_CALL(mock_ears, hide).Times(AnyNumber());
		EXPECT_CALL(mock_belt, hide).Times(AnyNumber());
		EXPECT_CALL(mock_ledkit, stop).Times(AnyNumber());
		EXPECT_CALL(mock_videokit, stopVideo).Times(AnyNumber());
		EXPECT_CALL(mock_motor_left, stop).Times(AnyNumber());
		EXPECT_CALL(mock_motor_right, stop).Times(AnyNumber());

		EXPECT_CALL(timeout_state_internal, stop).Times(AnyNumber());
		EXPECT_CALL(timeout_state_transition, stop).Times(AnyNumber());

		EXPECT_CALL(mock_videokit, displayImage).Times(AnyNumber());
	}	// TODO: Expect raise call

	rc.onMagicCardAvailable(MagicCard::dice_roll);
}

TEST_F(RobotControllerTest, onMagicCardAvailableOtherNotInAutonomousActivityIsNotPlaying)
{
	expectedCallsResetAutonomousActivitiesTimeout();

	rc.onMagicCardAvailable(MagicCard::number_10);
}

TEST_F(RobotControllerTest, onMagicCardAvailableOtherNotInAutonomousActivityIsPlaying)
{
	expectedCallsResetAutonomousActivitiesTimeout();

	auto set_activitykit_is_playing = [this] {
		const std::unordered_map<MagicCard, interface::Activity *> activities {{MagicCard::number_10, &display_tag}};
		activitykit.registerActivities(activities);
		EXPECT_CALL(mock_videokit, displayImage);
		activitykit.start(MagicCard::number_10);
	};
	set_activitykit_is_playing();

	rc.onMagicCardAvailable(MagicCard::number_10);
}

TEST_F(RobotControllerTest, onMagicCardAvailableOtherInAutonomousActivityIsPlaying)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	expectedCallsResetAutonomousActivitiesTimeout();

	auto set_activitykit_is_playing = [this] {
		const std::unordered_map<MagicCard, interface::Activity *> activities {{MagicCard::number_10, &display_tag}};
		activitykit.registerActivities(activities);
		EXPECT_CALL(mock_videokit, displayImage);
		activitykit.start(MagicCard::number_10);
	};
	set_activitykit_is_playing();

	rc.onMagicCardAvailable(MagicCard::number_10);
}

TEST_F(RobotControllerTest, onMagicCardAvailableOtherInAutonomousActivityIsNotPlaying)
{
	rc.state_machine.set_current_states(lksm::state::autonomous_activities);

	expectedCallsResetAutonomousActivitiesTimeout();

	rc.onMagicCardAvailable(MagicCard::number_10);
}

TEST_F(RobotControllerTest, raiseEmergencyStop)
{
	{
		EXPECT_CALL(mock_lcd, turnOff);

		EXPECT_CALL(mock_ears, hide).Times(AnyNumber());
		EXPECT_CALL(mock_belt, hide).Times(AnyNumber());
		EXPECT_CALL(mock_ledkit, stop).Times(AnyNumber());
		EXPECT_CALL(mock_videokit, stopVideo).Times(AnyNumber());
		EXPECT_CALL(mock_motor_left, stop).Times(AnyNumber());
		EXPECT_CALL(mock_motor_right, stop).Times(AnyNumber());

		EXPECT_CALL(timeout_state_internal, stop).Times(AnyNumber());
		EXPECT_CALL(timeout_state_transition, stop).Times(AnyNumber());
		EXPECT_CALL(timeout_autonomous_activities, stop).Times(AnyNumber());
	}	// TODO: Expect raise call

	rc.raiseEmergencyStop();
}

TEST_F(RobotControllerTest, raiseEmergencyStopReboot)
{
	rc.state_machine.set_current_states(lksm::state::emergency_stopped);

	auto min_calls_to_reboot = 7;
	for (int i = 0; i < min_calls_to_reboot; i++) {
		rc.raiseEmergencyStop();
	}

	// ? Should expect "system_reset()" call
}

TEST_F(RobotControllerTest, resetEmergencyStopCounter)
{
	rc.resetEmergencyStopCounter();

	// nothing expected
}

TEST_F(RobotControllerTest, startAutonomousActivityMode)
{
	EXPECT_CALL(mock_videokit, displayImage).Times(1);
	EXPECT_CALL(mock_lcd, turnOn);

	rc.startAutonomousActivityMode();
}

TEST_F(RobotControllerTest, stopAutonomousActivityMode)
{
	EXPECT_CALL(timeout_autonomous_activities, stop);
	{
		EXPECT_CALL(mock_ledkit, stop);
		EXPECT_CALL(mock_videokit, stopVideo);
		EXPECT_CALL(mock_motor_left, stop);
		EXPECT_CALL(mock_motor_right, stop);
	}	// TODO: Replace with EXPECT_CALL(mock_behaviorkit, stop);

	rc.stopAutonomousActivityMode();
}

TEST_F(RobotControllerTest, raiseAutonomousActivityModeRequested)
{
	{
		EXPECT_CALL(mock_ears, hide).Times(AnyNumber());
		EXPECT_CALL(mock_belt, hide).Times(AnyNumber());
		EXPECT_CALL(mock_ledkit, stop).Times(AnyNumber());
		EXPECT_CALL(mock_videokit, stopVideo).Times(AnyNumber());
		EXPECT_CALL(mock_motor_left, stop).Times(AnyNumber());
		EXPECT_CALL(mock_motor_right, stop).Times(AnyNumber());

		EXPECT_CALL(timeout_state_internal, stop).Times(AnyNumber());
		EXPECT_CALL(timeout_state_transition, stop).Times(AnyNumber());
		EXPECT_CALL(timeout_autonomous_activities, stop).Times(AnyNumber());

		EXPECT_CALL(mock_videokit, displayImage).Times(AnyNumber());
	}	// TODO: Expect raise call

	rc.raiseAutonomousActivityModeRequested();
}

TEST_F(RobotControllerTest, raiseAutonomousActivityModeExited)
{
	rc.raiseAutonomousActivityModeExited();

	// TODO: Expect raise call
}

TEST_F(RobotControllerTest, resetAutonomousActivitiesTimeout)
{
	auto expected_duration = 600s;

	Sequence seq;
	EXPECT_CALL(timeout_autonomous_activities, stop).InSequence(seq);
	EXPECT_CALL(timeout_autonomous_activities, onTimeout)
		.InSequence(seq)
		.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_autonomous_activities_timeout));
	EXPECT_CALL(timeout_autonomous_activities, start(std::chrono::microseconds {expected_duration})).InSequence(seq);

	rc.resetAutonomousActivitiesTimeout();
}

TEST_F(RobotControllerTest, onAutonomousActivitiesTimeout)
{
	EXPECT_CALL(timeout_autonomous_activities, stop);
	EXPECT_CALL(timeout_autonomous_activities, onTimeout)
		.WillOnce(GetCallback<interface::Timeout::callback_t>(&on_autonomous_activities_timeout));
	EXPECT_CALL(timeout_autonomous_activities, start);
	rc.resetAutonomousActivitiesTimeout();

	on_autonomous_activities_timeout();

	// TODO: Expect raise call
}

TEST_F(RobotControllerTest, onTagActivated)
{
	auto onTagActivated = rfidkit.getCallback();

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(2);

	expectedCallsResetAutonomousActivitiesTimeout();

	onTagActivated(MagicCard::none);
}
