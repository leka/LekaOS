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
	{
		InSequence seq;

		EXPECT_CALL(mbed_mock_gatt, addService).Times(7);
		EXPECT_CALL(mbed_mock_gap, setEventHandler).Times(1);
		EXPECT_CALL(mbed_mock_gatt, setEventHandler).Times(1);

		EXPECT_CALL(mock_mcu, getID).Times(1);
		// TODO: Specify which BLE service and what is expected if necessary
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(1);

		EXPECT_CALL(firmware_update, getCurrentVersion).Times(1);
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(1);

		Sequence set_serial_number_as_ble_device_name;
		EXPECT_CALL(mock_mcu, getID).InSequence(set_serial_number_as_ble_device_name);
		EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload).InSequence(set_serial_number_as_ble_device_name);

		expectedCallsStopMotors();

		EXPECT_CALL(mock_ledkit, init).Times(1);

		EXPECT_CALL(mock_videokit, initializeScreen).Times(1);
		EXPECT_CALL(mock_lcd, turnOff).Times(1);
		EXPECT_CALL(mock_videokit, stopVideo).Times(1);
	}

	rc.initializeComponents();
}
