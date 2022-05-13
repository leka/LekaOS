// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, stateIdleEventTimeout)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	Sequence on_exit_idle_sequence;
	EXPECT_CALL(timeout, stop).InSequence(on_exit_idle_sequence);
	EXPECT_CALL(mock_videokit, stopVideo).InSequence(on_exit_idle_sequence);
	expectedCallsStopMotors();

	// TODO (@hugo) EXPECT_CALL(mock_videokit, playVideo).Times(X);

	on_sleep_timeout();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::screensaver));
}

TEST_F(RobotControllerTest, stateIdleEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));

	Sequence on_exit_idle_sequence;
	EXPECT_CALL(timeout, stop).InSequence(on_exit_idle_sequence);
	EXPECT_CALL(mock_videokit, stopVideo).InSequence(on_exit_idle_sequence);
	expectedCallsStopMotors();

	Sequence on_charging_sequence;
	EXPECT_CALL(mock_lcd, turnOn).InSequence(on_charging_sequence);
	EXPECT_CALL(timeout, onTimeout).InSequence(on_charging_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_charging_sequence);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}

TEST_F(RobotControllerTest, stateIdleEventChargeDidStartGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

TEST_F(RobotControllerTest, startConnectionBehaviorIsCharging)
{
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));

	EXPECT_CALL(mock_videokit, stopVideo).Times(2);
	EXPECT_CALL(mock_belt, hide).Times(1);
	EXPECT_CALL(mock_ears, hide).Times(1);
	EXPECT_CALL(mock_motor_left, stop).Times(2);
	EXPECT_CALL(mock_motor_right, stop).Times(2);

	EXPECT_CALL(mock_videokit, playVideo).Times(0);
	rc.startConnectionBehavior();
	EXPECT_TRUE(rc.isBleConnected());
}

TEST_F(RobotControllerTest, startConnectionBehaviorIsNotCharging)
{
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	EXPECT_CALL(mock_videokit, stopVideo).Times(2);
	EXPECT_CALL(mock_belt, hide).Times(1);
	EXPECT_CALL(mock_ears, hide).Times(1);
	EXPECT_CALL(mock_motor_left, stop).Times(2);
	EXPECT_CALL(mock_motor_right, stop).Times(2);

	EXPECT_CALL(mock_videokit, playVideo).Times(1);

	rc.startConnectionBehavior();
	EXPECT_TRUE(rc.isBleConnected());
}

TEST_F(RobotControllerTest, startDisconnectionBehavior)
{
	EXPECT_CALL(mock_videokit, stopVideo).Times(2);
	EXPECT_CALL(mock_belt, hide).Times(1);
	EXPECT_CALL(mock_ears, hide).Times(1);
	EXPECT_CALL(mock_motor_left, stop).Times(2);
	EXPECT_CALL(mock_motor_right, stop).Times(2);

	rc.startDisconnectionBehavior();
	EXPECT_FALSE(rc.isBleConnected());
}
