// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, stateSetupEventSetupCompleteGuardIsChargingFalse)
{
	rc.state_machine.set_current_states(lksm::state::setup);

	spy_isCharging_return_value = false;
	expectedCallsRegisterEvents();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::idle));
}

TEST_F(RobotControllerTest, stateSetupEventSetupCompleteGuardIsChargingTrue)
{
	rc.state_machine.set_current_states(lksm::state::setup);

	spy_isCharging_return_value = true;
	expectedCallsRegisterEvents();

	EXPECT_TRUE(rc.state_machine.is(lksm::state::charging));
}
