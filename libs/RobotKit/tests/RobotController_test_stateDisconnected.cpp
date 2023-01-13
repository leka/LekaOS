// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, startDisconnectionBehaviorCharging)
{
	expectedCallsStopActuators();

	EXPECT_CALL(battery, isCharging).WillOnce(Return(true));
	EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::blink_on_charge))).Times(1);

	rc.startDisconnectionBehavior();
	EXPECT_FALSE(rc.isBleConnected());
}

TEST_F(RobotControllerTest, startDisconnectionBehaviorNotCharging)
{
	expectedCallsStopActuators();

	EXPECT_CALL(battery, isCharging).WillOnce(Return(false));
	EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::blink_on_charge))).Times(0);

	rc.startDisconnectionBehavior();
	EXPECT_FALSE(rc.isBleConnected());
}
