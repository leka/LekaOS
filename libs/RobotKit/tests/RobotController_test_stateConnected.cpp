// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, startConnectionBehaviorIsCharging)
{
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));

	EXPECT_CALL(mock_videokit, stopVideo).Times(AtLeast(1));
	EXPECT_CALL(mock_belt, hide).Times(AtLeast(1));
	EXPECT_CALL(mock_ears, hide).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_left, stop).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_right, stop).Times(AtLeast(1));

	EXPECT_CALL(mock_videokit, playVideoOnce).Times(0);
	EXPECT_CALL(mock_belt, setColor).Times(AtLeast(1));
	EXPECT_CALL(mock_belt, show).Times(AtLeast(1));
	rc.startConnectionBehavior();
	EXPECT_TRUE(rc.isBleConnected());
}

TEST_F(RobotControllerTest, startConnectionBehaviorIsNotCharging)
{
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	EXPECT_CALL(mock_videokit, stopVideo).Times(AtLeast(1));
	EXPECT_CALL(mock_belt, hide).Times(AtLeast(1));
	EXPECT_CALL(mock_ears, hide).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_left, stop).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_right, stop).Times(AtLeast(1));

	EXPECT_CALL(mock_videokit, playVideoOnce).Times(1);
	EXPECT_CALL(mock_belt, setColor).Times(AtLeast(1));
	EXPECT_CALL(mock_belt, show).Times(AtLeast(1));

	rc.startConnectionBehavior();
	EXPECT_TRUE(rc.isBleConnected());
}