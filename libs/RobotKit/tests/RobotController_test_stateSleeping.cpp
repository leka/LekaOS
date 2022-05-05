// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, stateSleepingEventCommandReceived)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	// TODO: Trigger command_received in StateMachine from BLE

	// EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

TEST_F(RobotControllerTest, stateSleepingEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));

	Sequence on_exit_sleeping_sequence;
	EXPECT_CALL(timeout, stop).InSequence(on_exit_sleeping_sequence);
	EXPECT_CALL(mock_videokit, stopVideo).InSequence(on_exit_sleeping_sequence);
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

TEST_F(RobotControllerTest, stateSleepingEventChargeDidStartGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::sleeping);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));

	on_charge_did_start();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::sleeping));
}
