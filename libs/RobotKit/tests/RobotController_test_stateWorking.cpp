// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, stateWorkingEventTimeout)
{
	rc.state_machine.set_current_states(lksm::state::working);

	Sequence on_exit_working_sequence;
	EXPECT_CALL(timeout, stop).InSequence(on_exit_working_sequence);

	Sequence on_idle_sequence;
	EXPECT_CALL(timeout, onTimeout).InSequence(on_idle_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_idle_sequence);
	EXPECT_CALL(mock_videokit, playVideoOnRepeat).InSequence(on_idle_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(on_idle_sequence);

	on_idle_timeout();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

TEST_F(RobotControllerTest, stateWorkingEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::working);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));

	Sequence on_exit_working_sequence;
	EXPECT_CALL(timeout, stop).InSequence(on_exit_working_sequence);

	Sequence start_charging_behavior_sequence;
	EXPECT_CALL(battery, level).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_videokit, displayImage).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_ledkit, start).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(mock_lcd, turnOn).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, onTimeout).InSequence(start_charging_behavior_sequence);
	EXPECT_CALL(timeout, start).InSequence(start_charging_behavior_sequence);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateWorkingEventChargeDidStartGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::working);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working));
}

TEST_F(RobotControllerTest, stateWorkingEventEmergencyStopDelayNotOver)
{
	rc.state_machine.set_current_states(lksm::state::working);

	auto maximal_delay_before_over = 10s;

	spy_kernel_addElapsedTimeToTickCount(maximal_delay_before_over);
	rc.onMagicCardAvailable(MagicCard::emergency_stop);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working));
}

TEST_F(RobotControllerTest, stateWorkingEventEmergencyStopDelayOver)
{
	rc.state_machine.set_current_states(lksm::state::working);

	auto delay_over = 11s;

	Sequence on_exit_working_sequence;
	EXPECT_CALL(timeout, stop).InSequence(on_exit_working_sequence);

	EXPECT_CALL(mock_motor_left, stop).Times(2);
	EXPECT_CALL(mock_motor_right, stop).Times(2);
	EXPECT_CALL(mock_belt, hide).Times(1);
	EXPECT_CALL(mock_ears, hide).Times(1);
	EXPECT_CALL(mock_lcd, turnOff).Times(1);
	EXPECT_CALL(mock_videokit, stopVideo).Times(2);

	spy_kernel_addElapsedTimeToTickCount(delay_over);
	rc.onMagicCardAvailable(MagicCard::emergency_stop);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::emergency_stopped));
}

TEST_F(RobotControllerTest, stateWorkingDiceRollDetectedDelayNotOver)
{
	rc.state_machine.set_current_states(lksm::state::working);

	auto maximal_delay_before_over = 1s;

	EXPECT_CALL(mock_videokit, displayImage).Times(0);

	spy_kernel_addElapsedTimeToTickCount(maximal_delay_before_over);
	rc.onMagicCardAvailable(MagicCard::dice_roll);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working));
}

// ! TODO: Refactor with composite SM & CoreTimer mock
TEST_F(RobotControllerTest, stateWorkingDiceRollDetectedDelayOverEventAutonomousActivityRequested)
{
	rc.state_machine.set_current_states(lksm::state::working);

	auto minimal_delay_over = 1001ms;

	Sequence on_exit_working_sequence;
	EXPECT_CALL(timeout, stop).InSequence(on_exit_working_sequence);

	EXPECT_CALL(mock_videokit, displayImage).Times(1);

	spy_kernel_addElapsedTimeToTickCount(minimal_delay_over);
	rc.onMagicCardAvailable(MagicCard::dice_roll);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::autonomous_activities));
}

// ! TODO: Refactor with composite SM & CoreTimer mock
TEST_F(RobotControllerTest, stateWorkingImpossibleSituationActivityStarted)
{
	rc.state_machine.set_current_states(lksm::state::working);

	auto maximal_delay_before_over = 1s;

	auto set_activitykit_is_playing = [this] {
		const std::unordered_map<MagicCard, interface::Activity *> activities {{MagicCard::number_10, &display_tag}};
		activitykit.registerActivities(activities);
		EXPECT_CALL(mock_videokit, displayImage).Times(1);
		activitykit.start(MagicCard::number_10);
	};
	set_activitykit_is_playing();

	spy_kernel_addElapsedTimeToTickCount(maximal_delay_before_over);
	rc.onMagicCardAvailable(MagicCard::number_0);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working));
}

// ! TODO: Refactor with composite SM & CoreTimer mock
TEST_F(RobotControllerTest, stateWorkingActivityStartedNotPlaying)
{
	rc.state_machine.set_current_states(lksm::state::working);

	auto maximal_delay_before_over = 1s;

	spy_kernel_addElapsedTimeToTickCount(maximal_delay_before_over);
	rc.onMagicCardAvailable(MagicCard::number_0);

	EXPECT_TRUE(rc.state_machine.is(lksm::state::working));
}
