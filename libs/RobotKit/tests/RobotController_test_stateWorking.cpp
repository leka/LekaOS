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

	Sequence on_charging_sequence;
	EXPECT_CALL(mock_lcd, turnOn).InSequence(on_charging_sequence);
	EXPECT_CALL(timeout, onTimeout).InSequence(on_charging_sequence);
	EXPECT_CALL(timeout, start).InSequence(on_charging_sequence);

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
