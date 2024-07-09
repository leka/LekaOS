// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"
#include "mocks/leka/DeepSleepEnabled.h"

TEST_F(RobotControllerTest, stopActuators)
{
	EXPECT_CALL(mock_ears, hide).Times(AnyNumber());
	EXPECT_CALL(mock_belt, hide).Times(AnyNumber());
	EXPECT_CALL(mock_ledkit, stop).Times(AnyNumber());
	EXPECT_CALL(mock_videokit, stopVideo).Times(AnyNumber());
	EXPECT_CALL(mock_motor_left, stop).Times(AnyNumber());
	EXPECT_CALL(mock_motor_right, stop).Times(AnyNumber());

	rc.stopActuators();
}

TEST_F(RobotControllerTest, stopActuatorsAndLcd)
{
	EXPECT_CALL(mock_lcd, turnOff);

	EXPECT_CALL(mock_ears, hide).Times(AnyNumber());
	EXPECT_CALL(mock_belt, hide).Times(AnyNumber());
	EXPECT_CALL(mock_ledkit, stop).Times(AnyNumber());
	EXPECT_CALL(mock_videokit, stopVideo).Times(AnyNumber());
	EXPECT_CALL(mock_motor_left, stop).Times(AnyNumber());
	EXPECT_CALL(mock_motor_right, stop).Times(AnyNumber());

	rc.stopActuatorsAndLcd();
}

TEST_F(RobotControllerTest, suspendHardwareForDeepSleep)
{
	mock::DeepSleepEnabled mock_deep_sleep_enabled {};

	auto components = std::to_array<interface::DeepSleepEnabled *>({&mock_deep_sleep_enabled});
	rc.registerDeepSleepEnabledComponents(components);

	EXPECT_CALL(mock_deep_sleep_enabled, enableDeepSleep);
	rc.suspendHardwareForDeepSleep();
}

TEST_F(RobotControllerTest, wakeUp)
{
	rc.wakeUp();

	// nothing expected
}

TEST_F(RobotControllerTest, onChargeDidStart)
{
	{
		EXPECT_CALL(battery, isCharging).WillOnce(Return(false));
		EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _));
	}	// TODO: Expect raise call

	on_charge_did_start();
}

TEST_F(RobotControllerTest, onChargeDidStop)
{
	on_charge_did_stop();

	// TODO: Expect raise call
}

TEST_F(RobotControllerTest, isCharging)
{
	auto actual_is_charging = bool {};

	// TODO: Specify which BLE service and what is expected if necessary
	EXPECT_CALL(mbed_mock_gatt, write(_, _, _, _)).Times(AnyNumber());

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(true));
	actual_is_charging = rc.isCharging();
	EXPECT_TRUE(actual_is_charging);

	EXPECT_CALL(battery, isCharging).WillRepeatedly(Return(false));
	actual_is_charging = rc.isCharging();
	EXPECT_FALSE(actual_is_charging);
}

TEST_F(RobotControllerTest, isBleConnected)
{
	rc.isBleConnected();

	// ? Cannot be tested
}
