// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, stateIdleEventTimeout)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	EXPECT_CALL(sleep_timeout, stop).Times(1);

	expectedCallsStopMotors();

	on_sleep_timeout();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::sleeping));
}

TEST_F(RobotControllerTest, stateIdleEventChargeDidStartGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::idle);

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));
	EXPECT_CALL(sleep_timeout, stop).Times(1);

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));
	expectedCallsStopMotors();

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
