// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, startDisconnectionBehavior)
{
	EXPECT_CALL(mock_videokit, stopVideo).Times(2);
	EXPECT_CALL(mock_motor_left, stop).Times(2);
	EXPECT_CALL(mock_motor_right, stop).Times(2);
	EXPECT_CALL(mock_belt, hide).Times(1);
	EXPECT_CALL(mock_ears, hide).Times(1);

	rc.startDisconnectionBehavior();
	EXPECT_FALSE(rc.isBleConnected());
}
