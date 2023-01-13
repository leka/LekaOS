// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, startConnectionBehaviorIsCharging)
{
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));

	expectedCallsStopActuators();
	Sequence on_ble_connection_sequence;
	EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::ble_connection)))
		.Times(1)
		.InSequence(on_ble_connection_sequence);
	EXPECT_CALL(mock_videokit, playVideoOnce).Times(0).InSequence(on_ble_connection_sequence);
	EXPECT_CALL(mock_lcd, turnOn).Times(0).InSequence(on_ble_connection_sequence);
	EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::blink_on_charge)))
		.Times(1)
		.InSequence(on_ble_connection_sequence);

	rc.startConnectionBehavior();
}

TEST_F(RobotControllerTest, startConnectionBehaviorIsNotCharging)
{
	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));

	expectedCallsStopActuators();
	Sequence on_ble_connection_sequence;
	EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::ble_connection)))
		.Times(1)
		.InSequence(on_ble_connection_sequence);
	EXPECT_CALL(mock_videokit, playVideoOnce).Times(1).InSequence(on_ble_connection_sequence);
	EXPECT_CALL(mock_lcd, turnOn).Times(1).InSequence(on_ble_connection_sequence);

	rc.startConnectionBehavior();
}
