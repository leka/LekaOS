// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, startDisconnectionBehavior)
{
	expectedCallsStopActuators();

	rc.startDisconnectionBehavior();
	EXPECT_FALSE(rc.isBleConnected());
}
