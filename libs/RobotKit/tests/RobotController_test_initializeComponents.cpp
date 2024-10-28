// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, initialization)
{
	EXPECT_NE(&rc, nullptr);
}

TEST_F(RobotControllerTest, initializeComponents)
{
	EXPECT_CALL(mock_mcu, getID).Times(AtLeast(1));

	EXPECT_CALL(firmware_update, getCurrentVersion);

	EXPECT_CALL(mbed_mock_gap, setEventHandler);
	EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload);

	EXPECT_CALL(mbed_mock_gatt, addService).Times(8);
	EXPECT_CALL(mbed_mock_gatt, setEventHandler);
	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber());

	EXPECT_CALL(mock_ledkit, init);

	EXPECT_CALL(mock_videokit, initializeScreen);
	EXPECT_CALL(mock_lcd, turnOff);
	EXPECT_CALL(mock_videokit, stopVideo);

	EXPECT_CALL(mock_motor_left, stop);
	EXPECT_CALL(mock_motor_right, stop);

	rc.initializeComponents();
}
